#include "../shared/versions.h"

#include "argparse.hpp"

#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#define INTEL_HEX_EOF_RECORD ":00000001FF"

std::array<uint8_t, 16> buildArray(uint8_t *data, size_t length) {
    std::array<uint8_t, 16> buffer;
    for (int i = 0; i < length && i < 16; i++) {
        buffer[i] = data[i];
    }
    return buffer;
}

std::array<uint8_t, 16> buildArray(uint16_t value) {
    std::array<uint8_t, 16> buffer;
    buffer[0] = value >> 8;
    buffer[1] = value & 0xFF;
    return buffer;
}

std::string buildRecord(uint8_t length, uint32_t address, uint8_t type, std::array<uint8_t, 16> data) {
    uint8_t checksum = 0;
    std::ostringstream out;
    out << std::uppercase << std::hex << std::setfill('0');

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

int main(int argc, char *argv[]) {
    argparse::ArgumentParser program("firmware-version-helper");
    program.add_argument("-f", "--firmware-hex")
        .help("path to firmware Intel HEX file")
        .required();
    program.add_argument("--version-firmware")
        .help("override firmware version number (hex)")
        .default_value(VERSION_FIRMWARE)
        .scan<'x', uint16_t>();
    program.add_argument("--version-microcontroller")
        .help("override microcontroller version number (string, max length of 32)")
        .default_value(std::string(VERSION_MICROCONTROLLER));
    program.add_argument("--version-configuration")
        .help("override configuration version number (hex)")
        .default_value(VERSION_CONFIGURATION)
        .scan<'x', uint16_t>();

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    auto firmware_hex_path = program.get<std::string>("firmware-hex");
    auto version_firmware = program.get<uint16_t>("version-firmware");
    auto version_microcontroller = program.get<std::string>("version-microcontroller");
    auto version_configuration = program.get<uint16_t>("version-configuration");

    // Widen the microcontroller version to 32 characters if it's smaller
    if (version_microcontroller.size() < 32) {
        version_microcontroller.append(32 - version_microcontroller.size(), '\0');
    }

    // Print all of the original firmware file
    std::ifstream firmware_hex_file(firmware_hex_path);
    std::string line;
    if (firmware_hex_file.is_open()) {
        while (std::getline(firmware_hex_file, line)) {
            if (line.find(INTEL_HEX_EOF_RECORD) == 0) {
                // Skip the EOF record from the original firmware file
                break;
            }

            std::cout << line << std::endl;
        }
        firmware_hex_file.close();
    } else {
        std::cerr << "Unable to open file: " << firmware_hex_path << std::endl;
        std::exit(1);
    }

    // Print the data records containing the version information
    std::cout << buildRecord(sizeof(version_firmware), VERSION_MAGIC_BASE + VERSION_MAGIC_OFFSET_FIRMWARE, 0, buildArray(version_firmware)) << std::endl;
    std::cout << buildRecord(16, VERSION_MAGIC_BASE + VERSION_MAGIC_OFFSET_MICROCONTROLLER, 0, buildArray((uint8_t *)(version_microcontroller.c_str()), 16)) << std::endl;
    std::cout << buildRecord(16, VERSION_MAGIC_BASE + VERSION_MAGIC_OFFSET_MICROCONTROLLER + 16, 0, buildArray((uint8_t *)(version_microcontroller.c_str()) + 16, 16)) << std::endl;
    std::cout << buildRecord(sizeof(version_configuration), VERSION_MAGIC_BASE + VERSION_MAGIC_OFFSET_CONFIGURATION, 0, buildArray(version_configuration)) << std::endl;
    
    // End the merged file
    std::cout << INTEL_HEX_EOF_RECORD << std::endl;

    return 0;
}