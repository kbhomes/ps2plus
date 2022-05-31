FROM ubuntu:22.04

ARG XC8_VERSION=2.36

RUN dpkg --add-architecture i386 && \
  apt-get update && \
  apt-get install -y libc6:i386 libx11-6:i386 libxext6:i386 libstdc++6:i386 libexpat1:i386 wget sudo make git python3 python3-pip && \
  apt-get clean && \
  apt-get autoremove && \
  rm -rf /var/lib/apt/lists/*

# Install the SCons build tool
RUN pip3 install scons

# Fix Git repository settings (used to determine version numbers)
RUN git config --global --add safe.directory /project

# Download and install XC8
RUN wget -nv -O /tmp/xc8 "https://ww1.microchip.com/downloads/en/DeviceDoc/xc8-v${XC8_VERSION}-full-install-linux-x64-installer.run" && \
    chmod +x /tmp/xc8 && \
    /tmp/xc8 --mode unattended --unattendedmodeui none --netservername localhost --LicenseType FreeMode --prefix "/opt/microchip/xc8" && \
    rm /tmp/xc8

ENV PATH $PATH:/opt/microchip/xc8/bin

RUN [ -x /opt/microchip/xc8/bin/xc8 ] && xc8 --ver

