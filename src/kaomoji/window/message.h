//
// Created by panda on 8/18/2025.
//

#ifndef KAOMOIJ_MESSAGE_H
#define KAOMOIJ_MESSAGE_H
#include <windows.h>

extern int toggle_win(HWND win_handle);

extern LRESULT CALLBACK win_proc(HWND win_handle, UINT win_msg, WPARAM w_param, LPARAM l_param);

#endif //KAOMOIJ_MESSAGE_H
