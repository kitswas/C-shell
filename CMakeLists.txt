cmake_minimum_required(VERSION 3.16)
project(C-shell)

set(CMAKE_C_STANDARD 11)

set(SOURCES
	src/main.c
	src/internal/cd.c
	src/internal/echo.c
	src/internal/ls.c
	src/internal/pwd.c
	src/parse.c
)

# Some compiler flags for GCC and Clang, disable if unwanted
if("${CMAKE_C_COMPILER_ID}" MATCHES "Clang" OR "${CMAKE_C_COMPILER_ID}" MATCHES "GNU") 
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -Wnull-dereference")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g -DDEBUG")
	# set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Werror") # Uncomment to treat warnings as errors in debug builds
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -s -DNDEBUG")
endif()

add_executable(cshell ${SOURCES})
