cmake_minimum_required(VERSION 3.16)
project(prog3 C)

set(CMAKE_C_STANDARD 99)

add_executable(prog3 main.c bbuffer.c bbuffer.h testbbuffer.c)