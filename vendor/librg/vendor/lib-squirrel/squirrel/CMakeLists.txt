cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(squirrel)

add_library(squirrel STATIC
    sqapi.cpp
    sqbaselib.cpp
    sqfuncstate.cpp
    sqdebug.cpp
    sqlexer.cpp
    sqobject.cpp
    sqcompiler.cpp
    sqstate.cpp
    sqtable.cpp
    sqmem.cpp
    sqvm.cpp
    sqclass.cpp
)

target_include_directories(squirrel PUBLIC ../include)

if(MSVC)
    target_compile_definitions(squirrel PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(squirrel PRIVATE -Wall -O2 -fno-exceptions -fno-rtti -fno-strict-aliasing)
endif()

if(SQUIRREL_USE_WCHAR)
    target_compile_definitions(squirrel PUBLIC SQUNICODE $<$<BOOL:MSVC>:_CRT_NON_CONFORMING_SWPRINTFS>)
endif()
