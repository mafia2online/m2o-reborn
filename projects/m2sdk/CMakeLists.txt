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
    ${PROJECT_SOURCE_DIR}/../../vendor
)

if (M2O_EXPORT_SDK)
    message("-- M2O_EXPORT_SDK enabled, auto exporting m2sdk to '../m2sdk' on every build")

    add_custom_command(TARGET m2sdk POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/m2sdk.lib" "${CMAKE_SOURCE_DIR}/../m2sdk/lib/"
    )

    add_custom_command(TARGET m2sdk POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/../m2sdk/include"
    )
endif()

if(MSVC)
    GroupSources(include)
    GroupSources(src)
endif()
