objs = start.o clock.o uart.o ddr.o nand.o boot.o lcd.o \
	interrupts.o touch.o test.o pwm.o dm9000.o timer.o \
	lib/libc.a
CC := arm-linux-gcc
AR := arm-linux-ar

INCLUDEDIR := $(shell pwd)/include
CFLAGS := -fno-builtin -Wall
CPPFLAGS := -nostdlib -I$(INCLUDEDIR)
export CC CFLAGS CPPFLAGS INCLUDEDIR

boot.bin:$(objs)
	arm-linux-ld -Tboot.lds -o boot.elf $^
	arm-linux-objcopy -O binary -S boot.elf $@
	arm-linux-objdump -D boot.elf > boot.dis

.PHONY: lib/libc.a
lib/libc.a:
	cd lib; make; cd ../ 
%.o: %.c %.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
%.o: %.S
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:	
	make clean -C lib
	-rm -f *.o *.bin *.elf *.dis
