cmake_minimum_required(VERSION 2.8)
project(client)

file(GLOB_RECURSE FILES
    "${PROJECT_SOURCE_DIR}/*.cpp"
    "${PROJECT_SOURCE_DIR}/*.h*"
    "${PROJECT_SOURCE_DIR}/../shared/*.h"
    "${PROJECT_SOURCE_DIR}/../../vendor/*.h"
    "${PROJECT_SOURCE_DIR}/../../vendor/sdl/include/*.h"
    "${PROJECT_SOURCE_DIR}/../../vendor/steam_api/include/*.h"
    "${PROJECT_SOURCE_DIR}/../../vendor/discord/include/*.h"
    "${PROJECT_SOURCE_DIR}/../../vendor/m2sdk/include/*.h*"
)

set(LIBRARIES)
set(LIBRARY_FOLDERS
    ${PROJECT_SOURCE_DIR}/../../vendor/discord/
    ${PROJECT_SOURCE_DIR}/../../vendor/m2sdk/lib/
    ${PROJECT_SOURCE_DIR}/../../vendor/sdl/
)
set(INCLUDE_FOLDERS
    ${PROJECT_SOURCE_DIR}/
    ${PROJECT_SOURCE_DIR}/../shared
    ${PROJECT_SOURCE_DIR}/../../vendor
    ${PROJECT_SOURCE_DIR}/../../vendor/sdl/include
    ${PROJECT_SOURCE_DIR}/../../vendor/m2sdk/include
    ${PROJECT_SOURCE_DIR}/../../vendor/detours/include
    ${PROJECT_SOURCE_DIR}/../../vendor/discord/include
)

if (WIN32)
    set(INCLUDE_FOLDERS ${INCLUDE_FOLDERS} ${PROJECT_SOURCE_DIR}/../../vendor/d3d9/include/)
    set(LIBRARY_FOLDERS ${LIBRARY_FOLDERS}
        ${PROJECT_SOURCE_DIR}/../../vendor/d3d9/
        ${PROJECT_SOURCE_DIR}/../../vendor/detours/lib/
    )
    set(LIBRARIES ${LIBRARIES}
        ws2_32  # for librg
        psapi   # for librg
        winmm   # for librg

        d3dx9
        dxgi
        dinput8
        dxguid

        m2sdk
        detours
        discord-rpc

        SDL2
        SDL2_ttf
        libfreetype-6
        imm32   # for sdl
        version # for sdl
    )
else()
    message("-- Can't compile on non-win32 platform")
endif()

if (M2O_CEF)
    message("-- M2O_CEF enabled, linking with CEF ...")

    set(LIBRARY_FOLDERS ${LIBRARY_FOLDERS} ${PROJECT_SOURCE_DIR}/../../vendor/cef/)
    set(INCLUDE_FOLDERS ${INCLUDE_FOLDERS}
        ${PROJECT_SOURCE_DIR}/../../vendor/cef
        ${PROJECT_SOURCE_DIR}/../../vendor/cef/include
    )

    set(LIBRARIES ${LIBRARIES} libcef libcef_dll_wrapper)
endif()

link_directories(${LIBRARY_FOLDERS})
add_library(client SHARED ${FILES})
target_link_libraries(client ${LIBRARIES})
target_include_directories(client PUBLIC ${INCLUDE_FOLDERS})

if (MSVC)
    set_target_properties(client PROPERTIES LINK_FLAGS_DEBUG "/SAFESEH:NO")
    set_target_properties(client PROPERTIES LINK_FLAGS_RELEASE "/SAFESEH:NO")

    add_custom_command(TARGET client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/client.dll" "${CMAKE_SOURCE_DIR}/binary/client/m2o-client.dll"
    )

    add_custom_command(TARGET client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/client.pdb" "${CMAKE_SOURCE_DIR}/binary/other/"
    )

    add_custom_command(
        TARGET client
        POST_BUILD
        COMMAND "mt.exe" -manifest "${PROJECT_SOURCE_DIR}\\manifest.xml" -outputresource:"${CMAKE_SOURCE_DIR}/binary/client/m2o-client.dll"\;\#2
        COMMENT "Adding manifest..."
    )

    GroupSourcesNamed(client ../client)
    GroupSourcesNamed(shared ../shared)
    GroupSourcesNamed(vendor ../../vendor)
endif()
