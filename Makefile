CC = avr-gcc
CFLAGS = -std=c99 -Wall -g -Os -mmcu=atmega328p

OBJCOPY = avr-objcopy
AVRDUDE = avrdude

AVRDUDE_FLAGS = -p atmega328p -c arduino -U -F -P COM1 COM2 COM3 -B12 -v

SRC = mst.c sl1.c sl2.c

BIN = $(SRC:.c=.bin)
HEX = $(SRC:.c=.hex)

all: $(BIN) $(HEX)

%.bin: %.c
	$(CC) $(CFLAGS) -o $@ $<

%.hex: %.bin
	$(OBJCOPY) -O ihex $< $@

flash-mst: mst.hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) mst.hex

flash-sl1: sl1.hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) sl1.hex

flash-sl2: sl2.hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) sl2.hex

clean:
	rm -f $(BIN) $(HEX)