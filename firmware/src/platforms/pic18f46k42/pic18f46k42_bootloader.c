#ifdef PLATFORM_PIC18F46K42

#include "pic18f46k42_platform.h"

#define PIC_FLASH_BLOCK_SIZE    128
#define PIC_FLASH_END_ADDRESS   0x10000

// Bitmask that can be and-ed with an address to get the offset of the address within its flash memory block
#define PIC_FLASH_BLOCK_OFFSET_MASK   (PIC_FLASH_BLOCK_SIZE-1)

// Bitmask that can be and-ed with an address to get the flash memory block that the address is a part of
// (i.e.: address == ((address & PIC_FLASH_BLOCK_ADDRESS_MASK) + (address & PIC_FLASH_BLOCK_OFFSET_MASK))
#define PIC_FLASH_BLOCK_ADDRESS_MASK  ((PIC_FLASH_END_ADDRESS-1) ^ PIC_FLASH_BLOCK_OFFSET_MASK)

// Firmware "jump vector"
platform_bootloader_execute_firmware_function execute_firmware_function = PIC_FIRMWARE_BASE;

void pic_flash_begin_operation() {
  NVMCON1bits.NVMREG = 2;
  NVMCON1bits.WREN = 1;
  
  bool interrupts_enabled = GIE;
  GIE = 0;
  NVMCON2 = 0x55;
  NVMCON2 = 0xAA;
  NVMCON1bits.WR = 1;
  GIE = interrupts_enabled;
  
  NVMCON1bits.WREN = 0;
}

bool pic_flash_erase_block(uint32_t address) {
  if (address >= PIC_FLASH_END_ADDRESS) {
    return false;
  }
  
  // Prepare the block to be erased
  TBLPTR = (address & 0xFFFFFF);
  NVMCON1bits.FREE = 1;

  // Perform the NVM unlock sequence and issue the flash erase
  pic_flash_begin_operation();
  
  return true;
}

uint8_t pic_flash_read(uint32_t address) {
  NVMCON1bits.NVMREG = 2;
  TBLPTR = (address & 0xFFFFFF);
  asm("TBLRD");
  return TABLAT;
}

uint16_t pic_flash_read_short(uint32_t address) {
  uint16_t upper = ((uint16_t)pic_flash_read(address + 1)) << 8;
  uint16_t lower = pic_flash_read(address);
  return upper | lower;
}

bool pic_flash_write_block(uint32_t address, uint8_t *block) {
  if (address >= PIC_FLASH_END_ADDRESS) {
    return false;
  }
  
  uint32_t blockStartAddr  = (uint32_t)(address & PIC_FLASH_BLOCK_ADDRESS_MASK);

  // Flash write must start at the beginning of a row
  if (address != blockStartAddr) {
    return false;
  }

  // Block erase sequence
  if (!pic_flash_erase_block(address)) {
    return false;
  }

  // Block write sequence
  TBLPTR = (address & 0xFFFFFF);

  // Write block of data
  for (int i = 0; i < PIC_FLASH_BLOCK_SIZE; i++) {
    TABLAT = block[i];  // Load data byte

    if (i == (PIC_FLASH_BLOCK_SIZE - 1)) {
      asm("TBLWT");
    } else {
      asm("TBLWTPOSTINC");
    }
  }
  
  // Perform the NVM unlock sequence and issue the flash write
  pic_flash_begin_operation();
  
  return true;
}

// Function is marked as reentrant to allow recursion
bool pic_flash_write_bytes(uint32_t address, uint8_t *bytes, size_t length) {
  static uint8_t buffer[PIC_FLASH_BLOCK_SIZE];
  
  while (length) {
    if (address + length > PIC_FLASH_END_ADDRESS) {
      return false;
    }

    uint32_t blockStartAddr = (uint32_t)(address & PIC_FLASH_BLOCK_ADDRESS_MASK);
    uint8_t offset = (uint8_t)(address & PIC_FLASH_BLOCK_OFFSET_MASK);
    size_t written = 0;

    // Entire row will be erased, read and save the existing data
    for (int i = 0; i < PIC_FLASH_BLOCK_SIZE; i++) {
      buffer[i] = pic_flash_read(blockStartAddr + i);
    }

    // Load bytes at offset
    for (int j = 0; j < length && offset + j < PIC_FLASH_BLOCK_SIZE; j++) {
      buffer[offset + j] = bytes[j];
      written++;
    }

    // Writes buffer contents to current block
    if (!pic_flash_write_block(blockStartAddr, buffer)) {
      return false;
    }

    // Advance write pointers and resume the loop (if there is more data to write)
    address += written;
    bytes += written;
    length -= written;
  }
  
  return true;
}

bool pic_flash_write(uint32_t address, uint8_t byte) {
  static uint8_t buffer[1];
  buffer[0] = byte;
  return pic_flash_write_bytes(address, buffer, 1);
}

void platform_bootloader_execute_firmware(void) {
  // Disable all interrupts
  GIE = 0;
  
  // "Jump" to the firmware's start address
  execute_firmware_function();
}

bool platform_bootloader_erase_firmware(void) {
  for (uint32_t block = PIC_FIRMWARE_BASE; block + PIC_FLASH_BLOCK_SIZE < PIC_FLASH_END_ADDRESS; block += PIC_FLASH_BLOCK_SIZE) {
    if (!pic_flash_erase_block(block)) {
      return false;
    }
  }
  
  return true;
}

platform_bootloader_update_record_address_validity platform_bootloader_validate_update_record_address(ps2plus_bootloader_update_record *record) {
  if (record->target_address < PIC_FIRMWARE_BASE) {
    // Writes in the bootloader are invalid
    return BLAddressInvalid;
  } else if (record->target_address + record->data_length >= PIC_FLASH_END_ADDRESS) {
    // Writes past flash memory (typically configuration IDs or device words) are ignored
    return BLAddressIgnore;
  } else {
    // Writes in non-bootloader flash memory are valid
    return BLAddressValid;
  }
}

bool platform_bootloader_flash_update_record(ps2plus_bootloader_update_record *record) {
  pic_flash_write_bytes(record->target_address, record->data, record->data_length);
}

void print_range(uint32_t start, uint32_t end) {
  for (uint32_t address = start; address < end; address++) {
    uint8_t byte = pic_flash_read(address);
    
    if (address % 16 == 0) {
      printf("\n[%08X] ", address);
    }
    
    printf("%02X ", byte);
  }
  
  printf("\n");
}

uint8_t platform_bootloader_calculate_firmware_checksum() {
//  // DEV: ERASE!
//  {
//    puts("[flash] Erasing firmware!");
//    
//    for (uint32_t block = PIC_FIRMWARE_BASE; block + PIC_FLASH_BLOCK_SIZE < PIC_FLASH_END_ADDRESS; block += PIC_FLASH_BLOCK_SIZE) {
//      pic_flash_erase_block(block);
//    }
//  }
//  
//  // DEV: WRITE!
//  {
//    puts("[flash] Writing individual byte: 0x7F at 0x4000");
//    pic_flash_write(0x4000, 0x7F);
//    
//    static char data[12] = "Hello World!";
//    puts("[flash] Writing multiple bytes: \"Hello World!\" at 0x4010");
//    pic_flash_write_bytes(0x4010, data, 12);
//    
//    static uint8_t empty_block[PIC_FLASH_BLOCK_SIZE] = {};
//    puts("[flash] Writing block: 128 zeroes at 0x4080");
//    pic_flash_write_block(0x4080, empty_block);
//  }
//  
//  // DEV: PRINT!
//  printf("[flash] First 256 bytes of bootloader:");
//  print_range(0x0000, 0x0100);
//  printf("[flash] First 256 bytes of firmware:");
//  print_range(0x4000, 0x4100);
  
  return 0;
}

#endif /* PLATFORM_PIC18F46K42 */