cmake_minimum_required(VERSION 4.0)
project(kaomoij C)

set(CMAKE_C_STANDARD 11)

add_executable(kaomoij
        src/kaomoji/main.c
        src/kaomoji/config.c
        src/kaomoji/config.h
        src/kaomoji/message.c
        src/kaomoji/message.h
        src/kaomoji/window.c
        src/kaomoji/window.h
        src/kaomoji/keybind.c
        src/kaomoji/keybind.h)