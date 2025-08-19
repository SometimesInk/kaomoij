//
// Created by panda on 8/19/2025.
//

#include "button.h"

int add_button(const HWND win_handle) {
    CreateWindowW(L"BUTTON", L"Beep",
                 WS_VISIBLE | WS_CHILD,
                 20, 50, 80, 25,
                 win_handle, (HMENU) 1, NULL, NULL);

    CreateWindowW(L"BUTTON", L"Quit",
                 WS_VISIBLE | WS_CHILD,
                 120, 50, 80, 25,
                 win_handle, (HMENU) 2, NULL, NULL);
    return 1;
}
