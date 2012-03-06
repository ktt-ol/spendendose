ARDUINO_DIR  = ../Arduino.app/Contents/Resources/Java

TARGET       = CLItest
ARDUINO_LIBS = LiquidCrystal

BOARD_TAG    = uno
ARDUINO_PORT = /dev/tty.usb*

include ../arduino.mk/Arduino.mk
