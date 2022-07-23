FROM ubuntu:22.04

RUN dpkg --add-architecture i386 && \
  apt-get update && \
  apt-get install -y git gcc python3 python3-pip && \
  apt-get clean && \
  apt-get autoremove && \
  rm -rf /var/lib/apt/lists/*

# Install the SCons build tool
RUN pip3 install scons

# Fix Git repository settings (used to determine version numbers)
RUN git config --global --add safe.directory /project

RUN gcc --version
