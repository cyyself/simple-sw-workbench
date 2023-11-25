#include "uart.h"
#include "trap.h"
#include "mmu.h"
#include <stdio.h>

int main() {
    setup_mtvec();
    mmu_init();
    enter_smode();
    print_s("\n\n");
    printf("Hello World from UART!\n");
    return 0;
}
