#include "uart.h"
#include "trap.h"

int main() {
    setup_mtvec();
    print_s("Hello World from T1 UART!");
    return 0;
}
