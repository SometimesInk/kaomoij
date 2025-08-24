//
// Created by panda on 8/23/2025.
//

#ifndef KAOMOIJ_LOG_H
#define KAOMOIJ_LOG_H

#define LOG_LAST_ERRS

typedef enum e_errs {
    ERR,
    ERR_RENDER,
    ERR_IO,
    ERR_WIN32,
    ERR_MEM,
    ERR_NULL
} t_errs;

extern void log_last_err();

#endif //KAOMOIJ_LOG_H