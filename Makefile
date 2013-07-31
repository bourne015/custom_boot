objs = start.o ddr.o nand.o boot.o lowlevel_init.o io.o
CC := arm-linux-gcc
CFLAGS := -fno-builtin -Wall -nostdlib

boot.bin:$(objs)
	arm-linux-ld -Tboot.lds -o boot.elf $^
	arm-linux-objcopy -O binary -S boot.elf $@
#	$(CC)objdump -D boot.elf > boot.dis

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: %.S %.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f *.o *.bin *.elf *.dis
