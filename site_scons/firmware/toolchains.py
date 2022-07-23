from mplabx import MPLABXProperties
from vscode import VSCodeProperties
import os.path
import shutil

class AbstractFirmwareToolchain:
    name: str
    vscode_properties: VSCodeProperties

    # On Windows, without this override, all C compiler flags use MSVC-style syntax.
    tools = ['cc', 'gnulink', 'ar']

    def setup_env(self, env):
        pass

class NativeToolchain(AbstractFirmwareToolchain):
    name = 'NativeToolchain'
    tools = ['mingw', 'gnulink']
    vscode_properties = None

    def setup_env(self, env):
        env.Replace(
            CFLAGS=[
                '-g'       # Debugging symbols
            ],
            LIBS=[
                'm'        # Link the math library for <math.h>
            ]
        )
    
class MicrochipXC8Toolchain(AbstractFirmwareToolchain):
    name = 'MicrochipXC8Toolchain'

    # Projects using this toolchain can be edited in MPLAB X, so add the
    # MPLAB X project generator tool to the environment
    tools = AbstractFirmwareToolchain.tools + ['mplabx_nbproject']

    vscode_properties = VSCodeProperties(
        c_standard='c99',
        intellisense_mode='linux-clang-x64',
    )

    mplabx_properties = MPLABXProperties(
        toolchain='XC8',
        toolchain_version='2.36',
        compiler_properties={
            'optimization-level': '-O2',
            'warning-level': '-3',
        },
        linker_properties={
            'clear-bss': 'true',
            'display-class-usage': 'false',
            'display-hex-usage': 'false',
            'display-overall-usage': 'true',
            'display-psect-usage': 'false',
            'initialize-data': 'true',
            'program-the-device-with-default-config-words': 'true',
        },
        xc8_properties={
            'gcc-opt-driver-new': 'true',
            'gcc-opt-std': '-std=c99',
        }
    )

    # Additional parameters needed by this toolchain
    mcu: str

    # Additional Microchip XC8 state
    xc8_directory: str
    xc8_driver: str
    xc8_clang: str

    def __init__(self, mcu: str):
        self.detect()

        self.mcu = mcu
        self.mplabx_properties.device = mcu
        self.vscode_properties.additional_includes += [
            f'{self.xc8_directory}/pic/include',
            f'{self.xc8_directory}/pic/include/c99',
            f'{self.xc8_directory}/pic/include/proc',
        ]
        self.vscode_properties.additional_defines += [
            'HI_TECH_C',
            '__CLANG__',
            '__XC',
            '__XC8',
            f'_{self.chipname()}',
            f'__{self.chipname()}',
        ]

        if mcu.startswith('PIC18'):
            self.vscode_properties.additional_defines += [
                '_PIC18',
                '__PICC18__',
            ]

        # Point VS Code to the underlying clang executable so its IntelliSense is accurate (and doesn't default to MSVC)
        self.vscode_properties.compiler_path = self.xc8_clang

    def setup_env(self, env):
        xc8_flags = ' '.join([
            '-mcpu=' + self.mcu,        # Target device for the compiler
            '-std=c99',                 # Compile as C99
            '-O2',                      # Highest optimization level available without a professional license
            '-Wl,--data-init',          # Clearing and initialization of C objects at runtime startup
            '-mdefault-config-bits',    # Used to have the compiler program default values for those configuration bits that have not been specified in the code using the config pragma
        ])
        
        env.Replace(
            CC=self.xc8_driver,
            CCFLAGS=xc8_flags,
            OBJSUFFIX='.p1',
            LINK=self.xc8_driver,
            LINKFLAGS=xc8_flags,
            
            # Even though XC8 generates ELFs as the main executable, we really only
            # care about the HEX file it also generates as this is used for flashing.
            PROGSUFFIX='.hex',
        )

    def chipname(self):
        return self.mcu.removeprefix('PIC')

    def detect(self):
        # Go up two directories from the XC8 driver program
        self.xc8_directory = os.path.dirname(os.path.dirname(shutil.which('xc8-cc') or ''))
        self.xc8_driver = f"\"{os.path.join(self.xc8_directory, 'bin', 'xc8-cc')}\""
        self.xc8_clang = os.path.join(self.xc8_directory, 'pic', 'bin', 'clang')
        
        # TODO: Make toolchains optional (for other platforms), but fail the build when required?
        # if not self.xc8_directory:
        #     raise Exception('Could not find `xc8-cc[.exe]` on the path')