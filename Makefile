TARGETS = tools configurator release
ROOTDIR = $(realpath $(CURDIR))

all: $(TARGETS)

tools:
	$(MAKE) -c tools ROOTDIR=$(ROOTDIR) PROJECTDIR=tools

configurator:
	$(MAKE) -C configurator ROOTDIR=$(ROOTDIR) PROJECTDIR=configurator

release:
	tools/firmware-version-helper --firmware-hex "firmware/dist/Firmware/production/firmware.production.hex" > build/dist/ps2plus.firmware-only.hex

clean:
	$(MAKE) -C configurator clean ROOTDIR=$(ROOTDIR) PROJECTDIR=configurator
	rm -rf $(ROOTDIR)/build

.PHONY: configurator