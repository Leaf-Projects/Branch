/*
Branch Bootloader - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <boot/kernel.h>
#include <lib/print.h>
#include <lib/string.h>
#include <ui/menu.h>

void load_kernel_callback()
{
    char *kernel_path = entries[current_entry].path;
    if (strcmp(entries[current_entry].protocol, "branch") != 0)
    {
        printf("ERROR: Unsupported protocol: %s\n", entries[current_entry].protocol);
        return;
    }
    printf(">>> %s <<<\n", entries[current_entry].title);
}