//
// Created by panda on 8/18/2025.
//

#include "keybind.h"
#include "../io/config.h"

#include <stdio.h>

int string_to_keybind(t_keybind *keybind, wchar_t *str) {
    _t_keybind_section section = MOD;

    // Parse sections
    const wchar_t *token = wcstok(str, L"/");
    while (token != NULL) {
        const int value = wcstol(token, NULL, 10);

        switch (section) {
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

void init_keybinds(HWND win_handle) {
    if (!register_keybind(conf.launch, win_handle)) {
        (void)printf("ERR @ init_win: Failed to register launch keybind.\n");
    }
    if (!register_keybind(conf.exit, win_handle)) {
        (void)printf("ERR @ init_win: Failed to register exit keybind.\n");
    }
}

void dispose_keybinds(HWND win_handle) {
    if (unregister_keybind(conf.launch, win_handle)) {
        (void) printf("DCON ERR @ init_win: Failed to unregister launch keybind.\n");
    }
    if (unregister_keybind(conf.exit, win_handle)) {
        (void) printf("DCON ERR @ init_win: Failed to unregister exit keybind.\n");
    }
}
