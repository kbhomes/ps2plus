#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <stdint.h>

#include "resource.h"
#include "../../../shared/bootloader.h"
#include "../../../shared/versions.h"

class FirmwareUpdate {
public:
    FirmwareUpdate(std::string filename);
    ~FirmwareUpdate();

    const std::vector<std::shared_ptr<ps2plus_bootloader_update_record>> GetRecords();
    uint16_t GetFirmwareVersion();
    const std::string GetMicrocontrollerVersion();
    uint8_t GetConfigurationVersion();

    static const std::vector<std::string> GetTestFilenames();

private:
    std::vector<std::shared_ptr<ps2plus_bootloader_update_record>> _records;
    uint16_t _firmwareVersion;
    std::string _microcontrollerVersion;
    uint8_t _configurationVersion;

    static const std::vector<std::string> _testFilenames;
    static const std::vector<std::string> _testFirmware;
};
