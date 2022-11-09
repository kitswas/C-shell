cmake_minimum_required(VERSION 3.16)
project(C-shell)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES ./src/main.c ./src/internal/echo.c ./src/internal/pwd.c)
add_executable(executable ${SOURCES})
