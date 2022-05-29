# PS2+

Under construction!

## Overview

## Requirements

### Software

### Hardware

## Build instructions

### Firmware

### Configurator

As with each firmware platform, there are two options for building the configurator app: using a local installation of the PS2SDK, or using this repository's Docker image.

#### Local PS2SDK installation

(TBD)

#### Repository Docker image

The configurator can be built locally using the same infrastructure that the GitHub CI build system uses. With [Docker](https://www.docker.com/products/docker-desktop/) installed, the following will directly build the configurator application:

```
docker compose -f docker/docker-compose.yml run configurator
```

On the first run, this command will build the Docker image and then immediately build the configurator, which will be available as `dist/PS2PLMAN.elf`. On future runs, the Docker image will be cached. To obtain an interactive shell into the Docker image in order to control the build step:

```
docker compose -f docker/docker-compose.yml run configurator sh
```

From within the interactive shell, you can run `make` to build the configurator and `make clean` to clear the build.

## Development
    
### Repository layout
    
### Platform porting

The firmware/bootloader's platform abstraction layer makes porting to new hardware relatively straightforward. New ports need modifications in the following locations:

- 👩‍💻 **Platform code**. Create a new folder in the `firmware/src/platforms` directory. This should contain implementations of the platform abstraction functions defined in the `firmware/src/platforms/*.h` header files. Implementation files should be `#ifdef`-guarded using the `PLATFORM_<NAME>` definition (provided by the build system).

- 🛠 **Platform/toolchain build configuration**:
    - **Toolchain configuration**. If the new platform uses a different toolchain than any existing platform, a new SCons toolchain has to be created for building. Create a new toolchain class in `site_scons/firmware/toolchains.py`. Its job is to set the correct C compiler/linker paths and flags so that the firmware source files can be properly built.

    - **Platform configuration**. Create a new platform class in `site_scons/firmware/platforms.py`. Its job is to select the correct toolchain, and to set the correct C compiler/linker flags to differentiate between the `bootloader` and `firmware` targets (the bootloader and firmware occupy different spaces in program memory). The platform class can also generate platform-specific IDE project files if desired. Include a new instance of this class in the `ALL_PLATFORMS` list.

- 🐳 **Docker-based continuous integration**:
    - **Docker toolchain image**. If the new platform uses a different toolchain than any existing platform, a new Docker image has to be created for building. Create a new `<toolchain>.Dockerfile` file in the `docker/` directory; this image should have the following installed:
        - Platform toolchain
        - `python3` and `python3-pip` (required for the build system and tools)
        - `scons` (via `pip3`; required for the build system)

    - **Docker Compose platform command**. Add the new platform to `docker/docker-compose.yml` as a new service named `firmware-<PLATFORM>` (must be all lowercase). If the platform also uses a new toolchain, add the toolchain to this same file in the `x-toolchains` section. **(TBD: Add a command that specifies to SCons which platform will be built.)**

    - **GitHub CI platform build**. The new platform has to be integrated into this repository's GitHub Workflows-based continuous integration building. **(TBD: This CI step should probably be refactored as a local GitHub Action that takes a list of platform names as inputs.)**

## References

## License

See [LICENSE](https://github.com/kbhomes/ps2plus/blob/main/LICENSE).