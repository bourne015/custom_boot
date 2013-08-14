objs = start.o clock.o uart.o ddr.o nand.o boot.o lcd.o \
	interrupts.o touch.o test.o
CC := arm-linux-gcc
CFLAGS := -fno-builtin -Wall -nostdlib

boot.bin:$(objs)
	arm-linux-ld -Tboot.lds -o boot.elf $^
	arm-linux-objcopy -O binary -S boot.elf $@
	arm-linux-objdump -D boot.elf > boot.dis

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f *.o *.bin *.elf *.dis
