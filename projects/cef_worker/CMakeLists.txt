cmake_minimum_required(VERSION 2.8)
project(cef_worker)

link_directories(
    ${PROJECT_SOURCE_DIR}/../../vendor/cef/${CMAKE_BUILD_TYPE}
)

add_executable(cef_worker "${PROJECT_SOURCE_DIR}/cef_worker.cpp")

target_include_directories(cef_worker PUBLIC
    ${PROJECT_SOURCE_DIR}/../../vendor/cef
    ${PROJECT_SOURCE_DIR}/../../vendor/cef/include
)

target_link_libraries(cef_worker libcef libcef_dll_wrapper)

if (MSVC)
    set_target_properties(cef_worker PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    set_target_properties(cef_worker PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(cef_worker PROPERTIES LINK_FLAGS "/ignore:4099")

    add_custom_command(TARGET cef_worker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/cef_worker.exe" "${CMAKE_SOURCE_DIR}/binary/"
    )

    add_custom_command(
        TARGET cef_worker
        POST_BUILD
        COMMAND "mt.exe" -manifest "${PROJECT_SOURCE_DIR}\\manifest.xml" -outputresource:"${CMAKE_SOURCE_DIR}/binary/cef_worker.exe"\;\#1
        COMMENT "Adding manifest..."
    )
endif()
