# specify the board using the command line:
# make BOARD=[mbed board name]
# 
# if you're using the k64f, use the additional VARIANT flag
# to make sure that js2c works correctly
#
# make BOARD=K64F VARIANT=FRDM

BOARD=
VARIANT=
HEAPSIZE=16

DEBUG=0
MBED_VERBOSE=0

MBED_CLI_FLAGS=-j0 --source . --source ../../

ifeq ($(BOARD),K64F)
VARIANT=FRDM
endif

ifeq ($(DEBUG), 1)
MBED_CLI_FLAGS += -o debug-info 
endif

ifeq ($(MBED_VERBOSE), 1)
MBED_CLI_FLAGS += -v
else ifeq ($(MBED_VERBOSE), 2)
MBED_CLI_FLAGS += -vv
endif

MBED_CLI_FLAGS += -D "CONFIG_MEM_HEAP_AREA_SIZE=(1024*$(HEAPSIZE))"


.PHONY: all js2c getlibs rebuild
all: source/js_encoded.cpp .mbed
	mbed target $(BOARD)
	mbed compile $(MBED_CLI_FLAGS)

rebuild: clean all

clean:
	rm -rf .build/$(BOARD)

js2c: js/main.js js/flash_leds.js
	python tools/js2c.py --mcu=$(BOARD) --variant=$(VARIANT)

source/js_encoded.cpp: js2c

getlibs: .mbed
	mbed deploy

.mbed:
	mbed config root .
	mbed toolchain GCC_ARM
	mbed target $(BOARD)

../../.mbedignore:
	cp ./template-mbedignore.txt ../../.mbedignore
