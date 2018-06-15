cmake_minimum_required(VERSION 2.8)
project(m2sdk)

file(GLOB_RECURSE M2SDK_FILES
    ${PROJECT_SOURCE_DIR}/include/classes/*.h*
    ${PROJECT_SOURCE_DIR}/include/wrappers/*.h*
    ${PROJECT_SOURCE_DIR}/include/utils/*.h*
    ${PROJECT_SOURCE_DIR}/include/*.h*
    ${PROJECT_SOURCE_DIR}/src/*.h**
    ${PROJECT_SOURCE_DIR}/src/m2sdk.cpp
)

add_library(m2sdk STATIC ${M2SDK_FILES})

target_link_libraries(m2sdk detours)
target_include_directories(m2sdk PUBLIC include
    ${PROJECT_SOURCE_DIR}/../../vendor/detours/include
    ${PROJECT_SOURCE_DIR}/../../vendor/librg/include
)

if(MSVC)
    GroupSources(include)
    GroupSources(src)
endif()

