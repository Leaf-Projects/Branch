/*
Branch Bootloader - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <ui/menu.h>
#include <pub.h>
#include <lib/print.h>

bool _draw_banner = true;

char *_banner[] = {
    " _____                 _      _____         _   _           _         ",
    "| __  |___ ___ ___ ___| |_   | __  |___ ___| |_| |___ ___ _| |___ ___ ",
    "| __ -|  _| .'|   |  _|   |  | __ -| . | . |  _| | . | .'| . | -_|  _|",
    "|_____|_| |__,|_|_|___|_|_|  |_____|___|___|_| |_|___|__,|___|___|_|  ",
};

int num_entries = 0;
int current_entry = 0;
menu_entry entries[MAX_ENTRIES];

void _display_banner()
{
    stdout->SetAttribute(stdout, EFI_MAGENTA | EFI_BACKGROUND_BLACK);
    for (size_t i = 0; i < sizeof(_banner) / sizeof(*_banner); i++)
    {
        printf("%s\n", _banner[i]);
    }
    stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
}

void draw_menu()
{
    if (_draw_banner)
    {
        stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
        stdout->SetCursorPosition(stdout, 0, 0);
        stdout->ClearScreen(stdout);

        stdout->SetCursorPosition(stdout, 0, 0);

        _display_banner();
        _draw_banner = false;
    }

    stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
    stdout->SetCursorPosition(stdout, 0, 5);
    for (int i = 0; i < num_entries; i++)
    {
        if (i == current_entry)
        {
            stdout->SetAttribute(stdout, EFI_WHITE | EFI_BACKGROUND_BLUE);
            printf("-> %s\n", entries[i].title);
        }
        else
        {
            stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
            printf("   %s\n", entries[i].title);
        }
    }
}

void add_menu_entry(const char *title, void (*action)(), void *data)
{
    if (num_entries < MAX_ENTRIES)
    {
        int i = 0;
        while (title[i] != '\0' && i < 256)
        {
            entries[num_entries].title[i] = title[i];
            i++;
        }

        entries[num_entries].action = action;
        entries[num_entries].data = data;
        num_entries++;
    }
}