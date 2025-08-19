//
// Created by panda on 8/15/2025.
//

#ifndef KAOMOIJ_CONFIG_H
#define KAOMOIJ_CONFIG_H

#include "../util/keybind.h"
#include "../util/type.h"

typedef enum e_window_display_corner {
    TOP_LEFT,
    TOP_RIGHT,
    BOT_LEFT,
    BOT_RIGHT
} t_win_corner;

typedef struct s_config {
    t_win_corner corner; // win_corner
    t_u8 width; // win_width
    t_u8 height; // win_height
    t_u8 cols; // win_cols
    t_keybind launch; // bind_launch
} t_conf;

extern t_conf conf;

struct _s_kvp {
    char *k;
    char *v;
    size_t k_len, v_len;
};

extern int _split_kv(struct _s_kvp *out, const char *str);

extern int _parse_conf_line(size_t len, const char *line);

extern int _parse_conf_content(size_t len, char *content);

extern int parse_conf();

#endif //KAOMOIJ_CONFIG_H
