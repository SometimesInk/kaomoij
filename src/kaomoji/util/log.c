//
// Created by panda on 8/23/2025.
//

#include "log.h"

#include <stdio.h>
#include <windows.h>

void log_msg(t_errs type, char *msg) {

}

void log_last_err() {
    (void) printf(" See last error: '%lu'.\n", GetLastError());
}
