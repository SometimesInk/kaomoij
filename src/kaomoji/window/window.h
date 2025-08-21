//
// Created by panda on 8/18/2025.
//

#ifndef KAOMOIJ_WINDOW_H
#define KAOMOIJ_WINDOW_H

#include <windows.h>

extern int _get_win_class(WNDCLASSEX *win_class, HINSTANCE hInstance);

extern int _get_win_handle(HWND *win_handle, HINSTANCE hInstance);

extern void _win_loop(MSG *win_msg);

extern int init_win(HINSTANCE hInstance);

#endif //KAOMOIJ_WINDOW_H
