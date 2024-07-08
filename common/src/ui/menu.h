#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <stddef.h>

extern bool _draw_banner;
extern int num_entries;
extern int current_entry;

void draw_menu();

#endif // MENU_H