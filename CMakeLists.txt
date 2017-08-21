###
# CMake entry point
###
cmake_minimum_required (VERSION 2.8)
project (m2o-reborn)

include(misc/cmake/FindDirectX.cmake)

# set(WIN32 ${CMAKE_SYSTEM_NAME} MATCHES "Windows")

###
# Predefines to block make at the root
###
if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
    message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
    message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Zi" CACHE STRING "Debug compiler flags" FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /Zi" CACHE STRING "Release compiler flags" FORCE)
endif()

set(CMAKE_BUILD_TYPE "Debug")

###
# Compile vendor dependencies
###
add_subdirectory(vendor/librg)

if (WIN32)
    if (NOT DirectX_D3DX9_INCLUDE_FOUND)
        #message(FATAL_ERROR "Cannot find D3DX9 (DirectX 9 extenions). Make sure you installed DirectX SDK August 2010.")
    endif()

    find_path (D3DX9_INCLUDE d3dx9.h HINTS ${DirectX_INC_SEARCH_PATH} DOC "The directory where ${header} resides" CMAKE_FIND_ROOT_PATH_BOTH)
    find_path (D3DX9_LIBRARY d3dx9.lib HINTS ${DirectX_LIB_SEARCH_PATH} DOC "The directory where ${header} resides" CMAKE_FIND_ROOT_PATH_BOTH)
endif()


###
# Includes for libs and remote sources
###
include_directories(
    vendor/steam_api/include
    vendor/detours/include
    vendor/librg/include
    vendor/m2sdk/include
    vendor/nuklear/include
    server/
    shared/
    launcher/
    ${D3DX9_INCLUDE}
)

link_directories(
    ${PROJECT_SOURCE_DIR}/vendor/steam_api/lib
    ${PROJECT_SOURCE_DIR}/vendor/detours/lib
    ${D3DX9_LIBRARY}
)

file(GLOB_RECURSE SERVER_FILES
    "${PROJECT_SOURCE_DIR}/server/*.cpp"
    "${PROJECT_SOURCE_DIR}/server/*.h"
    "${PROJECT_SOURCE_DIR}/shared/*.h"
)

file(GLOB_RECURSE CLIENT_FILES
    "${PROJECT_SOURCE_DIR}/client/*.cpp"
    "${PROJECT_SOURCE_DIR}/client/*.h"
    "${PROJECT_SOURCE_DIR}/client/*.hpp"
    "${PROJECT_SOURCE_DIR}/shared/*.h"
    "${PROJECT_SOURCE_DIR}/vendor/steam_api/include/*.h"
    "${PROJECT_SOURCE_DIR}/vendor/detours/include/*.h"
    "${PROJECT_SOURCE_DIR}/vendor/m2sdk/include/*.h*"
    "${PROJECT_SOURCE_DIR}/vendor/nuklear/include/*.h"
)

set(SERVER_LIBRARIES librg)
set(CLIENT_LIBRARIES librg d3d9 d3dx9 dxgi dinput8 dxguid detours)
set(LAUNCHER_LIBRARIES Psapi steam_api)

###
# Adding executables
# Linking libraries
###
add_executable(server ${SERVER_FILES})
target_link_libraries(server ${SERVER_LIBRARIES})

add_custom_command(TARGET server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/server${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/binary/"
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

    ## tha clienta
    add_library(client SHARED ${CLIENT_FILES})
    target_link_libraries(client ${CLIENT_LIBRARIES})
    set_target_properties(client PROPERTIES LINK_FLAGS_DEBUG "/SAFESEH:NO")
    set_target_properties(client PROPERTIES LINK_FLAGS_RELEASE "/SAFESEH:NO")

    ## tha lancunha
    add_executable(launcher "${PROJECT_SOURCE_DIR}/launcher/launcher.cpp")
    target_link_libraries(launcher ${LAUNCHER_LIBRARIES})
    set_target_properties(launcher PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    set_target_properties(launcher PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")

    add_custom_command(TARGET client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/client.dll" "${CMAKE_CURRENT_SOURCE_DIR}/binary/"
    )

    add_custom_command(TARGET launcher POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/launcher.exe" "${CMAKE_CURRENT_SOURCE_DIR}/binary/"
    )
endif()

if (MSVC)
    GroupSources(client)
    GroupSources(server)
    GroupSources(shared)
    GroupSources(vendor/steam_api)
    GroupSources(vendor/detours)
    GroupSources(vendor/m2sdk)
    GroupSources(vendor/nuklear)
    GroupSources(launcher)
endif()

