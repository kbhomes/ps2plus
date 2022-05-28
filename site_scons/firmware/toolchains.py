from mplabx import MPLABXProperties
from vscode import VSCodeProperties

class AbstractFirmwareToolchain:
    name: str
    vscode_properties: VSCodeProperties

    # On Windows, without this override, all C compiler flags use MSVC-style syntax.
    tools = ['cc', 'gnulink', 'ar']

    def setup_env(self, env):
        pass
    
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
    xc8_directory = 'C:/Program Files/Microchip/xc8/v2.36'

    def __init__(self, mcu: str):
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
        self.vscode_properties.compiler_path = f'{self.xc8_directory}/pic/bin/clang.exe'

    def setup_env(self, env):
        xc8_driver = f'{self.xc8_directory}/bin/xc8-cc.exe',
        xc8_flags = ' '.join([
            '-mcpu=' + self.mcu,        # Target device for the compiler
            '-std=c99',                 # Compile as C99
            '-O2',                      # Highest optimization level available without a professional license
            '-Wl,--data-init',          # Clearing and initialization of C objects at runtime startup
            '-mdefault-config-bits',    # Used to have the compiler program default values for those configuration bits that have not been specified in the code using the config pragma
        ])
        
        env.Replace(
            CC=xc8_driver,
            CCFLAGS=xc8_flags,
            OBJSUFFIX='.p1',
            LINK=xc8_driver,
            LINKFLAGS=xc8_flags,
            
            # Even though XC8 generates ELFs as the main executable, we really only
            # care about the HEX file it also generates as this is used for flashing.
            PROGSUFFIX='.hex',
        )

    def chipname(self):
        return self.mcu.removeprefix('PIC')