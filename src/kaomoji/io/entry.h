//
// Created by panda on 8/20/2025.
//

#ifndef KAOMOIJ_ENTRY_H
#define KAOMOIJ_ENTRY_H
#include <stddef.h>

extern wchar_t **d_entries;
extern size_t entries_len;

extern int parse_entries();

extern void dispose_entries();

extern wchar_t *_parse_entry(size_t len, const wchar_t *start);

extern int _parse_entries(const wchar_t *content);

#endif //KAOMOIJ_ENTRY_H
