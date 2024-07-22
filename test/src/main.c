#include <branch.h>
#include "printf.h"

framebuffer_t *framebuffer;

void hlt()
{
    __asm__ volatile("hlt");
    for (;;)
        ;
}

void put_pixel_rgb(uint64_t x, uint64_t y, uint8_t r, uint8_t g, uint8_t b)
{
    *(uint32_t *)(framebuffer->address + x * (framebuffer->bpp >> 3) +
                  y * framebuffer->pitch) =
        (0xFF << 24) | (r << 16) | (g << 8) | b;
}

void rect(int x, int y, int w, int h)
{
    for (int xi = 0; x < w; x++)
    {
        for (int yi = 0; y < h; y++)
        {
            put_pixel_rgb(x + xi, y + yi, 255, 255, 255);
        }
    }
}

void _start(framebuffer_t *fb)
{
    if (fb == NULL)
    {
        printf("Failed to get framebuffer!\n");
        hlt();
    }

    framebuffer = &*fb;

    printf("Hello, World! (%dx%d)\n", framebuffer->width, framebuffer->height);
    rect(0, 0, 100, 100);
    hlt();
}