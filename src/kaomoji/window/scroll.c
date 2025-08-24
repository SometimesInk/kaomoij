//
// Created by panda on 8/23/2025.
//

#include "scroll.h"

#include <stdio.h>

#include "../util/log.h"

int scroll_offset_y = 0;

void scroll_window(const HWND win_handle, const WPARAM w_param) {
    scroll_offset_y(win_handle, GET_WHEEL_DELTA_WPARAM(w_param));
}

int scroll_window_by(const HWND win_handle, const int delta) {
    scroll_offset_y += (delta > 0 ? 20 : -20);

    // Move buttons
    HDWP hdwp = BeginDeferWindowPos(buttons_len);
    if (hdwp == NULL) {
        (void) printf("WIN ERR @ scroll_window_by: Failed to call BeginDeferWindowPos.");
        log_last_err();
    }
    for (size_t i = 0; i < buttons_len; i++) {
        RECT rect;
        if (GetWindowRect(d_buttons[i], &rect)) {
            (void) printf("WIN ERR @ scroll_window_by: Failed to get window rect.");
            log_last_err();
            return 0;
        }
        SetLastError(0);
        if (!MapWindowPoints(HWND_DESKTOP, win_handle, (POINT *) &rect, 2) &&
            GetLastError() != 0) {
            (void) printf("WIN ERR @ scroll_window_by: Failed to map window points.");
            log_last_err();
            return 0;
        }

        hdwp = DeferWindowPos(hdwp, d_buttons[i], NULL,
                              rect.left, rect.top + (delta > 0 ? 20 : -20),
                              0, 0,
                              SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
    }
    if (!EndDeferWindowPos(hdwp)) {
        (void) printf("WIN ERR @ scroll_window_by: Failed to end defer window pos. See last error: '%lu'.\n",
            GetLastError());
        return 0;
    }
    return 1;
}
