/*
Branch Bootloader - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_ENTRIES 10

typedef struct menu_entry
{
    char title[256];
    char path[256];
    char protocol[256];
    void *data;
    void (*action)();
} menu_entry;

extern bool _draw_banner;
extern int num_entries;
extern int current_entry;
extern menu_entry entries[MAX_ENTRIES];

void draw_menu();
void add_menu_entry(const char *title, const char *path, const char *protocol, void (*action)());

#endif // MENU_H