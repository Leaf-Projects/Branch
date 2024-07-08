#define _X64
#include <efi.h>

EFI_STATUS branch_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello, World!\r\n");
    for (;;)
        ;
}