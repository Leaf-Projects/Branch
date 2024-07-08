#include <ui/menu.h>
#include <pub.h>

bool _draw_banner = true;

char *_banner[] = {
    " _____                 _      _____         _   _           _         ",
    "| __  |___ ___ ___ ___| |_   | __  |___ ___| |_| |___ ___ _| |___ ___ ",
    "| __ -|  _| .'|   |  _|   |  | __ -| . | . |  _| | . | .'| . | -_|  _|",
    "|_____|_| |__,|_|_|___|_|_|  |_____|___|___|_| |_|___|__,|___|___|_|  ",
};

int num_entries = 0;
int current_entry = 0;

void _putc(char c)
{
    CHAR16 str[2];
    str[0] = (CHAR16)c;
    str[1] = '\0';

    stdout->OutputString(stdout, str);
}

void _println(char *str)
{
    while (*str != '\0')
    {
        _putc(*str);
        if (*str == '\n' && ++*str != '\r')
        {
            _putc('\r');
        }
        str++;
    }
    _putc('\r');
    _putc('\n');
}

void _display_banner()
{
    stdout->SetAttribute(stdout, EFI_MAGENTA | EFI_BACKGROUND_BLACK);
    for (size_t i = 0; i < sizeof(_banner) / sizeof(*_banner); i++)
    {
        _println(_banner[i]);
    }
    stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
}

void draw_menu()
{
    if (_draw_banner)
    {
        _display_banner();
        _draw_banner = false;
    }

    stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
    stdout->SetCursorPosition(stdout, 0, 5);
    for (int i = 0; i < num_entries; i++)
    {
        if (i == current_entry)
            stdout->SetAttribute(stdout, EFI_WHITE | EFI_BACKGROUND_BLUE);
        else
            stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);

        _println("test");
    }
}