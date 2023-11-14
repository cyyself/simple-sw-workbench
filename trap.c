#include "trap.h"
#include "uart.h"

void __attribute__((aligned(4))) trap_handler() {
    unsigned long mcause, mtval, mepc;
    asm volatile(
        "csrr %0, mcause"
        : "=r" (mcause)
    );
    asm volatile(
        "csrr %0, mtval"
        : "=r" (mtval)
    );
    asm volatile(
        "csrr %0, mepc"
        : "=r" (mepc)
    );
    print_s("Exception: ");
    print_s("\nmcause: ");
    dump_hex(mcause);
    print_s("mtval: ");
    dump_hex(mtval);
    print_s("mepc: ");
    dump_hex(mepc);
    while(1);
}

void setup_mtvec() {
    void* ptr = &trap_handler;
    print_s("setting mtvec to ");
    dump_hex((unsigned long)ptr);
    print_s("\n");
    if (((unsigned long)ptr & 0b11)) {
        print_s("Error! mtvec does not aligned to 4B\n");
    }
    asm volatile(
        "csrw mtvec, %0"
        :
        : "r" (ptr)
    );
}