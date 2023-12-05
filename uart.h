#ifndef UART_H
#define UART_H

void uart_init();
void uart_put_c(char c);
char uart_check_read();
char uart_get_c();
void print_s(const char *c);
void print_long(long x);
void print_digit(unsigned char x);
void dump_hex(unsigned long x);

#endif