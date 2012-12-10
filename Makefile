objs = start.o clock.o uart.o ddr.o nand.o test.o boot.o
CC := arm-linux-gcc
CFLAGS := -fno-builtin -Wall

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
