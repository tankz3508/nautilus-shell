cmake_minimum_required(VERSION 3.28)

project(nautilus LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(nautilus ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(nautilus PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(nautilus PRIVATE
    $<$<C_COMPILER_ID:GNU,Clang>:
        -Wall -Wextra -Wpedantic 
        -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes
    >
    $<$<C_COMPILER_ID:MSVC>:/W4 /permissive->
)

target_compile_options(nautilus PRIVATE
    $<$<CONFIG:Debug>:
        $<$<C_COMPILER_ID:GNU,Clang>:-O0 -g3 -ggdb>
        $<$<C_COMPILER_ID:Clang,GNU>:-fsanitize=address,undefined>
    >
    $<$<CONFIG:Release>:
        $<$<C_COMPILER_ID:GNU,Clang>:-O3 -march=native -flto>
    >
)

target_link_options(nautilus PRIVATE
    $<$<CONFIG:Debug>:
        $<$<C_COMPILER_ID:Clang,GNU>:-fsanitize=address,undefined>
    >
    $<$<CONFIG:Release>:
        $<$<C_COMPILER_ID:GNU,Clang>:-fuse-ld=lld -flto>
    >
)
