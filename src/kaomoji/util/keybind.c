//
// Created by panda on 8/18/2025.
//


#include "keybind.h"

#include <stdio.h>

int string_to_keybind(t_keybind *keybind, wchar_t *str) {
    _t_keybind_section section = ID;

    // Parse sections
    const wchar_t *token = wcstok(str, L"/");
    while (token != NULL) {
        const int value = wcstol(token, NULL, 10);

        switch (section) {
            case ID:
                keybind->id = value;
                break;
            case MOD:
                keybind->modifiers = value;
                break;
            case VK:
                keybind->key = value;
                break;
            default:
                (void) printf("ERR @ string_to_keybind: Invalid section.\n");
                return 0;
        }

        section++;
        token = wcstok(NULL, L"/");
    }
    return 1;
}

int register_keybind(const t_keybind keybind, const HWND win_handle) {
    if (!RegisterHotKey(win_handle, keybind.id, keybind.modifiers, keybind.key)) {
        (void) printf("WIN ERR @ register_keybind: Couldn't register keybind of ID %i. See last error: '%lu'.\n",
                      keybind.id, GetLastError());
        return 0;
    }
    return 1;
}

int unregister_keybind(const t_keybind keybind, const HWND win_handle) {
    if (!UnregisterHotKey(win_handle, keybind.id)) {
        (void) printf("WIN ERR @ register_keybind: Couldn't unregister keybind of ID %i. See last error: '%lu'.\n",
                      keybind.id, GetLastError());
        return 0;
    }
    return 1;
}
