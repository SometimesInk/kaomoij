#include <stdio.h>
#include <windows.h>

#include "io/config.h"
#include "io/entry.h"
#include "io/file.h"
#include "window/window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    // Initialize config
    if (!init_file_system()) {
        (void) printf("INIT ERR WinMain: Failed to initialize file system.");
        return 0;
    }
    if (!parse_conf()) {
        (void) printf("INIT ERR WinMain: Failed to parse config.");
        return 0;
    }
    if (!parse_entries()) {
        (void) printf("INIT ERR WinMain: Failed to initialize file system.");
        return 0;
    }

    // Initialize window
    return init_win(hInstance);
}
