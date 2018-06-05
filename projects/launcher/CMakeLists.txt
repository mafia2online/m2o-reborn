cmake_minimum_required(VERSION 2.8)
project(launcher)

if (NOT WIN32)
    message(FATAL_ERROR "launcher supports only win32 for now")
endif()

link_directories(${PROJECT_SOURCE_DIR}/../../vendor/steam_api/lib)
add_executable(launcher "${PROJECT_SOURCE_DIR}/launcher.cpp")

target_link_libraries(launcher psapi steam_api)
target_include_directories(launcher PUBLIC
    ${PROJECT_SOURCE_DIR}/../../vendor/steam_api/include
)

if (MSVC)
    set_target_properties(launcher PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    set_target_properties(launcher PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")

    add_custom_command(TARGET launcher POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/launcher.exe" "${CMAKE_SOURCE_DIR}/binary/"
    )
endif()
