//
// Created by panda on 8/15/2025.
//

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

t_conf conf = {
    TOP_RIGHT, 240, 120,
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
    // Get application data location
    const DWORD max_env_size = 65535;
    // See https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getenvironmentvariable#parameters
    wchar_t buffer[max_env_size];
    const DWORD buffer_len = GetEnvironmentVariableW(L"LOCALAPPDATA", buffer, max_env_size);
    if (buffer_len == 0 || buffer_len > max_env_size) {
        printf("ERR @ parse_conf: Could not get LOCALAPPDATA. See last error: '%lu'.\n", GetLastError());
        return 0;
    }

    // Get config file location
    wchar_t dir[MAX_PATH];
    memcpy(&dir, &buffer, MAX_PATH);
    wcscat_s(dir, MAX_PATH, L"\\kaomoji");
    if (!CreateDirectoryW(dir, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
        printf("IO ERR @ parse_conf: Could not create directory '%ls'. See last error: '%lu'.\n",
               dir, GetLastError());
        return 0;
    }
    wcscat_s(dir, MAX_PATH, L"\\config.txt");

    // Get file handle
    HANDLE hFile = CreateFileW(dir,
                               GENERIC_READ,
                               0,
                               NULL,
                               OPEN_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL,
                               NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        (void) printf("IO ERR @ parse_conf: Couldn't get the handle to the file '%ls'. See last error: '%lu'.ln",
                      dir, GetLastError());
        return 0;
    }

    // Get file size
    LARGE_INTEGER size;
    if (!GetFileSizeEx(hFile, &size)) {
        CloseHandle(hFile);
        (void) printf("IO ERR @ parse_conf: Could not get the size of file '%ls'. See last error: '%lu'.\n",
                      dir, GetLastError());
        return 0;
    }

    if (size.QuadPart > SIZE_MAX) {
        CloseHandle(hFile);
        (void) printf("ERR @ parse_conf: Size of file '%ls' is to too large to be allocated.\n", dir);
        return 0;
    }

    const size_t len = (size_t) size.QuadPart;
    char *content = malloc(len + 1);
    if (content == NULL) {
        CloseHandle(hFile);
        (void) printf("MEM ERR @ parse_conf: Failed to allocate memory for file at @ '%ls' contents.\n", dir);
        return 0;
    }
    content[len] = '\0';

    // Read file
    DWORD read = 0;
    if (!ReadFile(hFile, content, len, &read, NULL)) {
        CloseHandle(hFile);
        free(content);
        (void) printf("IO ERR @ parse_conf: Could not read file '%ls'. See last error: '%lu'.\n",
                      dir, GetLastError());
        return 0;
    }

    // Parse file contents
    if (!_parse_conf_content(len, content)) {
        CloseHandle(hFile);
        free(content);
        (void) printf("ERR @ parse_conf: An error occurred whilst parsing file '%ls'.\n", dir);
        return 0;
    }

    CloseHandle(hFile);
    free(content);
    return 1;
}
