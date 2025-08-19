//
// Created by panda on 8/19/2025.
//

#ifndef KAOMOIJ_FILE_H
#define KAOMOIJ_FILE_H
#include <minwindef.h>
#include "../util/type.h"

extern t_path local_app_data;

extern int init_file_system();

extern int get_data_path(t_path *path, const wchar_t *filename);

extern int read_file(t_path dir, size_t *len, char **d_content);

#endif //KAOMOIJ_FILE_H