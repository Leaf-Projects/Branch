#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_ENTRIES 10

typedef struct menu_entry
{
    char title[256];
    void *data;
    void (*action)();
} menu_entry;

extern bool _draw_banner;
extern int num_entries;
extern int current_entry;
extern menu_entry entries[MAX_ENTRIES];

void draw_menu();
void add_menu_entry(const char *title, void (*action)(), void *data);

#endif // MENU_H