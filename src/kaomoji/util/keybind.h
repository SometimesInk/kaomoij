//
// Created by panda on 8/18/2025.
//

#ifndef KAOMOIJ_KEYBIND_H
#define KAOMOIJ_KEYBIND_H
#include "../window/message.h"

typedef enum _e_keybind_section {
    ID,
    MOD,
    VK
} _t_keybind_section;

typedef struct s_keybind {
    int id;
    UINT modifiers;
    UINT key;
} t_keybind;

extern int string_to_keybind(t_keybind *keybind, wchar_t *str);

extern int register_keybind(t_keybind keybind, HWND win_handle);

extern int unregister_keybind(t_keybind keybind, HWND win_handle);

#endif //KAOMOIJ_KEYBIND_H
