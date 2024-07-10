/*
Branch Bootloader - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <lib/alloc.h>
#include <lib/print.h>

void *malloc(size_t size)
{
    void *ptr;
    if (EFI_ERROR(systemTable->BootServices->AllocatePool(EfiLoaderData, size, &ptr)))
    {
        printf("ERROR: Failed to allocate memory\n");
        return NULL;
    }

    return ptr;
}

void free(void *ptr) { systemTable->BootServices->FreePool(ptr); }