//
// Created by panda on 8/19/2025.
//

#ifndef KAOMOIJ_BUTTON_H
#define KAOMOIJ_BUTTON_H
#include "../io/config.h"

extern HWND *d_buttons;
extern size_t buttons_len;

extern HFONT font;

extern int init_font(void);

extern int add_button(HWND win_handle);

extern void on_command_msg(HWND win_handle, UINT win_msg, WPARAM w_param, LPARAM l_param);

extern void _on_click(HWND sender, size_t i);

extern void dispose_buttons(void);

extern void dispose_fonts(void);

#endif //KAOMOIJ_BUTTON_H