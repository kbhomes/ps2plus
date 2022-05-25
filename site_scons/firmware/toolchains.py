class AbstractFirmwareToolchain:
    name: str

    # On Windows, without this override, all C compiler flags use MSVC-style syntax.
    tools = ['cc', 'gnulink', 'ar']

    def setup_env(self, env):
        pass
    
class MicrochipXC8Toolchain(AbstractFirmwareToolchain):
    name = 'MicrochipXC8Toolchain'
    mcu: str

    def __init__(self, mcu):
        self.mcu = mcu

    def setup_env(self, env):
        xc8_driver = 'C:/Program Files/Microchip/xc8/v2.36/bin/xc8-cc.exe',
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