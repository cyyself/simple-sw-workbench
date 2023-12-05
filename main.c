#include "uart.h"
#include "trap.h"
#include "mmu.h"

char s0[64] = "Hello World from UART!\r\n";
char s1[64];

void *v_memcpy(void* dest, const void* src, long n);

unsigned long rdcycle() {
    unsigned long cycle;
    asm volatile ("rdcycle %0" : "=r" (cycle));
    return cycle;
}

unsigned long rdtime() {
    unsigned long time;
    asm volatile ("rdtime %0" : "=r" (time));
    return time;
}

void check_freq() {
    const long timer_base_freq = 27000000;
    unsigned long start_cycle = rdcycle();
    unsigned long start_time = rdtime();
    unsigned long end_time = start_time;
    while (end_time - start_time < timer_base_freq) {
        end_time = rdtime();
    }
    unsigned long end_cycle = rdcycle();
    print_s("cpu freq = ");
    print_long(end_cycle - start_cycle);
    print_s("Hz\r\n");
}

int main() {
    setup_mtvec();
    // mmu_init();
    // enter_smode();
    double x = 1.0;
    dump_hex(*(unsigned long*)(&x));
    v_memcpy(s1, s0, 64);
    print_s(s1);
    check_freq();
    return 0;
}
