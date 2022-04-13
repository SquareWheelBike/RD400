
# arduino-cli install instructions https://siytek.com/arduino-cli-raspberry-pi/

AC = arduino-cli
# USB = /dev/ttyACM0
USB = COM8
MAIN = arduino
BOARD = arduino:avr:nano:cpu=atmega328old
# BOARD = arduino:avr:micro
PROGRAMMER = avrispmkii

# LIST PROGRAMMERS
# arduino-cli board details -b arduino:avr:micro --list-programmers

# verbose cli flag is -v or --verbose

# arduino-cli documentation [here](https://arduino.github.io/arduino-cli/0.21/commands/arduino-cli/)

libraries:
# 	install the avr binaries for micro/nano
	$(AC) core install arduino:avr

# attach:
# 	$(AC) board attach -b $(BOARD) -p $(USB) $(MAIN)

build:
	$(AC) compile --fqbn $(BOARD) $(MAIN)

vbuild:
	$(AC) compile -v --fqbn $(BOARD) $(MAIN)

# upload:
#	 $(AC) upload --port $(USB) --fqbn $(BOARD) --programmer $(PROGRAMMER) $(MAIN)

clean:
	$(AC) cache clean -v

find:
	$(AC) core update-index
	$(AC) board list
