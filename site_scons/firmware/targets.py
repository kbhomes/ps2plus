from enum import Enum

class FirmwareTarget(Enum):
    FIRMWARE = 'firmware'
    BOOTLOADER = 'bootloader'

ALL_TARGETS = [t.value for t in FirmwareTarget]