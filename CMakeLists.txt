cmake_minimum_required(VERSION 3.16)
project(C-shell)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES ./src/main.c)
add_executable(executable ${SOURCES})
