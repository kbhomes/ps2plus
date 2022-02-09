# PS2+

Under construction!

## Overview

## Requirements

### Software

### Hardware

## Build instructions

### Firmware

### Configurator

[Guide for developing with PS2SDK and Docker](https://www.psx-place.com/threads/how-to-develop-ps2-homebrew-using-docker-containers.29907/)

Build a Docker image with the PS2SDK toolchain:

```
docker build -t ps2plus .
```

Start a Docker container with the created Docker image. It binds the repository's folder to the `/project` 
path in the container and opens shell at that path.

```
docker run -it -w /project -v ${PWD}:/project ps2plus sh
```

Build the application:

```
make -C configurator
```

## Development
    
### Repository layout
    
### Platform porting

## References

## License

See [LICENSE](https://github.com/kbhomes/ps2plus/blob/main/LICENSE).