#include "firmware-update.h"

#include <memory.h>
#include <sstream>
#include <stdint.h>

#define MAX_DATA_LENGTH 16

using namespace std;

const vector<string> FirmwareUpdate::_testFilenames = {
    "Upgrade",
    "Downgrade",
    "No Change",
    "Configuration Change",
    "Microcontroller Mismatch",
    "Invalid Update"
};

const vector<string> FirmwareUpdate::GetTestFilenames() {
    vector<string> names;
    for (int i = 0; i < _testFirmware.size(); i++) {
        names.push_back("V" + to_string(i + 1));
    }

    names.insert(names.end(), _testFilenames.begin(), _testFilenames.end());
    return names;
    // return _testFilenames;
}

enum HEXRecordType {
    HEXData = 0,
    HEXEndOfFile = 1,
    HEXExtendedSegmentAddress = 2,
    HEXStartSegmentAddress = 3,
    HEXExtendedLinearAddress = 4,
    HEXStartLinearAddress = 5,
};

bool validateChecksum(uint8_t length, uint16_t address, uint8_t type, uint8_t *data, uint8_t checksum) {
    uint8_t value = 0;
    value += length;
    value += (address >> 8);
    value += address & 0xFF;
    value += type;
    
    for (int i = 0; i < length; i++) {
        value += data[i];
    }

    value += checksum;
    return (value & 0xFF) == 0;
}

FirmwareUpdate::FirmwareUpdate(string filename) {
    ifstream ifs;
    istringstream iss;

    unique_ptr<istream> input_stream;
    std::string line;
    uint32_t base = 0x00;
    shared_ptr<ps2plus_bootloader_update_record> record;

    // Holding area for firmware update "magic" data
    uint8_t magicData[VERSION_MAGIC_TOTAL_SIZE] = { 0 };

    if (filename.find("V") == 0) {
        printf("Got a V-firmware\n");
        int index = std::stoi(filename.substr(1)) - 1;
        printf("Opening firmware %d\n", index);
        iss.str(_testFirmware[index]);
    } else if (filename == "Upgrade") {
        _firmwareVersion = 40;
        _microcontrollerVersion = "PIC18F46K42";
        _configurationVersion = 1;
        for (int i = 0; i < 1000; i++) 
            _records.push_back(std::make_shared<ps2plus_bootloader_update_record>());
        return;
    } else if (filename == "Downgrade") {
        _firmwareVersion = 20;
        _microcontrollerVersion = "PIC18F46K42";
        _configurationVersion = 1;
        for (int i = 0; i < 1000; i++) 
            _records.push_back(nullptr);
        return;
    } else if (filename == "No Change") {
        _firmwareVersion = 30;
        _microcontrollerVersion = "PIC18F46K42";
        _configurationVersion = 1;
        for (int i = 0; i < 1000; i++) 
            _records.push_back(nullptr);
        return;
    } else if (filename == "Configuration Change") {
        _firmwareVersion = 31;
        _microcontrollerVersion = "PIC18F46K42";
        _configurationVersion = 3;
        for (int i = 0; i < 1000; i++) 
            _records.push_back(nullptr);
        return;
    } else if (filename == "Microcontroller Mismatch") {
        _firmwareVersion = 30;
        _microcontrollerVersion = "Arduino-Nano";
        _configurationVersion = 1;
        for (int i = 0; i < 1000; i++) 
            _records.push_back(nullptr);
        return;
    } else {
        ifs.open(filename);
        if (!ifs) {
            // TODO: Throw exception
        }
    }

    std::istream& is = ifs.is_open() ? static_cast<std::istream&>(ifs) : iss;

    // Create the start record
    record = make_shared<ps2plus_bootloader_update_record>();
    record->type = BLRecordTypeStart;
    record->target_address = 0xFFFF;
    record->data_length = 0;
    record->data_checksum = 0;
    _records.push_back(record);

    while (getline(is, line)) {
        uint8_t length;
        uint16_t address;
        uint8_t type;
        uint8_t data[MAX_DATA_LENGTH];
        uint8_t checksum;
        
        if (line.length() < 11) {
            // TODO: Throw exception InvalidLine (too few characters)
            cout << "[error] InvalidLine (too few characters)" << endl;
        }

        if (line[0] != ':') {
            // TODO: Throw exception InvalidToken (expected `:`)
            cout << "[error] InvalidToken (expected `:`)" << endl;
        }
        
        // Parse the record
        length = strtol(line.substr(1, 2).c_str(), NULL, 16);
        address = strtol(line.substr(3, 4).c_str(), NULL, 16);
        type = strtol(line.substr(7, 2).c_str(), NULL, 16);

        // Validate the length of the payload
        if (length > MAX_DATA_LENGTH) {
            // TODO: Throw exception InvalidRecordLength (too many data bytes)
            cout << "[error] InvalidRecordLength (too many data bytes, max " << MAX_DATA_LENGTH << ")" << endl;
        }

        for (int i = 0; i < length; i++) {
            data[i] = strtol(line.substr(9 + i*2, 2).c_str(), NULL, 16);
        }
        
        // Validate checksum
        checksum = strtol(line.substr(9 + length*2, 2).c_str(), NULL, 16);
        if (!validateChecksum(length, address, type, data, checksum)) {
            // TODO: Throw exception InvalidChecksum
            cout << "[error] InvalidChecksum" << endl;
        }


        if (type == HEXExtendedLinearAddress) {
            if (length != 2) {
                // TODO: Throw exception InvalidRecordLength (expected 2 bytes, got %d)
                cout << "[error] InvalidRecordLength (expected 2 bytes, got " << length << ")" << endl;
            }

            // Calculate the new base address
            base = (data[0] << 24) | (data[1] << 16);
        } else if (type == HEXData) {
            uint32_t target_address = base + address;

            // Determine if this record contains firmware "magic" data
            if ((target_address >= VERSION_MAGIC_BASE) && (target_address < VERSION_MAGIC_BASE + VERSION_MAGIC_TOTAL_SIZE)) {
                for (int i = 0; i < length; i++) {
                    magicData[target_address - VERSION_MAGIC_BASE + i] = data[i];
                }
            } else {
                record = make_shared<ps2plus_bootloader_update_record>();
                record->type = BLRecordTypeData;
                record->target_address = target_address;
                record->data_length = length;
                for (int i = 0; i < length; i++) {
                    record->data[i] = data[i];
                }
                record->data_checksum = ps2plus_bootloader_update_record_calculate_checksum(record.get());
                _records.push_back(record);
            }
        } else if (type == HEXEndOfFile) {
            record = make_shared<ps2plus_bootloader_update_record>();
            record->type = BLRecordTypeEnd;
            record->target_address = 0xFFFF;
            record->data_length = 0;
            record->data_checksum = ps2plus_bootloader_update_record_calculate_checksum(record.get());
            _records.push_back(record);
            break;
        } else {
            // TODO: Throw exception InvalidRecordType
            cout << "[error] InvalidRecordType (record " << type << " is not supported)" << endl;;
        }
    }

    printf("[info] Magic: ");
    for (size_t i = 0; i < VERSION_MAGIC_TOTAL_SIZE; i++) {
        printf("%02X ", magicData[i]);
    }
    printf("\n");

    // Parse magic version data
    _firmwareVersion = (magicData[VERSION_MAGIC_OFFSET_FIRMWARE] << 8) | (magicData[VERSION_MAGIC_OFFSET_FIRMWARE + 1]);
    _configurationVersion = (magicData[VERSION_MAGIC_OFFSET_CONFIGURATION] << 8) | (magicData[VERSION_MAGIC_OFFSET_CONFIGURATION + 1]);
    for (size_t i = 0; i < sizeof(VERSION_MICROCONTROLLER); i++) {
        _microcontrollerVersion += (char)magicData[VERSION_MAGIC_OFFSET_MICROCONTROLLER + i];
    }

    cout << "[info] Created " << _records.size() << " records" << endl;
}

FirmwareUpdate::~FirmwareUpdate() {

}

const vector<shared_ptr<ps2plus_bootloader_update_record>> FirmwareUpdate::GetRecords() {
    return _records;
}

uint16_t FirmwareUpdate::GetFirmwareVersion() {
    return _firmwareVersion;
}

const std::string FirmwareUpdate::GetMicrocontrollerVersion() {
    return _microcontrollerVersion;
}

uint8_t FirmwareUpdate::GetConfigurationVersion() {
    return _configurationVersion;
}

/** TEST FIRMWARE - These are real Intel HEX files **/
RESOURCE_EXTERNS(test_firmware_ps2plus_pic18f46k42_firmware_v1_hex);
RESOURCE_EXTERNS(test_firmware_ps2plus_pic18f46k42_firmware_v2_hex);
RESOURCE_EXTERNS(test_firmware_ps2plus_pic18f46k42_firmware_v3_hex);

const vector<string> FirmwareUpdate::_testFirmware = {
    string(reinterpret_cast<char *>(RESOURCE_POINTER(test_firmware_ps2plus_pic18f46k42_firmware_v1_hex)), RESOURCE_SIZE(test_firmware_ps2plus_pic18f46k42_firmware_v1_hex)),
    string(reinterpret_cast<char *>(RESOURCE_POINTER(test_firmware_ps2plus_pic18f46k42_firmware_v2_hex)), RESOURCE_SIZE(test_firmware_ps2plus_pic18f46k42_firmware_v2_hex)),
    string(reinterpret_cast<char *>(RESOURCE_POINTER(test_firmware_ps2plus_pic18f46k42_firmware_v3_hex)), RESOURCE_SIZE(test_firmware_ps2plus_pic18f46k42_firmware_v3_hex)),
};