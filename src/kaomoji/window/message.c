//
// Created by panda on 8/18/2025.
//

#include <stdio.h>

#include "button.h"
#include "../io/config.h"

int toggle_win(const HWND win_handle) {
    // Toggle window visibility
    if (IsWindowVisible(win_handle)) {
        (void) ShowWindow(win_handle, SW_HIDE);
    } else {
        (void) ShowWindow(win_handle, SW_SHOW);
        SetForegroundWindow(win_handle);
    }

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

    if (MoveWindow(win_handle, x, y, conf.width, conf.height, FALSE)) {
        (void) printf("WIN ERR @ toggle_win: Failed to move window. See last error: '%lu'.\n", GetLastError());
    }
    return 1;
}

LRESULT CALLBACK win_proc(const HWND win_handle, const UINT win_msg, const WPARAM w_param, const LPARAM l_param) {
    (void) printf("MSG: win_msg=%i; w_param=%llu; l_param=%lld\n", win_msg, w_param, l_param);
    switch (win_msg) {
        case WM_CREATE:
            // Add buttons
            if (!add_button(win_handle)) {
                (void) printf("ERR @ init_win: Failed add buttons.\n");
                return 0;
            }
            break;
        case WM_HOTKEY: {
            if (w_param != conf.launch.id) return 0;

            if (!toggle_win(win_handle)) {
                (void) printf("ERR @ WndProc: An error occurred whilst toggling window visibility.\n");
            }
        }
        break;
        case WM_CLOSE:
            DestroyWindow(win_handle);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(win_handle, win_msg, w_param, l_param);
    }
    return 0;
}
