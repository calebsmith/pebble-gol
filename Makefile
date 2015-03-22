.PHONY: build install watch-install all clean

build:
	pebble build

install:
	echo "Set env variable PEBBLE_PLATFORM to aplite or basalt"
	pebble install --emulator $$PEBBLE_PLATFORM

watch-install:
	echo "Set env variable PEBBLE_PHONE to correct IP"
	pebble install --phone $$PEBBLE_PHONE

all:
	make build && make install

clean:
	rm -rf build
