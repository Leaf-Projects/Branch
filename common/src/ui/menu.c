/*
Branch Bootloader - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <ui/menu.h>
#include <common.h>
#include <lib/print.h>
#include <lib/string.h>
#include <lib/alloc.h>

bool _draw_banner = true;
int num_entries = 0;
int current_entry = 0;
menu_entry **entries = NULL;

char *_banner[] = {
    " _____                 _      _____         _   _           _         ",
    "| __  |___ ___ ___ ___| |_   | __  |___ ___| |_| |___ ___ _| |___ ___ ",
    "| __ -|  _| .'|   |  _|   |  | __ -| . | . |  _| | . | .'| . | -_|  _|",
    "|_____|_| |__,|_|_|___|_|_|  |_____|___|___|_| |_|___|__,|___|___|_|  ",
};

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
            printf("-> %s\n", entries[i]->title);
        }
        else
        {
            stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
            printf("   %s\n", entries[i]->title);
        }

        stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
    }
}

void add_menu_entry(const char *title, const char *path, const char *protocol, void (*action)())
{
    menu_entry *entry = malloc(sizeof(menu_entry));
    if (!entry)
    {
        printf("ERROR: Failed to allocate memory for menu entry\n");
        return;
    }

    entry->title = malloc(strlen(title) + 1);
    if (!entry->title)
    {
        free(entry);
        printf("ERROR: Failed to allocate memory for menu entry title\n");
        return;
    }
    strcpy(entry->title, title);

    entry->path = malloc(strlen(path) + 1);
    if (!entry->path)
    {
        free(entry->title);
        free(entry);
        printf("ERROR: Failed to allocate memory for menu entry path\n");
        return;
    }
    strcpy(entry->path, path);

    entry->protocol = malloc(strlen(protocol) + 1);
    if (!entry->protocol)
    {
        free(entry->path);
        free(entry->title);
        free(entry);
        printf("ERROR: Failed to allocate memory for menu entry protocol\n");
        return;
    }
    strcpy(entry->protocol, protocol);

    entry->action = action;

    menu_entry **new_entries = realloc(entries, sizeof(menu_entry *) * (num_entries + 1));
    if (!new_entries)
    {
        free(entry->protocol);
        free(entry->path);
        free(entry->title);
        free(entry);
        printf("ERROR: Failed to reallocate memory for menu entries array\n");
        return;
    }

    entries = new_entries;
    entries[num_entries] = entry;
    num_entries++;
}

void free_menu()
{
    for (int i = 0; i < num_entries; i++)
    {
        free(entries[i]->title);
        free(entries[i]->path);
        free(entries[i]->protocol);
        free(entries[i]);
    }
    free(entries);
}
