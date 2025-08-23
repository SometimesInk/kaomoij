//
// Created by panda on 8/23/2025.
//

#include "clipboard.h"

#include <stdio.h>

void set_clipboard(const wchar_t *str) {
    (void) printf("INFO @ set_clipboard: Adding '%ls' to clipboard.", str);

    if (!OpenClipboard(NULL)) {
        (void) printf("API ERR @ set_clipboard: Failed to open clipboard. See last error: '%lu'.\n",
            GetLastError());
        return;
    }
    EmptyClipboard();

    size_t len = (wcslen(str) + 1) * sizeof(wchar_t);
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    if (hMem == NULL) {
        (void)printf("MEM ERR @ set_clipboard: Failed to allocate global memory.\n");
        CloseClipboard();
        return;
    }

    LPWSTR lpwstr = (LPWSTR)GlobalLock(hMem);
    wcscpy_s(lpwstr, len / sizeof(wchar_t), str);
    GlobalUnlock(hMem);

    SetClipboardData(CF_UNICODETEXT, hMem);
    CloseClipboard();
}
