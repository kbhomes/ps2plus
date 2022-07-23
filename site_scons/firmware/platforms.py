from . import toolchains, targets

class AbstractFirmwarePlatform:
    name: str
    toolchain: toolchains.AbstractFirmwareToolchain
    valid_targets: list[targets.Target] = targets.ALL_TARGETS

    def setup_env_for_target(self, env, target: targets.Target):
        pass

    def generate_ide_project_configuration_for_target(self, env, target: targets.Target):
        pass

    def generate_ide_project(self, env, project_path: str, source_files: list[str]):
        pass

class NativePlatform(AbstractFirmwarePlatform):
    name = 'native'
    toolchain = toolchains.NativeToolchain()
    valid_targets = [targets.TestBootloaderTarget, targets.TestFirmwareTarget]

class PIC18F46K42Platform(AbstractFirmwarePlatform):
    name = 'PIC18F46K42'
    toolchain = toolchains.MicrochipXC8Toolchain('PIC18F46K42')
    valid_targets = [targets.BootloaderTarget, targets.FirmwareTarget]

    mplab_configurations = []

    def setup_env_for_target(self, env, target):
        if target == targets.BootloaderTarget:
            # Restrict the program memory range to the first 0x4000 bytes
            env.Append(LINKFLAGS=' -mrom=0000-3FFF')
        elif target == targets.FirmwareTarget:
            # Set the offset of program memory to 0x4000
            env.Append(LINKFLAGS=' -mcodeoffset=0x4000')

    def generate_ide_project_configuration_for_target(self, env, target):
        self.mplab_configurations.append(
            env.MplabxNbprojectConfiguration(
                name=f'{target.name}_{self.name}',
                properties=self.toolchain.mplabx_properties,       
                additional_linker_properties={
                    'additional-options-code-offset': '0x4000' if target == targets.FirmwareTarget else '0x0000',
                    'code-model-rom': '0000-3FFF' if target == targets.BootloaderTarget else '',
                },
            )
        )

    def generate_ide_project(self, env, project_path: str, source_files: list[str]):
        env.AlwaysBuild(env.MplabxNbproject(
            env.Dir(project_path), 
            self.mplab_configurations, 
            source_files=source_files))

ALL_PLATFORMS: list[AbstractFirmwarePlatform] = [
    NativePlatform(),
    PIC18F46K42Platform(),
]