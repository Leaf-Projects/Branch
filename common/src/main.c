/*
Branch Bootloader - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <pub.h>
#include <ui/menu.h>
#include <fs/sfs.h>
#include <boot/cfg.h>
#include <lib/print.h>
#include <lib/alloc.h>

EFI_HANDLE *imageHandle;
EFI_SYSTEM_TABLE *systemTable;

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *stdout;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stdin;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *stderr;

EFI_STATUS branch_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    imageHandle = &ImageHandle;
    systemTable = &*SystemTable;
    stdout = systemTable->ConOut;
    stdin = systemTable->ConIn;
    stderr = systemTable->StdErr;

    stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
    stdout->SetCursorPosition(stdout, 0, 0);
    stdout->ClearScreen(stdout);

    SimpleFile cfg = sfs_open(u"boot.cfg");
    if (EFI_ERROR(cfg.status))
    {
        printf("ERROR: Failed to open boot.cfg\n");
        return cfg.status;
    }

    char *buffer = malloc(cfg.info.physicalSize);
    if (buffer == NULL)
    {
        printf("ERROR: Failed to allocate memory for boot.cfg buffer\n");
        return cfg.status;
    }

    sfs_read(&cfg, &*buffer);
    if (EFI_ERROR(cfg.status))
    {
        printf("ERROR: Failed to read boot.cfg\n");
        return cfg.status;
    }
    buffer[cfg.info.physicalSize] = '\0';

    parse_config(buffer);
    sfs_close(&cfg);
    free(buffer);

    EFI_INPUT_KEY key;
    EFI_UINTN key_event = 0;

    for (;;)
    {
        draw_menu();

        SystemTable->BootServices->WaitForEvent(1, &(SystemTable->ConIn->WaitForKey), &key_event);
        stdin->ReadKeyStroke(stdin, &key);

        switch (key.ScanCode)
        {
        case EFI_SCANCODE_ARROW_DOWN:
            current_entry = (current_entry + 1) % num_entries;
            continue;
        case EFI_SCANCODE_ARROW_UP:
            current_entry = (current_entry + num_entries - 1) % num_entries;
            continue;
        default:
            switch (key.UnicodeChar)
            {
            case '\r':
                if (entries[current_entry].action != NULL)
                {
                    stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
                    stdout->SetCursorPosition(stdout, 0, 0);
                    stdout->ClearScreen(stdout);

                    entries[current_entry].action();

                    printf("INFO: Press Enter to continue...\n");

                    do
                    {
                        SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key);
                    } while (key.UnicodeChar != L'\r');

                    stdout->ClearScreen(stdout);
                    _draw_banner = true;
                    draw_menu();
                    continue;
                }

                break;
            }
        }
    }

    for (;;)
        ;
}