//
// Created by panda on 8/18/2025.
//

#include "../keybind.h"

#include <stdio.h>

#include "../message.h"

int register_keybind(t_keybind keybind, HWND win_handle) {
    if (!RegisterHotKey(win_handle, keybind.id, keybind.modifiers, keybind.key)) {
        printf("WIN32 ERR @ register_keybind: Failed to register keybind with ID %i.", keybind.id);
        return 0;
    }
    return 1;
}

int unregister_keybind(t_keybind keybind, HWND win_handle) {
    if (!UnregisterHotKey(win_handle, keybind.id)) {
        printf("WIN32 ERR @ unregister_keybind: Failed to unregister keybind with ID %i.", keybind.id);
        return 0;
    }
    return 1;
}
