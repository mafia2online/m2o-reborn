cmake_minimum_required(VERSION 2.8)
project(client)

file(GLOB_RECURSE FILES
    "${PROJECT_SOURCE_DIR}/*.cpp"
    "${PROJECT_SOURCE_DIR}/*.h*"
    "${PROJECT_SOURCE_DIR}/../shared/*.h"
    "${PROJECT_SOURCE_DIR}/../../vendor/sdl/include/*.h"
    "${PROJECT_SOURCE_DIR}/../../vendor/librg/include/*.h"
    "${PROJECT_SOURCE_DIR}/../../vendor/steam_api/include/*.h"
    "${PROJECT_SOURCE_DIR}/../../vendor/detours/include/*.h"
    "${PROJECT_SOURCE_DIR}/../../vendor/nuklear/include/*.h"
    "${PROJECT_SOURCE_DIR}/../../vendor/discord/include/*.h"
)

set(LIBRARIES m2sdk discord-rpc SDL2)
set(LIBRARY_FOLDERS
    ${PROJECT_SOURCE_DIR}/../../vendor/discord/${CMAKE_BUILD_TYPE}/
    ${PROJECT_SOURCE_DIR}/../../vendor/sdl/${CMAKE_BUILD_TYPE}/
)
set(INCLUDE_FOLDERS
    ${PROJECT_SOURCE_DIR}/
    ${PROJECT_SOURCE_DIR}/../shared
    ${PROJECT_SOURCE_DIR}/../m2sdk/include
    ${PROJECT_SOURCE_DIR}/../../vendor/sdl/include
    ${PROJECT_SOURCE_DIR}/../../vendor/librg/include
    ${PROJECT_SOURCE_DIR}/../../vendor/nuklear/include
    ${PROJECT_SOURCE_DIR}/../../vendor/detours/include
    ${PROJECT_SOURCE_DIR}/../../vendor/discord/include
)

if (WIN32)
    include(../../misc/cmake/FindDirectX.cmake)

    find_path(D3DX9_INCLUDE d3dx9.h HINTS ${DirectX_INC_SEARCH_PATH} DOC "The directory where ${header} resides" CMAKE_FIND_ROOT_PATH_BOTH)
    find_path(D3DX9_LIBRARY d3dx9.lib HINTS ${DirectX_LIB_SEARCH_PATH} DOC "The directory where ${header} resides" CMAKE_FIND_ROOT_PATH_BOTH)

    set(INCLUDE_FOLDERS ${INCLUDE_FOLDERS} ${D3DX9_INCLUDE})
    set(LIBRARY_FOLDERS ${LIBRARY_FOLDERS}
        ${D3DX9_LIBRARY}
        ${PROJECT_SOURCE_DIR}/../../vendor/detours/lib/
    )
    set(LIBRARIES ${LIBRARIES}
        ws2_32  # for librg
        psapi   # for librg
        winmm   # for librg
        d3d9
        d3dx9
        dxgi
        dinput8
        dxguid
        imm32   # for sdl
        version # for sdl
    )
elseif (UNIX)
    set(LIBRARIES ${LIBRARIES} m pthread dl)
endif()

if (M2O_CEF)
    message("-- M2O_CEF enabled, linking with CEF ...")

    set(LIBRARY_FOLDERS ${LIBRARY_FOLDERS} ${PROJECT_SOURCE_DIR}/../../vendor/cef/${CMAKE_BUILD_TYPE}/)
    set(INCLUDE_FOLDERS ${INCLUDE_FOLDERS}
        ${PROJECT_SOURCE_DIR}/../../vendor/cef
        ${PROJECT_SOURCE_DIR}/../../vendor/cef/include
    )
endif()

link_directories(${LIBRARY_FOLDERS})
add_library(client SHARED ${FILES})
target_link_libraries(client ${LIBRARIES})
target_include_directories(client PUBLIC ${INCLUDE_FOLDERS})

if (MSVC)
    set_target_properties(client PROPERTIES LINK_FLAGS_DEBUG "/SAFESEH:NO")
    set_target_properties(client PROPERTIES LINK_FLAGS_RELEASE "/SAFESEH:NO")

    add_custom_command(TARGET client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/client.dll" "${CMAKE_SOURCE_DIR}/binary/"
    )

    add_custom_command(
        TARGET client
        POST_BUILD
        COMMAND "mt.exe" -manifest "${PROJECT_SOURCE_DIR}\\manifest.xml" -outputresource:"${CMAKE_SOURCE_DIR}/binary/client.dll"\;\#2
        COMMENT "Adding manifest..."
    )

    GroupSourcesNamed(client ../client)
    GroupSourcesNamed(shared ../shared)
    GroupSourcesNamed(vendor ../../vendor)
endif()
