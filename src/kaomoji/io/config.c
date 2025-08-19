//
// Created by panda on 8/15/2025.
//

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "file.h"

t_conf conf = {
    TOP_RIGHT, 240, 120, 3,
    {1, MOD_CONTROL | MOD_ALT, 'A'}
};

int _split_kv(struct _s_kvp *out, const char *str) {
    const char *eq = strchr(str, '=');
    if (eq == NULL) {
        (void) printf("NULL ERR @ _split_kv: No '=' sign found.\n");
        return 0;
    }

    // Key
    out->k_len = eq - str;
    out->k = malloc(out->k_len + 1);
    if (out->k == NULL) {
        (void) printf("MEM ERR @ _split_kv: Count not alloc mem to key.\n");
        return 0;
    }
    strncpy(out->k, str, out->k_len);
    out->k[out->k_len] = L'\0';

    // Value
    const char *v = eq + 1;
    out->v_len = strlen(v);
    out->v = malloc(out->v_len + 1);
    if (out->v == NULL) {
        free(out->k);
        (void) printf("MEM ERR @ _split_kv: Count not alloc mem to value.\n");
        return 0;
    }
    strcpy(out->v, v);

    return 1;
}

int _parse_conf_line(size_t len, const char *line) {
    // Get kvp from line
    struct _s_kvp kvp;
    if (!_split_kv(&kvp, line)) {
        (void) printf("ERR @ _parse_conf_line: Invalid kvp.\n");
        return 0;
    }

    // Parse key
    if (strcmp(kvp.k, "win_corner") == 0) {
        const long corner = strtol(kvp.v, NULL, 10);
        conf.corner = corner;
    } else if (strcmp(kvp.k, "win_width") == 0) {
        const long width = strtol(kvp.v, NULL, 10);
        conf.width = width;
    } else if (strcmp(kvp.k, "win_height") == 0) {
        const long height = strtol(kvp.v, NULL, 10);
        conf.height = height;
    } else {
        (void) printf("ERR @ _parse_conf_line: Key '%s' not found.\n", kvp.k);
        return 0;
    }
    return 1;
}

int _parse_conf_content(size_t len, char *content) {
    if (len < 1) {
        (void) printf("WARN @ _parse_conf_content: No content to be parsed.\n");
        return 0;
    }

    // Read conf line by line
    int line_num = 0;
    const char *str = strtok(content, "\n");
    while (str != NULL) {
        line_num++;
        const size_t str_len = strlen(str);

        // Skip comments and empty line
        if (str_len > 1 && str[0] != '#') {
            if (!_parse_conf_line(str_len, str)) {
                (void) printf("ERR @ _parse_conf_content: Error parsing configuration @ line %i.\n", line_num);
            }
        }

        str = strtok(NULL, "\n");
    }

    return 1;
}

int parse_conf() {
    // Get config file location
    t_path dir;
    get_data_path(&dir, L"config.txt");

    // Read file contents
    size_t len;
    char *d_content;
    read_file(dir, &len, &d_content);

    // Parse file contents
    if (!_parse_conf_content(len, d_content)) {
        free(d_content);
        (void) printf("ERR @ parse_conf: An error occurred whilst parsing file '%ls'.\n", dir);
        return 0;
    }

    free(d_content);
    return 1;
}
