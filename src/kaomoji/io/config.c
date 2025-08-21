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
    TOP_RIGHT, 240, 120, 3, 6,
    {1, MOD_CONTROL | MOD_ALT, 'A'}
};

int _split_kv(struct _s_kvp *out, const wchar_t *str) {
    const wchar_t *eq = wcschr(str, L'=');
    if (eq == NULL) {
        (void) printf("NULL ERR @ _split_kv: No '=' sign found.\n");
        return 0;
    }

    // Key
    out->len_k = eq - str;
    out->d_k = malloc(out->len_k + 1);
    if (out->d_k == NULL) {
        (void) printf("MEM ERR @ _split_kv: Count not alloc mem to key.\n");
        return 0;
    }
    (void) wcsncpy(out->d_k, str, out->len_k);
    out->d_k[out->len_k] = L'\0';

    // Value
    const wchar_t *v = eq + 1;
    out->len_v = wcslen(v);
    out->d_v = malloc(out->len_v + 1);
    if (out->d_v == NULL) {
        free(out->d_k);
        (void) printf("MEM ERR @ _split_kv: Count not alloc mem to value.\n");
        return 0;
    }
    (void) wcscpy(out->d_v, v);

    return 1;
}

int _parse_conf_line(size_t len, const wchar_t *line) {
    // Get kvp from line
    struct _s_kvp kvp;
    if (!_split_kv(&kvp, line)) {
        (void) printf("ERR @ _parse_conf_line: Invalid kvp.\n");
        return 0;
    }

    // Parse key
    if (wcscmp(kvp.d_k, L"win_corner") == 0) {
        const long corner = wcstol(kvp.d_v, NULL, 10);
        conf.corner = corner;
    } else if (wcscmp(kvp.d_k, L"win_width") == 0) {
        const long width = wcstol(kvp.d_v, NULL, 10);
        conf.width = width;
    } else if (wcscmp(kvp.d_k, L"win_height") == 0) {
        const long height = wcstol(kvp.d_v, NULL, 10);
        conf.height = height;
    } else if (wcscmp(kvp.d_k, L"win_cols") == 0) {
        const long cols = wcstol(kvp.d_v, NULL, 10);
        conf.cols = cols;
    } else if (wcscmp(kvp.d_k, L"win_rows") == 0) {
        const long rows = wcstol(kvp.d_v, NULL, 10);
        conf.rows = rows;
    } else if (wcscmp(kvp.d_k, L"bind_launch") == 0) {
        t_keybind launch;
        string_to_keybind(&launch, kvp.d_v);
        conf.launch = launch;
    } else {
        (void) printf("ERR @ _parse_conf_line: Key '%ws' not found.\n", kvp.d_k);
        free(kvp.d_k);
        free(kvp.d_v);
        return 0;
    }
    free(kvp.d_k);
    free(kvp.d_v);
    return 1;
}

int _parse_conf_content(size_t len, wchar_t *content) {
    if (len < 1) {
        (void) printf("WARN @ _parse_conf_content: No content to be parsed.\n");
        return 0;
    }

    // Read conf line by line
    int line_num = 0;
    const wchar_t *str = wcstok(content, L"\n");
    while (str != NULL) {
        line_num++;
        const size_t str_len = wcslen(str);

        // Skip comments and empty line
        if (str_len > 1 && str[0] != '#') {
            if (!_parse_conf_line(str_len, str)) {
                (void) printf("ERR @ _parse_conf_content: Error parsing configuration @ line %i.\n", line_num);
            }
        }

        str = wcstok(NULL, L"\n");
    }

    return 1;
}

int parse_conf() {
    // Get config file location
    t_path dir;
    if (!get_data_path(&dir, L"config.wtxt")) {
        (void) printf("ERR @ parse_conf: Error getting file path.\n");
        return 0;
    }

    // Get default config file location
    t_path default_dir;
    if (!get_resource_path(&default_dir, L"config\\config.wtxt")) {
        (void) printf("ERR @ parse_conf: Error getting default file path.\n");
        return 0;
    }

    // Read file contents
    size_t len;
    wchar_t *d_content;
    if (!read_file(dir, default_dir, &len, &d_content)) {
        (void) printf("ERR @ parse_conf: Error reading file.\n");
        return 0;
    }

    // Parse file contents
    if (!_parse_conf_content(len, d_content)) {
        free(d_content);
        (void) printf("ERR @ parse_conf: An error occurred whilst parsing file '%ls'.\n", dir);
        return 0;
    }

    free(d_content);
    return 1;
}
