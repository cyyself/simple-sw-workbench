CROSS_COMPILE :=	riscv64-linux-gnu-
CC		:=	$(CROSS_COMPILE)gcc
OBJCOPY	:=	$(CROSS_COMPILE)objcopy
OBJDUMP :=  $(CROSS_COMPILE)objdump

CFLAGS := -nostdlib -static -mcmodel=medany -fno-builtin-printf -O2
.PHONY: clean

all: start.bin dump.S

start.bin: start.elf
	$(OBJCOPY) -O binary $< $@

start.elf: start.S main.c uart.c trap.c mmu.c linker.ld
	$(CC) $(CFLAGS) -T linker.ld start.S main.c uart.c trap.c mmu.c -lgcc -o $@

dump.S: start.elf
	$(OBJDUMP) -D $< > $@

clean:
	rm start.bin start.elf
