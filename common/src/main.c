#include <pub.h>
#include <ui/menu.h>

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

    add_menu_entry("Test Entry", NULL, NULL);
    add_menu_entry("Test Entry", NULL, NULL);
    add_menu_entry("Test Entry", NULL, NULL);
    add_menu_entry("Test Entry", NULL, NULL);
    add_menu_entry("Test Entry", NULL, NULL);

    EFI_INPUT_KEY key;
    uint64_t key_event = 0;

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