#include <windows.h>

#include "config.h"
#include "../window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    // Initialize config
    parse_conf();

    // Initialize window
    return init_win(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
