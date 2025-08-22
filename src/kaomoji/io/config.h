//
// Created by panda on 8/15/2025.
//

#ifndef KAOMOIJ_CONFIG_H
#define KAOMOIJ_CONFIG_H

#include "../util/keybind.h"
#include "../util/type.h"

#define MAX_K_LEN 1024
#define MAX_V_LEN 1024

typedef enum e_window_display_corner {
    TOP_LEFT,
    TOP_RIGHT,
    BOT_LEFT,
    BOT_RIGHT
} t_win_corner;

typedef struct s_config {
    t_win_corner corner; // win_corner
    t_u16 width; // win_width
    t_u16 height; // win_height
    t_u16 cols; // win_cols
    t_u16 rows; // win_rows
    t_keybind launch; // bind_launch
} t_conf;

extern t_conf conf;

struct _s_kvp {
    wchar_t d_k[MAX_K_LEN];
    wchar_t d_v[MAX_V_LEN];
    size_t len_k, len_v;
};

extern int _split_kv(struct _s_kvp *out, const wchar_t *str);

extern int _parse_conf_line(const wchar_t *line);

extern void _parse_conf_content(wchar_t *content);

extern int parse_conf();

#endif //KAOMOIJ_CONFIG_H
