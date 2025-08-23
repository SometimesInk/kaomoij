//
// Created by panda on 8/23/2025.
//

#include "toggle.h"
#include "../io/config.h"

#include <stdio.h>
#include <windows.h>

int toggle_win(const HWND win_handle) {
    // Toggle window visibility
    if (IsWindowVisible(win_handle)) {
        toggle_win_off(win_handle);
        return 1;
    }
    return toggle_win_on(win_handle);
}

void toggle_win_off(const HWND win_handle) {
        (void) ShowWindow(win_handle, SW_HIDE);
}

int toggle_win_on(const HWND win_handle) {
    if (!IsWindowVisible(win_handle)) (void) ShowWindow(win_handle, SW_SHOW);
    SetForegroundWindow(win_handle);

    // Move window to cursor position
    POINT cursor_pos;
    if (!GetCursorPos(&cursor_pos)) {
        (void) printf("WIN ERR @ appear_window: Failed to get cursor position.\n");
        return 0;
    }

    LONG x = cursor_pos.x;
    LONG y = cursor_pos.y;
    switch (conf.corner) {
        case TOP_LEFT:
            break;
        case TOP_RIGHT:
            x -= conf.width;
            break;
        case BOT_LEFT:
            y -= conf.height;
            break;
        case BOT_RIGHT:
            x -= conf.width;
            y -= conf.height;
            break;
        default:
            (void) printf("ERR @ appear_window: No correct corner defined.\n");
            return 0;
    }

    if (!MoveWindow(win_handle, x, y, conf.width, conf.height, FALSE)) {
        (void) printf("WIN ERR @ toggle_win: Failed to move window. See last error: '%lu'.\n", GetLastError());
        return 0;
    }
    return 1;
}


