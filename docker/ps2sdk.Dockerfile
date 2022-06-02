FROM ps2dev/ps2dev:latest
RUN apk update
RUN apk add build-base make git vim python3 py3-pip

# Install the SCons build tool
RUN pip3 install scons

# Fix Git repository settings (used to determine version numbers)
RUN git config --global --add safe.directory /project