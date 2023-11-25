#include "uart.h"
#include <stdio.h>
#include <sys/stat.h>


char *heap_ptr;

char *_sbrk(int nbytes) {
    extern char* heap_top;
    char *res;
    if (!heap_ptr) heap_ptr = (char *)&heap_top;
    res = heap_ptr;
    heap_ptr += nbytes;
    return res;
}

int _isatty(int file) {
    print_s("Error: _isatty\n");
    return -1;
}

int _kill(int pid, int sig) {
    print_s("Error: _kill\n");
    return -1;
}

int _getpid() {
    print_s("Error: _getpid\n");
    return -1;
}

int _fstat(int file, struct stat* st) {
    if (file == 1) return 0;
    print_s("Error: _fstat\n");
    return -1;
}

int _close(int file) {
    print_s("Error: _close\n");
    return -1;
}

int _lseek(int file, int ptr, int dir) {
    print_s("Error: _lseek\n");
    return -1;
}

int _read(int file, char* ptr, int len) {
    if (file == 0) {
        for (int i = 0; i < len; i++) {
            ptr[i] = uart_get_c();
        }
        return len;
    }
    print_s("Error: _read\n");
    return -1;
}

int _write(int file, char* ptr, int len) {
    if (file == 1) {
        for (int i = 0; i < len; i++) {
            uart_put_c(ptr[i]);
        }
        return len;
    }
    print_s("Error: _write\n");
    return -1;
}