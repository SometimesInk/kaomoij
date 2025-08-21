//
// Created by panda on 8/19/2025.
//

#include "file.h"

#include <windows.h>
#include <stdio.h>

#define BOM 0xFEFF

t_path local_app_data;
t_path resources_data_dir;

int init_file_system() {
    // Get application data location
    // See https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getenvironmentvariable#parameters
    const DWORD max_env_size = 65535;

    wchar_t buffer[max_env_size];
    const DWORD buffer_len = GetEnvironmentVariableW(L"LOCALAPPDATA", buffer, max_env_size);
    if (buffer_len == 0 || buffer_len > max_env_size) {
        (void) printf("IO ERR @ init_file_system: Could not get LOCALAPPDATA. See last error: '%lu'.\n",
                      GetLastError());
        return 0;
    }

    (void) memcpy(local_app_data, &buffer, MAX_PATH);
    if (wcscat_s(local_app_data, MAX_PATH, L"\\kaomoji")) {
        (void) printf("ERR @ init_file_system: Failed to concatenate path.\n");
        return 0;
    }

    // Get resources directory
    t_path wd_path;
    if (!GetCurrentDirectoryW(MAX_PATH, wd_path)) {
        (void) printf("IO ERR @ init_file_system: Failed to get current directory. See last error: '%lu'.\n",
                      GetLastError());
        return 0;
    }

    // Add resource directory to path
    if (!_add_path(&wd_path, L"..\\src\\resources")) {
        (void) printf("ERR @ get_resource_path: Failed to add resource path.\n");
        return 0;
    }

    // Resolve path
    if (!GetFullPathNameW(wd_path, MAX_PATH, resources_data_dir, NULL)) {
        (void) printf("IO ERR @ get_resource_path: Failed to add resource path. See last error: '%lu'.\n",
                      GetLastError());
        return 0;
    }
    return 1;
}

int _add_path(t_path *path, const wchar_t *to_add) {
    if (wcscat_s(*path, MAX_PATH, L"\\")) {
        (void) printf("ERR @ _add_path: Failed to concatenate slash.\n");
        return 0;
    }
    if (wcscat_s(*path, MAX_PATH, to_add)) {
        (void) printf("ERR @ get_resource_path: Failed to concatenate string to be added.\n");
        return 0;
    }
    return 1;
}

int get_data_path(t_path *path, const wchar_t *filename) {
    (void) memcpy(*path, local_app_data, MAX_PATH);
    return _add_path(path, filename);
}

int get_resource_path(t_path *path, const wchar_t *filename) {
    (void) memcpy(*path, resources_data_dir, MAX_PATH);
    return _add_path(path, filename);
}

int read_file_no_default(t_path dir, size_t *len, wchar_t **d_content) {
    // Get file handle
    const HANDLE file = CreateFileW(dir, GENERIC_READ, 0, NULL,
                                    OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,
                                    NULL);
    if (file == INVALID_HANDLE_VALUE) {
        (void) printf("IO ERR @ read_file_no_default: Couldn't get the handle of file @ '%ls'. "
                      "See last error: '%lu'.\n", dir, GetLastError());
        return 0;
    }

    // Get file size
    LARGE_INTEGER size;
    if (!GetFileSizeEx(file, &size)) {
        (void) printf("IO ERR @ read_file_no_default: Could not get the size of file '%ls'. "
                      "See last error: '%lu'.\n", dir, GetLastError());
        goto dispose;
    }

    if (size.QuadPart > SIZE_MAX) {
        (void) printf("ERR @ read_file_no_default: Size of file '%ls' is to too large to be allocated.\n", dir);
        goto dispose;
    }

    *len = (size_t) size.QuadPart / sizeof(wchar_t);
    *d_content = (wchar_t *) malloc((*len + 1) *  sizeof(wchar_t));
    if (*d_content == NULL) {
        (void) printf("MEM ERR @ read_file_no_default: "
                      "Failed to allocate memory for file at @ '%ls' contents.\n", dir);
        goto dispose;
    }

    // Read file
    DWORD read = 0;
    if (!ReadFile(file, *d_content, size.QuadPart, &read, NULL)) {
        free(*d_content);
        (void) printf("IO ERR @ read_file_no_default: Could not read file '%ls'. See last error: '%lu'.\n",
                      dir, GetLastError());
        goto dispose;
    }
    (*d_content)[read / sizeof(wchar_t)] = L'\0';

    if (!CloseHandle(file)) {
        (void) printf("IO ERR @ read_file_no_default: Failed to close file handle. See last error: '%lu'.\n",
                      GetLastError());
    }
    return 1;

dispose:
    if (!CloseHandle(file)) {
        (void) printf("IO ERR @ read_file_no_default: Failed to close file handle. See last error: '%lu'.\n",
                      GetLastError());
    }
    return 0;
}

int read_file(t_path dir, t_path default_data, size_t *len, wchar_t **d_content) {
    // Check if file exists
    const DWORD attrs = GetFileAttributesW(dir);
    const int file_exists = attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY);

    // Get file handle
    const HANDLE file = CreateFileW(dir, GENERIC_READ | GENERIC_WRITE,
                                    0, NULL,
                                    OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,
                                    NULL);
    if (file == INVALID_HANDLE_VALUE) {
        (void) printf("IO ERR @ read_file: Couldn't get the handle of file @ '%ls'. See last error: '%lu'.\n",
                      dir, GetLastError());
        return 0;
    }

    // If file is new, copy default data
    if (!file_exists) {
        // Get default data
        wchar_t *data;
        size_t data_len;
        if (!read_file_no_default(default_data, &data_len, &data)) {
            (void) printf("ERR @ read_file: Failed to read default file data.\n");
            free(data);
            goto dispose;
        }

        // Write to file
        DWORD data_bytes_written;
        if (!WriteFile(file, data, data_len * sizeof(wchar_t), &data_bytes_written, NULL)) {
            (void) printf("IO ERR @ read_file: Failed to write default file data. See last error: '%lu'.\n",
                          GetLastError());
            free(data);
            goto dispose;
        }
        free(data);
    }

    // Get file size
    LARGE_INTEGER size;
    if (!GetFileSizeEx(file, &size)) {
        (void) printf("IO ERR @ read_file: Could not get the size of file '%ls'. See last error: '%lu'.\n",
                      dir, GetLastError());
        goto dispose;
    }

    if (size.QuadPart > SIZE_MAX) {
        (void) printf("ERR @ read_file: Size of file '%ls' is to too large to be allocated.\n", dir);
        goto dispose;
    }

    // Allocate memory for content
    *len = size.QuadPart / sizeof(wchar_t);
    *d_content = (wchar_t *) malloc((*len + 1) * sizeof(wchar_t));
    if (*d_content == NULL) {
        (void) printf("MEM ERR @ read_file: Failed to allocate memory for file at @ '%ls' contents.\n", dir);
        goto dispose;
    }

    // Read file
    DWORD read = 0;
    if (!ReadFile(file, *d_content, size.QuadPart, &read, NULL)) {
        free(*d_content);
        (void) printf("IO ERR @ read_file: Could not read file '%ls'. See last error: '%lu'.\n",
                      dir, GetLastError());
        goto dispose;
    }
    (*d_content)[read / sizeof(wchar_t)] = L'\0';

    if (!CloseHandle(file)) {
        (void) printf("IO ERR @ read_file: Failed to close file handle. See last error: '%lu'.\n",
                      GetLastError());
    }
    return 1;

dispose:
    if (!CloseHandle(file)) {
        (void) printf("IO ERR @ read_file: Failed to close file handle. See last error: '%lu'.\n",
                      GetLastError());
    }
    return 0;
}
