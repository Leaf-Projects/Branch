#include <stdint.h>

void outb(uint16_t port, uint8_t data)
{
    __asm__ volatile("outb %b0, %w1" : : "a"(data), "Nd"(port));
}

void print_string(const char *str)
{
    int i = 0;
    while (str[i++] != '\0')
        outb(0x0E9, str[i - 1]);
}

void _start(void)
{

    print_string("Kernel Loaded Successfully!\n");

    __asm__ volatile("hlt");
    for (;;)
        ;
}