cmake_minimum_required(VERSION 2.8)
project(server)

file(GLOB_RECURSE SERVER_FILES
    "${PROJECT_SOURCE_DIR}/*.cpp"
    "${PROJECT_SOURCE_DIR}/*.h"
    "${PROJECT_SOURCE_DIR}/../shared/*.h"
    "${PROJECT_SOURCE_DIR}/../../vendor/*.h"
)

add_executable(server ${SERVER_FILES})

if (WIN32)
    target_link_libraries(server winmm psapi ws2_32)
elseif (UNIX)
    target_link_libraries(server m pthread dl)
endif()

target_include_directories(server PUBLIC
    ${PROJECT_SOURCE_DIR}/
    ${PROJECT_SOURCE_DIR}/../shared
    ${PROJECT_SOURCE_DIR}/../../vendor
)

if (MSVC)
    # set_target_properties(server PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    # set_target_properties(server PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")

    add_custom_command(TARGET server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/server.exe" "${CMAKE_SOURCE_DIR}/binary/server/m2o-server.exe"
    )

    GroupSourcesNamed(server ../server)
    GroupSourcesNamed(shared ../shared)
    GroupSourcesNamed(vendor ../../vendor)
endif()
