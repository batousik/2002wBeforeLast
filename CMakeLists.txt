cmake_minimum_required (VERSION 2.8.11)
project (sp1)

set(CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra -g")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_executable(sp1 src/sp1.c)
add_executable(sp1_p2 src/sp1_p2.c)