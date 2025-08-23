cmake_minimum_required(VERSION 4.0)
project(kaomoij C)

set(CMAKE_C_STANDARD 11)

add_executable(kaomoij
        src/kaomoji/main.c
        src/kaomoji/io/config.c
        src/kaomoji/io/config.h
        src/kaomoji/window/message.c
        src/kaomoji/window/message.h
        src/kaomoji/window/window.c
        src/kaomoji/window/window.h
        src/kaomoji/util/keybind.c
        src/kaomoji/util/keybind.h
        src/kaomoji/window/button.c
        src/kaomoji/window/button.h
        src/kaomoji/util/type.c
        src/kaomoji/util/type.h
        src/kaomoji/io/file.c
        src/kaomoji/io/file.h
        src/kaomoji/io/entry.c
        src/kaomoji/io/entry.h
        src/kaomoji/window/toggle.c
        src/kaomoji/window/toggle.h
        src/kaomoji/util/clipboard.c
        src/kaomoji/util/clipboard.h)