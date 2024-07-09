/*
Branch Bootloader - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <boot/cfg.h>
#include <pub.h>

#include <stdint.h>
#include <stddef.h>
#include <lib/string.h>
#include <lib/ctype.h>
#include <lib/print.h>
#include <lib/alloc.h>

#define MAX_ENTRIES 10

typedef struct
{
    char name[50];
    char kernel[50];
    char protocol[20];
} BootEntry;

BootEntry boot_entries[MAX_ENTRIES];
int timeout = -1;

void parse_config(const char *raw)
{
    const char *delim = "{}\n,;\" \t\r";
    char *token;
    int entry_count = 0;
    int in_entry = 0;

    token = strtok((char *)raw, delim);

    while (token != NULL)
    {
        if (strcmp(token, "timeout:") == 0)
        {
            token = strtok(NULL, delim);
            if (token != NULL)
            {
                timeout = atoi(token);
            }
        }
        else if (strcmp(token, "\"") == 0)
        {
            token = strtok(NULL, "\"");
            if (token != NULL && entry_count < MAX_ENTRIES)
            {
                strcpy(boot_entries[entry_count].name, token);

                token = strtok(NULL, delim);
                if (token != NULL && strcmp(token, "{") == 0)
                {
                    in_entry = 1;
                }
                else
                {
                    printf("Error parsing entry %d\n", entry_count + 1);
                    break;
                }
            }
        }
        else if (in_entry)
        {
            if (strcmp(token, "kernel:") == 0)
            {
                token = strtok(NULL, "\"");
                if (token != NULL)
                {
                    strcpy(boot_entries[entry_count].kernel, token);
                }
            }
            else if (strcmp(token, "protocol:") == 0)
            {
                token = strtok(NULL, "\"");
                if (token != NULL)
                {
                    strcpy(boot_entries[entry_count].protocol, token);
                    entry_count++;
                    in_entry = 0;
                }
            }
            else if (strcmp(token, "}") == 0)
            {
                in_entry = 0;
            }
        }

        token = strtok(NULL, delim);
    }

    if (timeout != -1)
    {
        printf("Timeout: %d\n", timeout);
    }
    else
    {
        printf("Timeout not specified in config.\n");
    }

    printf("Boot Entries:\n");
    for (int i = 0; i < entry_count; ++i)
    {
        printf("Name: %s, Kernel: %s, Protocol: %s\n",
               boot_entries[i].name, boot_entries[i].kernel, boot_entries[i].protocol);
    }
}
