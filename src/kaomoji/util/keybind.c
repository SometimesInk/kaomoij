//
// Created by panda on 8/18/2025.
//


#include "keybind.h"

#include <stdio.h>


int register_keybind(const t_keybind keybind, const HWND win_handle) {
    if (!RegisterHotKey(win_handle, keybind.id, keybind.modifiers, keybind.key)) {
        (void) printf("WIN ERR @ register_keybind: Failed to register keybind with ID %i.\n", keybind.id);
        return 0;
    }
    return 1;
}

int unregister_keybind(const t_keybind keybind, const HWND win_handle) {
    if (!UnregisterHotKey(win_handle, keybind.id)) {
        (void) printf("WIN ERR @ unregister_keybind: Failed to unregister keybind with ID %i.\n", keybind.id);
        return 0;
    }
    return 1;
}
