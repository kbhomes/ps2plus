from . import toolchains, targets
import os
import shutil
import subprocess
import sys

class AbstractFirmwarePlatform:
    name: str
    toolchain: toolchains.AbstractFirmwareToolchain
    valid_targets: list[targets.Target] = targets.ALL_TARGETS

    def setup_env_for_target(self, env, target: targets.Target):
        pass

    def execute_test_for_target(self, env, target: targets.Target, build_folder: str, executable: object) -> int:
        pass

    def generate_ide_project_configuration_for_target(self, env, target: targets.Target):
        pass

    def generate_ide_project(self, env, project_path: str, source_files: list[str]):
        pass

class NativePlatform(AbstractFirmwarePlatform):
    name = 'native'
    toolchain = toolchains.NativeToolchain()
    valid_targets = [targets.TestBootloaderTarget, targets.TestFirmwareTarget]

    def setup_env_for_target(self, env, target: targets.Target):
        if target.is_test:
            env.Append(
                CFLAGS=[
                    '-g',                   # Debugging symbols,
                    '-fprofile-arcs',       # Profiles program control flow (required for coverage)
                    '-ftest-coverage',      # Generates test coverage data
                ],
                LIBS=[
                    'gcov',                 # Link the coverage library
                ]
            )

    def execute_test_for_target(self, env, target: targets.Target, build_folder: str, executable: object) -> int:
        # Native tests can be directly executed on the host
        ret = subprocess.run(str(executable), stdout=sys.stdout).returncode

        if not ret:
            # Run coverage report generation
            if shutil.which('gcovr'):
                coverage_folder = os.path.join(build_folder, 'coverage')
                coverage_report = os.path.join(coverage_folder, 'index.html')

                # Delete any existing coverage data
                if os.path.isdir(coverage_folder):
                    shutil.rmtree(coverage_folder)

                # Create the coverage report folder
                os.makedirs(coverage_folder, exist_ok=True)

                # Execute gcovr to generate the coverage report from gcov
                coverage_command = [
                    'gcovr', 
                    '--exclude=firmware/test', 
                    '--print-summary',
                    '--html-details=' + coverage_report,
                    f'--html-title=PS2+ Code Coverage - Platform: {self.name} - Target: {target.name}',
                ]
                print(f'Generating coverage report: {" ".join(coverage_command)}')
                ret = subprocess.run(coverage_command).returncode
            else:
                print('Unable to run coverage report due to missing `gcovr` executable')

        return ret

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