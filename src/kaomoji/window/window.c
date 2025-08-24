//
// Created by panda on 8/18/2025.
//

#include "window.h"

#include <stdio.h>

#include "button.h"
#include "../io/config.h"
#include "message.h"
#include "../io/entry.h"

const char g_szClassName[] = "windowClass";

int _get_win_class(WNDCLASSEX *win_class, const HINSTANCE hInstance) {
    win_class->cbSize = sizeof(WNDCLASSEX);
    win_class->style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    win_class->lpfnWndProc = win_proc;
    win_class->cbClsExtra = 0;
    win_class->cbWndExtra = 0;
    win_class->hInstance = hInstance;
    win_class->hIcon = LoadIcon(NULL, IDI_APPLICATION);
    win_class->hCursor = LoadCursor(NULL, IDC_ARROW);
    win_class->hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    win_class->lpszMenuName = NULL;
    win_class->lpszClassName = g_szClassName;
    win_class->hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(win_class)) {
        (void)printf("WIN ERR @ _get_win_class: Failed to register window class. See last error: '%lu'.\n",
               GetLastError());
        return 0;
    }
    return 1;
}

int _get_win_handle(HWND *win_handle, const HINSTANCE hInstance) {
    *win_handle = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        NULL,
        WS_POPUP | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, conf.width, conf.height,
        NULL, NULL, hInstance, NULL);

    if (*win_handle == NULL) {
        (void)printf("WIN ERR @ _get_win_handle: Failed to create window handle. See last error: '%lu'.\n",
               GetLastError());
        return 0;
    }

    ShowWindow(*win_handle, SW_HIDE);
    UpdateWindow(*win_handle);
    return 1;
}

void _win_loop(MSG *win_msg) {
    while (GetMessage(win_msg, NULL, 0, 0) > 0) {
        TranslateMessage(win_msg);
        DispatchMessage(win_msg);
    }
}

int init_win(const HINSTANCE hInstance) {
    // Initialize window class
    WNDCLASSEX win_class;
    if (!_get_win_class(&win_class, hInstance)) {
        (void)printf("ERR @ init_win: Failed to initialize window class.\n");
        return 0;
    }

    // Initialize handle
    HWND win_handle;
    if (!_get_win_handle(&win_handle, hInstance)) {
        (void)printf("ERR @ init_win: Failed to initialize window handle.\n");
        return 0;
    }

    // Register keybinds
    init_keybinds(win_handle);

    // Enter window loop
    (void)printf("Ready.\n");
    MSG win_msg;
    _win_loop(&win_msg);

    // Deconstruct
    deconstruct(win_handle);
    return win_msg.wParam;
}

void deconstruct(const HWND win_handle) {
    dispose_buttons();
    dispose_entries();
    dispose_fonts();
    dispose_keybinds(win_handle);
}
