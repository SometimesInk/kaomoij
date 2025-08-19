//
// Created by panda on 8/18/2025.
//

#include "../message.h"

#include <stdio.h>

#include "config.h"

int appear_window(HWND hwnd) {
    // Toggle window visibility
    if (IsWindowVisible(hwnd)) {
        ShowWindow(hwnd, SW_HIDE);
    } else {
        ShowWindow(hwnd, SW_SHOW);
        SetForegroundWindow(hwnd);
    }

    // Move window to cursor position
    POINT cursor_pos;
    if (!GetCursorPos(&cursor_pos)) {
        printf("ERR @ appear_window: Failed to get cursor position.");
        return 0;
    }

    switch (conf.corner) {
        case TOP_LEFT:
            MoveWindow(hwnd, cursor_pos.x, cursor_pos.y, conf.width, conf.height, FALSE);
            break;
        case TOP_RIGHT:
            MoveWindow(hwnd, cursor_pos.x - conf.width, cursor_pos.y, conf.width, conf.height, FALSE);
            break;
        case BOT_LEFT:
            MoveWindow(hwnd, cursor_pos.x, cursor_pos.y - conf.height, conf.width, conf.height, FALSE);
            break;
        case BOT_RIGHT:
            MoveWindow(hwnd, cursor_pos.x - conf.width, cursor_pos.y - conf.height,
                       conf.width, conf.height, FALSE);
            break;
        default:
            printf("ERR @ appear_window: No correct corner defined.");
            return 0;
    }
    return 1;
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_HOTKEY: {
            if (wParam != conf.launch.id) return 0;

            if (!appear_window(hwnd)) {
                printf("ERR @ WndProc: An error occurred whilst toggling window visibility.");
            }
        }
        break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
