//
// Created by panda on 8/20/2025.
//

// ReSharper disable CppDFADeletedPointer
#include "entry.h"

#include <stdio.h>

#include "file.h"

wchar_t **d_entries;
size_t entries_len;

wchar_t *_parse_entry(const size_t len, const wchar_t *start) {
    // Allocate memory for entry
    wchar_t *entry = malloc((len + 1) * sizeof(wchar_t));
    if (entry == NULL) {
        (void) printf("MEM ERR @ _parse_entry: Failed to allocate memory for entries.\n");
        return 0;
    }

    // Copy substring
    (void) memcpy(entry, start, len * sizeof(wchar_t));

    // Null terminate
    entry[len] = L'\0';

    return entry;
}

int _parse_entries(const wchar_t *content) {
    // Reset global length
    entries_len = 0;

    // Count the number of lines
    size_t lines = 0;
    const wchar_t *start = content;
    if (start[0] == 0xFEFF) start++;
    const wchar_t *nl = wcsstr(start, L"\r\n");
    while (nl != NULL) {
        lines++;
        start = nl + 2; // Skip NL
        nl = wcsstr(start, L"\r\n");
    }
    if (*start != L'\0') {
        lines++;
    }

    if (lines < 1) {
        (void) printf("ERR @ _parse_entries: No lines to parse.\n");
        return 1;
    }

    // Allocate memory for entries
    d_entries = malloc(lines * sizeof(*d_entries));
    if (d_entries == NULL) {
        (void) printf("MEM ERR @ _parse_entries: Failed to allocate memory for entries.");
        return 0;
    }

    // Extract lines
    start = content;
    nl = wcsstr(start, L"\r\n");
    while (nl != NULL) {
        // Allocate memory for entry
        const size_t len = nl - start;
        wchar_t *entry = _parse_entry(len, start);
        if (entry == NULL) {
            (void) printf("ERR @ _parse_entries: Couldn't get pointer to entry.");
            dispose_entries();
            return 0;
        }

        d_entries[entries_len++] = entry;

        start = nl + 2;
        nl = wcsstr(start, L"\r\n");
    }

    // Tailing content
    if (*start != L'\0') {
        const size_t len = wcslen(start);
        wchar_t *entry = _parse_entry(len, start);
        if (entry == NULL) {
            (void) printf("ERR @ _parse_entries: Couldn't get pointer to entry.");
            dispose_entries();
            return 0;
        }

        d_entries[entries_len++] = entry;
    }

    return 1;
}


void dispose_entries() {
    for (size_t j = 0; j < entries_len; j++) free(d_entries[j]);
    free(d_entries);
    (void) printf("Disposed of entries.\n");
}

int parse_entries() {
    // Get config file location
    t_path dir;
    if (!get_data_path(&dir, L"entries.wtxt")) {
        (void) printf("ERR @ parse_entries: Error getting file path.\n");
        return 0;
    }

    // Get default config file location
    t_path default_dir;
    if (!get_resource_path(&default_dir, L"config\\entries.wtxt")) {
        (void) printf("ERR @ parse_entries: Error getting default file path.\n");
        return 0;
    }

    // Read file contents
    size_t len;
    wchar_t *d_content;
    if (!read_file(dir, default_dir, &len, &d_content)) {
        (void) printf("ERR @ parse_entries: Error reading file.\n");
        return 0;
    }

    // Parse entries
    if (!_parse_entries(d_content)) {
        (void) printf("ERR @ parse_entries: Error parsing entries.\n");
        free(d_content);
        return 0;
    }

    free(d_content);
    return 1;
}
