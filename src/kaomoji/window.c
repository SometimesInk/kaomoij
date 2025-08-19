//
// Created by panda on 8/18/2025.
//

#include "../window.h"

#include <stdio.h>

#include "config.h"
#include "message.h"

const char g_szClassName[] = "windowClass";

int _get_win_class(WNDCLASSEX *win_class, HINSTANCE hInstance) {
    win_class->cbSize = sizeof(WNDCLASSEX);
    win_class->style = 0;
    win_class->lpfnWndProc = WndProc;
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
        printf("WIN ERR @ _get_win_class: Failed to register window class. See last error: '%lu'.",
               GetLastError());
        return 0;
    }
    return 1;
}

int _get_win_handle(HWND *win_handle, HINSTANCE hInstance) {
    *win_handle = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        NULL,
        WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT, conf.width, conf.height,
        NULL, NULL, hInstance, NULL);

    if (*win_handle == NULL) {
        printf("WIN ERR @ _get_win_handle: Failed to create window handle. See last error: '%lu'.",
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

int init_win(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize window class
    WNDCLASSEX win_class;
    if (!_get_win_class(&win_class, hInstance)) {
        printf("ERR @ init_win: Failed to initialize window class.");
        return 0;
    }

    // Initialize handle
    HWND win_handle;
    if (!_get_win_handle(&win_handle, hInstance)) {
        printf("ERR @ init_win: Failed to initialize window handle.");
        return 0;
    }

    // Register keybinds
    register_keybind(conf.launch, win_handle);

    // Enter window loop
    MSG win_msg;
    _win_loop(&win_msg);

    // Deconstruct
    unregister_keybind(conf.launch, win_handle);
    return win_msg.wParam;
}
