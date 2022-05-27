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
    vscode_properties = VSCodeProperties(
        c_standard='c99',
        intellisense_mode='clang-x64',
    )

    # Additional parameters needed by this toolchain
    mcu: str
    xc8_directory = 'C:/Program Files/Microchip/xc8/v2.36'

    def __init__(self, mcu):
        self.mcu = mcu
        self.vscode_properties.additional_includes += [
            f'{self.xc8_directory}/pic/include',
            f'{self.xc8_directory}/pic/include/c99',
            f'{self.xc8_directory}/pic/include/proc',
        ]
        self.vscode_properties.additional_defines += [
            'HI_TECH_C',
            '_PIC18',
            '__CLANG__',
            '__XC',
            '__XC8',
            f'_{self.chipname()}',
            f'__{self.chipname()}'
        ]

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