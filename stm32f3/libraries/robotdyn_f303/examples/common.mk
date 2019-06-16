# common.mk - called by example programs 

.phony: all build clean install

OPENOCD_ARGS  = -f interface/stlink.cfg
OPENOCD_ARGS += -c 'transport select hla_swd'
OPENOCD_ARGS += -f target/stm32f3x.cfg
OPENOCD_LOG   = $(BUILD_DIR)/openocd.log

HEX := build/$(TARGET).ino.hex

all: build
	$(ARDUINO_DIR)/arduino-builder -compile $(V) \
	-fqbn=ricks_core:stm32f3:robotdyn:mcpu=robotdyn_f303cc,uploader=openocd \
	-ide-version=10809 -warnings=all \
	-hardware $(ARDUINO_DIR)/hardware \
	-tools $(ARDUINO_DIR)/tools-builder \
	-build-path build \
	$(TARGET).ino

# -ide-version=10809 -warnings=all -verbose \

build:
	mkdir -p build

clean:
	rm -rf build

install:
	openocd $(OPENOCD_ARGS) -c "program $(HEX) verify reset exit"

$(V).SILENT:


# vim: set ts=2 sw=2 noexpandtab : 

