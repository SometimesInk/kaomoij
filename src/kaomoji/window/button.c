//
// Created by panda on 8/19/2025.
//

#include <math.h>

#include "button.h"

#include <stdio.h>

#include "../io/entry.h"

int add_button(const HWND win_handle) {
    const int width = conf.width / conf.cols;
    const int height = conf.height / conf.rows;
    for (int i = 0; i < entries_len; ++i) {
        if (!CreateWindowW(L"BUTTON", d_entries[i],
                     WS_VISIBLE | WS_CHILD,
                     (i % conf.cols) * width,
                     floor(i / conf.cols) * height,
                     width, height,
                     win_handle, (HMENU) i, NULL, NULL)) {
            (void) printf("WIN ERR @ add_button: Failed to add button @ i=%i. See last error: '%lu'.\n",
                i, GetLastError());
        }
    }
    return 1;
}
