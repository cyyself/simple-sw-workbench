#include "uart.h"
#include "trap.h"

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

int main() {
    setup_mtvec();
    print_s(s0);
    v_memcpy(s1, s0, 64);
    print_s(s1);
    return 0;
}
