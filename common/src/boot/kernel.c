/*
Branch Bootloader - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <boot/kernel.h>
#include <protocol/branch.h>
#include <data/elf.h>
#include <utils/wchar.h>
#include <lib/print.h>
#include <lib/string.h>
#include <lib/alloc.h>
#include <ui/menu.h>
#include <fs/sfs.h>

void load_kernel_callback()
{
    char *kernel_path = entries[current_entry]->path;
    if (strcmp(entries[current_entry]->protocol, "branch") != 0)
    {
        printf("ERROR: Unsupported protocol: %s (Entry: %s, Path: %s)\n", entries[current_entry]->protocol, entries[current_entry]->title, kernel_path);
        for (;;)
            ;
    }
    printf(">>> %s (%s) <<<\n", entries[current_entry]->title, kernel_path);

    CHAR16 *path_wide = malloc(strlen(kernel_path) * sizeof(CHAR16) + 2);
    utf8_char_to_wchar(kernel_path, path_wide);

    SimpleFile kernel = sfs_open(path_wide);
    if (EFI_ERROR(kernel.status))
    {
        printf("ERROR: Failed to open kernel path\n");
        for (;;)
            ;
    }

    char *buffer = malloc(kernel.info.physicalSize);
    if (buffer == NULL)
    {
        printf("ERROR: Failed to allocate memory for kernel data buffer\n");
        for (;;)
            ;
    }

    sfs_read(&kernel, &*buffer);
    if (EFI_ERROR(kernel.status))
    {
        printf("ERROR: Failed to read kernel path\n");
        for (;;)
            ;
        ;
    }

    elf_exec_handle *data = load_elf(buffer);
    if (data == NULL)
    {
        printf("ERROR: Failed to load kernel (tried loading as ELF)\n");
        sfs_close(&kernel);
        free(buffer);
        free(path_wide);
        for (;;)
            ;
    }

    sfs_close(&kernel);
    free(buffer);
    free(path_wide);

    EFI_STATUS status;

    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    EFI_UINTN SizeOfInfo, numModes, nativeMode;

    status = systemTable->BootServices->LocateProtocol(&gopGuid, NULL, (void **)&gop);
    if (EFI_ERROR(status))
    {
        printf("ERROR: Failed to locate GOP protocol\n");
        free(data);
        for (;;)
            ;
    }

    status = gop->QueryMode(gop, gop->Mode == NULL ? 0 : gop->Mode->Mode, &SizeOfInfo, &info);
    if (status == (EFI_UINTN)EFI_NOT_STARTED)
    {
        status = gop->SetMode(gop, 0);
    }

    if (EFI_ERROR(status))
    {
        printf("ERROR: Unable to get native mode\n");
        free(data);
        for (;;)
            ;
    }
    else
    {

        nativeMode = gop->Mode->Mode;
        numModes = gop->Mode->MaxMode;
    }

    status = gop->QueryMode(gop, nativeMode, &SizeOfInfo, &info);
    if (EFI_ERROR(status))
    {
        printf("ERROR: Unable to get native mode info\n");
        free(data);
        for (;;)
            ;
    }

    status = gop->SetMode(gop, nativeMode);
    if (EFI_ERROR(status))
    {
        printf("ERROR: Unable to set mode\n");
        free(data);
        for (;;)
            ;
    }

    framebuffer_t framebuffer = (framebuffer_t){0};
    switch (gop->Mode->Info->PixelFormat)
    {
    case PixelBlueGreenRedReserved8BitPerColor:
        framebuffer.bpp = 32;
        framebuffer.red_mask_size = 8;
        framebuffer.red_mask_shift = 16;
        framebuffer.green_mask_size = 8;
        framebuffer.green_mask_shift = 8;
        framebuffer.blue_mask_size = 8;
        framebuffer.blue_mask_shift = 0;
        break;
    case PixelRedGreenBlueReserved8BitPerColor:
        framebuffer.bpp = 32;
        framebuffer.red_mask_size = 8;
        framebuffer.red_mask_shift = 0;
        framebuffer.green_mask_size = 8;
        framebuffer.green_mask_shift = 8;
        framebuffer.blue_mask_size = 8;
        framebuffer.blue_mask_shift = 16;
        break;
    default:
        break;
    }

    framebuffer.address = gop->Mode->FrameBufferBase;
    framebuffer.width = gop->Mode->Info->HorizontalResolution;
    framebuffer.height = gop->Mode->Info->VerticalResolution;
    framebuffer.pitch = gop->Mode->Info->PixelsPerScanLine * (framebuffer.bpp / 8);

    stdout->ClearScreen(stdout);

    // TODO: Setup the env for the kernel and pass shit based on protocol
    systemTable->BootServices->ExitBootServices(imageHandle, 0);
    void (*entry)(framebuffer_t *) = (void (*)(framebuffer_t *))(uintptr_t)data->entry_point;
    entry(&framebuffer);
}