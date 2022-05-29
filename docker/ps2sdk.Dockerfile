FROM ps2dev/ps2dev:latest
RUN apk update
RUN apk add build-base make git vim
RUN git config --global core.autocrlf true