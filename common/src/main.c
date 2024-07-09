/*
Branch Bootloader - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <pub.h>
#include <ui/menu.h>
#include <fs/sfs.h>
#include <lib/print.h>

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

    // Add some test entries
    add_menu_entry("Test Entry 1", NULL, NULL);
    add_menu_entry("Test Entry 2", NULL, NULL);
    add_menu_entry("Test Entry 3", NULL, NULL);

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
                    entries[current_entry].action();

                break;
            }
        }
    }

    for (;;)
        ;
}