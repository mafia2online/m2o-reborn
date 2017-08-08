cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(sq)

add_executable(sq sq.c)

target_link_libraries(sq PRIVATE squirrel sqstdlib)

if(MSVC)
    target_compile_definitions(sq PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(sq PRIVATE -O2 -fno-exceptions)
endif()
