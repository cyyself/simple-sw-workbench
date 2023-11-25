CROSS :=	riscv64-none-elf
AS		:=	$(CROSS)-as
CC		:=	$(CROSS)-gcc
OBJCOPY	:=	$(CROSS)-objcopy
OBJDUMP :=  $(CROSS)-objdump
EXTRA_CFLAGS := 
CFLAGS := -nostdlib -static -mcmodel=medany -O2 $(EXTRA_CFLAGS)
.PHONY: clean

all: start.bin dump.S

deps/newlib/$(CROSS)/newlib/libc.a: deps/newlib
	cd deps/newlib && ./configure --target $(CROSS) --disable-newlib-supplied-syscalls CFLAGS_FOR_TARGET="-g -O2 -mcmodel=medany" && make -j `nproc`

start.bin: start.elf
	$(OBJCOPY) -O binary $< $@

start.elf: linker.ld start.S main.c uart.c trap.c mmu.c sys.c deps/newlib/$(CROSS)/newlib/libc.a
	$(CC) $(CFLAGS) -T $^ -I deps/newlib/include -lgcc -o $@

dump.S: start.elf
	$(OBJDUMP) -D $< > $@

clean:
	rm start.bin start.elf
