//
// Created by panda on 8/19/2025.
//

#include <math.h>

#include "button.h"

#include <stdio.h>

#include "toggle.h"
#include "../io/entry.h"
#include "../util/clipboard.h"

HWND *d_buttons;
size_t buttons_len;
HFONT font;

int init_font(void) {
    font = CreateFontW(-12, 0, 0, 0, // TODO: Make this configurable
        FW_NORMAL,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Arial");
    if (font == NULL) {
        (void) printf("ERR @ init_font: Failed to create font.\n");
        return 0;
    }
    return 1;
}

int add_button(const HWND win_handle) {
    // Allocate memory for buttons
    d_buttons = malloc(entries_len * sizeof(HWND));
    if (d_buttons == NULL) {
        (void) printf("MEM ERR @ add_button: Failed to allocate memory for buttons.\n");
        return 0;
    }

    // Create buttons
    const int width = conf.width / conf.cols;
    const int height = conf.height / conf.rows;
    for (int i = 0; i < entries_len; ++i) {
        HWND btn = CreateWindowW(L"BUTTON", d_entries[i],
                     WS_VISIBLE | WS_CHILD,
                     (i % conf.cols) * width,
                     (i / conf.cols) * height,
                     width, height,
                     win_handle, (HMENU)(intptr_t)i, NULL, NULL);
        if (btn == NULL) {
            (void) printf("WIN ERR @ add_button: Failed to add button @ i=%i. See last error: '%lu'.\n",
                i, GetLastError());
            return 0;
        }

        (void) SendMessageW(btn, WM_SETFONT, (WPARAM)font, TRUE);

        d_buttons[buttons_len++] = btn;
    }
    return 1;
}

void on_command_msg(const HWND win_handle, const UINT win_msg, const WPARAM w_param, const LPARAM l_param) {
    const int event = HIWORD(w_param);
    if (event != BN_CLICKED) return;

    HWND sender = (HWND) l_param;
    if (sender == NULL) return;

    // Find button sender
    for (size_t i = 0; i < buttons_len; i++) {
        if (d_buttons[i] == sender) {
            _on_click(sender, i);
            break;
        }
    }

    if (conf.hide_on_inactive) toggle_win_off(win_handle);
}

void _on_click(HWND sender, const size_t i) {
    set_clipboard(d_entries[i]);
}

void dispose_buttons(void) {
    for (size_t j = 0; j < buttons_len; j++) free(d_buttons[j]);
    free(d_buttons);
    (void) printf("INFO @ dispose_buttons: Disposed of entries.\n");
}

void dispose_fonts(void) {
    if (DeleteObject(font)) {
        (void) printf("DCON ERR @ deconstruct_button: Failed to delete object.\n");
    }
}