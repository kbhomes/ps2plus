TARGETS = configurator
ROOTDIR = $(realpath $(CURDIR))

all: $(TARGETS)

configurator:
	$(MAKE) -C configurator ROOTDIR=$(ROOTDIR) PROJECTDIR=configurator

clean:
	$(MAKE) -C configurator clean ROOTDIR=$(ROOTDIR) PROJECTDIR=configurator
	rm -rf $(ROOTDIR)/build

.PHONY: configurator