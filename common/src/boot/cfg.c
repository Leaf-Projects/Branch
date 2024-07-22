/*
Branch Bootloader - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <boot/cfg.h>
#include <boot/kernel.h>
#include <common.h>

#include <stdint.h>
#include <stddef.h>
#include <lib/string.h>
#include <lib/ctype.h>
#include <lib/print.h>
#include <lib/alloc.h>
#include <ui/menu.h>

void _parse_entry(char *line, char **name, char **path, char **protocol)
{
    char *startQuote = strchr(line, '"');
    if (startQuote == NULL)
        return;

    char *endQuote = strchr(startQuote + 1, '"');
    if (endQuote == NULL)
        return;

    size_t nameLength = endQuote - startQuote - 1;

    *name = malloc(nameLength + 1);
    if (*name == NULL)
        return;

    strncpy(*name, startQuote + 1, nameLength);
    (*name)[nameLength] = '\0';

    char *kernelKeyword = strstr(line, "kernel");
    if (kernelKeyword == NULL)
    {
        free(*name);
        *name = NULL;
        return;
    }

    startQuote = strchr(kernelKeyword, '"');
    if (startQuote == NULL)
    {
        free(*name);
        *name = NULL;
        return;
    }

    endQuote = strchr(startQuote + 1, '"');
    if (endQuote == NULL)
    {
        free(*name);
        *name = NULL;
        return;
    }

    size_t pathLength = endQuote - startQuote - 1;

    *path = malloc(pathLength + 1);
    if (*path == NULL)
    {
        free(*name);
        *name = NULL;
        return;
    }

    strncpy(*path, startQuote + 1, pathLength);
    (*path)[pathLength] = '\0';

    char *protocolKeyword = strstr(line, "protocol");
    if (protocolKeyword == NULL)
    {
        *protocol = NULL;
        return;
    }

    startQuote = strchr(protocolKeyword, '"');
    if (startQuote == NULL)
    {
        *protocol = NULL;
        return;
    }

    endQuote = strchr(startQuote + 1, '"');
    if (endQuote == NULL)
    {
        *protocol = NULL;
        return;
    }

    size_t protocolLength = endQuote - startQuote - 1;

    *protocol = malloc(protocolLength + 1);
    if (*protocol == NULL)
    {
        free(*name);
        *name = NULL;
        free(*path);
        *path = NULL;
        return;
    }

    strncpy(*protocol, startQuote + 1, protocolLength);
    (*protocol)[protocolLength] = '\0';
}

void parse_config(const char *raw)
{
    char *line = (char *)raw;
    while (*line != '\0')
    {
        if (strncmp(line, "entry", 5) == 0)
        {
            char *name = NULL;
            char *path = NULL;
            char *protocol = NULL;
            _parse_entry(line, &name, &path, &protocol);

            if (name != NULL && path != NULL && protocol != NULL)
            {
                add_menu_entry(name, path, protocol, &load_kernel_callback);
            }

            free(name);
            free(path);
            free(protocol);
        }

        while (*line != '\n' && *line != '\0')
            line++;
        if (*line == '\n')
            line++;
    }
}