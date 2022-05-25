from . import toolchains
from .targets import FirmwareTarget

class AbstractFirmwarePlatform:
    name: str
    toolchain: toolchains.AbstractFirmwareToolchain

    def setup_env_for_target(self, env, target):
        pass

class PIC18F46K42Platform(AbstractFirmwarePlatform):
    name = 'PIC18F46K42'
    toolchain = toolchains.MicrochipXC8Toolchain('PIC18F46K42')

    def setup_env_for_target(self, env, target):
        if target == FirmwareTarget.BOOTLOADER.value:
            # Restrict the program memory range to the first 0x4000 bytes
            env.Append(LINKFLAGS=' -mrom=0000-3FFF')
        elif target == FirmwareTarget.FIRMWARE.value:
            # Set the offset of program memory to 0x4000
            env.Append(LINKFLAGS=' -mcodeoffset=0x4000')

ALL_PLATFORMS = [PIC18F46K42Platform()]