CROSS_COMPILE :=	riscv32-unknown-elf-
CC		:=	$(CROSS_COMPILE)gcc
OBJCOPY	:=	$(CROSS_COMPILE)objcopy
OBJDUMP :=  $(CROSS_COMPILE)objdump
EXTRA_CFLAGS := 
CFLAGS := -march=rv32gcv -nostdlib -static -mcmodel=medlow -fno-builtin-printf -O2 $(EXTRA_CFLAGS)
.PHONY: clean

all: start.bin dump.S

start.bin: start.elf
	$(OBJCOPY) -O binary $< $@

start.elf: linker.ld start.S main.c uart.c trap.c
	$(CC) $(CFLAGS) -T $^ -lgcc -o $@

dump.S: start.elf
	$(OBJDUMP) -D $< > $@

clean:
	rm start.bin start.elf
