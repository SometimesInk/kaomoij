//
// Created by panda on 8/18/2025.
//

#ifndef KAOMOIJ_KEYBIND_H
#define KAOMOIJ_KEYBIND_H
#include "../window/message.h"

typedef struct s_keybind {
    int id;
    UINT modifiers;
    UINT key;
} t_keybind;

extern int register_keybind(t_keybind keybind, HWND win_handle);

extern int unregister_keybind(t_keybind keybind, HWND win_handle);

#endif //KAOMOIJ_KEYBIND_H
