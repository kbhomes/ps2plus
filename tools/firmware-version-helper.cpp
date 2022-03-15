#include "../shared/versions.h"

#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#define HEXMATE_PATH "& \"C:\\Program Files\\Microchip\\MPLABX\\v6.00\\mplab_platform\\bin\\hexmate.exe\""

std::array<uint8_t, 16> buildArray(uint8_t *data, size_t length) {
    std::array<uint8_t, 16> buffer;
    for (int i = 0; i < length && i < 16; i++) {
        buffer[i] = data[i];
    }
    return buffer;
}

std::string buildRecord(uint8_t length, uint32_t address, uint8_t type, std::array<uint8_t, 16> data) {
    uint8_t checksum = 0;
    std::ostringstream out;
    out << std::hex << std::setfill('0');

    if (address > 0xFFFF) {
        // Create an extended address record
        out << ":";
        out << std::setw(2) << 2u;
        out << std::setw(4) << 0u; 
        out << std::setw(2) << 4u;
        out << std::setw(4) << unsigned(address >> 16);
        checksum = 2 + 0 + 4 + ((address >> 24) & 0xFF) + ((address >> 16) & 0xFF);
        out << std::setw(2) << unsigned((~checksum + 1) & 0xFF); // Checksum
        out << std::endl;
    }

    // Create the data record
    checksum = 0;
    out << ":";
    out << std::setw(2) << unsigned(length); checksum += length;
    out << std::setw(4) << unsigned(address & 0xFFFF); 
    checksum += (address >> 8) & 0xFF; 
    checksum += (address & 0xFF);
    out << std::setw(2) << unsigned(type);
    for (int i = 0; i < length; i++) {
        out << std::setw(2) << unsigned(data[i]);
        checksum += data[i];
    }
    out << std::setw(2) << unsigned((~checksum + 1) & 0xFF); // Checksum

    return out.str();
}

int main() {
    std::ofstream outfile("versions.hex");
    outfile << buildRecord(sizeof(VERSION_FIRMWARE), VERSION_MAGIC_BASE + VERSION_MAGIC_OFFSET_FIRMWARE, 0, { VERSION_FIRMWARE >> 8, VERSION_FIRMWARE & 0xFF }) << std::endl;
    outfile << buildRecord(16, VERSION_MAGIC_BASE + VERSION_MAGIC_OFFSET_MICROCONTROLLER, 0, buildArray((uint8_t *)VERSION_MICROCONTROLLER, 16)) << std::endl;
    outfile << buildRecord(16, VERSION_MAGIC_BASE + VERSION_MAGIC_OFFSET_MICROCONTROLLER + 16, 0, buildArray((uint8_t *)VERSION_MICROCONTROLLER + 16, 16)) << std::endl;
    outfile << buildRecord(sizeof(VERSION_CONFIGURATION), VERSION_MAGIC_BASE + VERSION_MAGIC_OFFSET_CONFIGURATION, 0, { VERSION_CONFIGURATION }) << std::endl;
    outfile << buildRecord(0, 0, 1, {}) << std::endl;

    // printf("%s ", HEXMATE_PATH);

    // // Firmware version
    // printf("-SERIAL=%04X@0x%lX ", VERSION_FIRMWARE, VERSION_MAGIC_OFFSET_FIRMWARE);

    // // Microcontroller version
    // for (size_t i = 0; i < sizeof(VERSION_MICROCONTROLLER); i++) {
    //     if (VERSION_MICROCONTROLLER[i] == 0) {
    //         break;
    //     }
    //     printf("-SERIAL=%02X@0x%lX ", VERSION_MICROCONTROLLER[i], VERSION_MAGIC_OFFSET_MICROCONTROLLER + i);
    // }

    // // Configuration version
    // printf("-SERIAL=%02X@0x%lX ", VERSION_CONFIGURATION, VERSION_MAGIC_OFFSET_CONFIGURATION);

    return 0;
}