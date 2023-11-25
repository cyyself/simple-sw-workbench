SECTIONS
{
    . = 0x80000000;
    .ro : {
        *(.text)
        *(.rodata)
    }
    .rw : {
        *(.data)
        *(.bss)
        *(COMMON)
    }
    . = ALIGN(8);
    . = . + 0x10000; /* 64kB of stack memory */
    stack_top = .; /* for startup.s */
    heap_low = .; /* for _sbrk */
    . = . + 0x10000; /* 64kB of heap memory */
    heap_top = .; /* for _sbrk */
}
