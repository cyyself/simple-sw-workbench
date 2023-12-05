#include "uart.h"
#include "trap.h"
#include "mmu.h"

int main() {
    setup_mtvec();
    // mmu_init();
    // enter_smode();
    print_s("Hello World from UART!");
    return 0;
}
