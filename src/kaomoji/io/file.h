//
// Created by panda on 8/19/2025.
//

#ifndef KAOMOIJ_FILE_H
#define KAOMOIJ_FILE_H
#include <minwindef.h>
#include "../util/type.h"

extern t_path local_app_data;
extern t_path resources_data_dir;

extern int init_file_system();

extern int _add_path(t_path *path, const wchar_t *to_add);

extern int get_data_path(t_path *path, const wchar_t *filename);

extern int get_resource_path(t_path *path, const wchar_t *filename);

extern int read_file_no_default(t_path dir, size_t *len, wchar_t **d_content);

extern int read_file(t_path dir, t_path default_data, size_t *len, wchar_t **d_content);

#endif //KAOMOIJ_FILE_H