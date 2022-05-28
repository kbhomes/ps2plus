from . import toolchains
from .targets import FirmwareTarget

class AbstractFirmwarePlatform:
    name: str
    toolchain: toolchains.AbstractFirmwareToolchain

    def setup_env_for_target(self, env, target):
        pass

    def generate_ide_project_configuration_for_target(self, env, target):
        pass

    def generate_ide_project(self, env, project_path: str, source_files: list[str]):
        pass

class PIC18F46K42Platform(AbstractFirmwarePlatform):
    name = 'PIC18F46K42'
    toolchain = toolchains.MicrochipXC8Toolchain('PIC18F46K42')

    mplab_configurations = []

    def setup_env_for_target(self, env, target):
        if target == FirmwareTarget.BOOTLOADER.value:
            # Restrict the program memory range to the first 0x4000 bytes
            env.Append(LINKFLAGS=' -mrom=0000-3FFF')
        elif target == FirmwareTarget.FIRMWARE.value:
            # Set the offset of program memory to 0x4000
            env.Append(LINKFLAGS=' -mcodeoffset=0x4000')

    def generate_ide_project_configuration_for_target(self, env, target):
        self.mplab_configurations.append(
            env.MplabxNbprojectConfiguration(
                name=f'{target}_{self.name}',
                properties=self.toolchain.mplabx_properties,       
                additional_linker_properties={
                    'additional-options-code-offset': '0x4000' if target == FirmwareTarget.FIRMWARE.value else '0x0000',
                    'code-model-rom': '0000-3FFF' if target == FirmwareTarget.BOOTLOADER.value else '',
                },
            )
        )

    def generate_ide_project(self, env, project_path: str, source_files: list[str]):
        env.AlwaysBuild(env.MplabxNbproject(
            env.Dir(project_path), 
            self.mplab_configurations, 
            source_files=source_files))

ALL_PLATFORMS = [PIC18F46K42Platform()]