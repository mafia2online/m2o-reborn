###
# CMake entry point
###
cmake_minimum_required (VERSION 2.8)
project (m2o-reborn)


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
    set(CMAKE_CXX_FLAGS_DEBUG "/MDd" CACHE STRING "Debug compiler flags" FORCE)
endif()


###
# Compile vendor dependencies
###
add_subdirectory(vendor/librg)

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
)

link_directories(
    ${PROJECT_SOURCE_DIR}/vendor/steam_api/lib
    ${PROJECT_SOURCE_DIR}/vendor/detours/lib
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

###
# Adding executables
# Linking libraries
###
add_executable(server ${SERVER_FILES})
target_link_libraries(server librg)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

    ## tha clienta
    add_library(client SHARED ${CLIENT_FILES})
    target_link_libraries(client librg d3d9 d3dx9 dxgi dinput8 dxguid detours)
    set_target_properties(client PROPERTIES LINK_FLAGS_DEBUG "/SAFESEH:NO")

    ## tha lancunha
    add_executable(launcher "${PROJECT_SOURCE_DIR}/launcher/launcher.cpp")
    target_link_libraries(launcher Psapi steam_api)
    set_target_properties(launcher PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")

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
