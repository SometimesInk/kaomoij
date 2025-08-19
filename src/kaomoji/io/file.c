//
// Created by panda on 8/19/2025.
//

#include "file.h"

#include <windows.h>
#include <stdio.h>

t_path local_app_data;

int init_file_system() {
    // Get application data location
    // See https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getenvironmentvariable#parameters
    const DWORD max_env_size = 65535;

    wchar_t buffer[max_env_size];
    const DWORD buffer_len = GetEnvironmentVariableW(L"LOCALAPPDATA", buffer, max_env_size);
    if (buffer_len == 0 || buffer_len > max_env_size) {
        (void) printf("WIN32 ERR @ init_file_system: Could not get LOCALAPPDATA. See last error: '%lu'.\n",
                      GetLastError());
        return 0;
    }

    (void) memcpy(local_app_data, &buffer, MAX_PATH);
    if (wcscat_s(local_app_data, MAX_PATH, L"\\kaomoji")) {
        (void) printf("ERR @ init_file_system: Failed to concatenate path.\n");
        return 0;
    }
    return 1;
}

int get_data_path(t_path *path, const wchar_t *filename) {
    (void) memcpy(*path, local_app_data, MAX_PATH);
    if (wcscat_s(*path, MAX_PATH, L"\\")) {
        (void) printf("ERR @ get_data_path: Failed to concatenate slash.\n");
        return 0;
    }
    if (wcscat_s(*path, MAX_PATH, filename)) {
        (void) printf("ERR @ get_data_path: Failed to concatenate filename.\n");
        return 0;
    }
    return 1;
}

int read_file(t_path dir, size_t *len, char **d_content) {
    // Get file handle
    const HANDLE file = CreateFileW(dir, GENERIC_READ,
                                    0, NULL,
                                    OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,
                                    NULL);
    if (file == INVALID_HANDLE_VALUE) {
        (void) printf("IO ERR @ read_file: Couldn't get the handle of file @ '%ls'. See last error: '%lu'.\n",
                      dir, GetLastError());
        return 0;
    }

    // Get file size
    LARGE_INTEGER size;
    if (!GetFileSizeEx(file, &size)) {
        (void) printf("IO ERR @ read_file: Could not get the size of file '%ls'. See last error: '%lu'.\n",
                      dir, GetLastError());
        if (!CloseHandle(file)) {
            (void) printf("IO ERR @ read_file: Failed to close file handle. See last error: '%lu'.\n",
                          GetLastError());
        }
        return 0;
    }

    if (size.QuadPart > SIZE_MAX) {
        (void) printf("ERR @ read_file: Size of file '%ls' is to too large to be allocated.\n", dir);
        if (!CloseHandle(file)) {
            (void) printf("IO ERR @ read_file: Failed to close file handle. See last error: '%lu'.\n",
                          GetLastError());
        }
        return 0;
    }

    *len = (size_t) size.QuadPart;
    *d_content = (char *) malloc(size.QuadPart + 1);
    if (*d_content == NULL) {
        (void) printf("MEM ERR @ read_file: Failed to allocate memory for file at @ '%ls' contents.\n", dir);
        if (!CloseHandle(file)) {
            (void) printf("IO ERR @ read_file: Failed to close file handle. See last error: '%lu'.\n",
                          GetLastError());
        }
        return 0;
    }
    (*d_content)[size.QuadPart] = '\0';

    // Read file
    DWORD read = 0;
    if (!ReadFile(file, *d_content, *len, &read, NULL)) {
        free(*d_content);
        (void) printf("IO ERR @ read_file: Could not read file '%ls'. See last error: '%lu'.\n",
                      dir, GetLastError());
        if (!CloseHandle(file)) {
            (void) printf("IO ERR @ read_file: Failed to close file handle. See last error: '%lu'.\n",
                          GetLastError());
        }
        return 0;
    }

    if (!CloseHandle(file)) {
        (void) printf("IO ERR @ read_file: Failed to close file handle. See last error: '%lu'.\n",
                      GetLastError());
    }
    return 1;
}
