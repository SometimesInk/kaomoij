//
// Created by panda on 8/18/2025.
//

#include <stdio.h>

#include "button.h"
#include "toggle.h"
#include "window.h"
#include "../io/config.h"

LRESULT CALLBACK win_proc(const HWND win_handle, const UINT win_msg, const WPARAM w_param, const LPARAM l_param) {
    (void) printf("MSG: win_msg=%i; w_param=%llu; l_param=%lld\n", win_msg, w_param, l_param);
    switch (win_msg) {
        case WM_ACTIVATE:
            if (LOWORD(w_param) == WA_INACTIVE) {
                toggle_win_off(win_handle);
            }
            break;
        case WM_COMMAND:
            on_command_msg(win_handle, win_msg, w_param, l_param);
            break;
        case WM_CREATE:
            // Add buttons
            if (!add_button(win_handle)) {
                (void) printf("ERR @ init_win: Failed add buttons.\n");
            }
            break;
        case WM_HOTKEY: {
            switch (w_param) {
                case 1: // Launch id
                    if (!toggle_win(win_handle)) {
                        (void) printf("ERR @ WndProc: An error occurred whilst toggling window visibility.\n");
                    }
                    break;
                case 2: // Launch id
                    deconstruct(win_handle);
                    DestroyWindow(win_handle);
                    PostQuitMessage(0);
                    break;
                default: ;
                    return 0;
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
