###
# CMake entry point
###
cmake_minimum_required (VERSION 2.8)
project (m2o)


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

###
# Compile vendor dependencies
###
add_subdirectory(vendor/librg)

###
# Includes for libs and remote sources
###
include_directories(
    vendor/librg/include
    vendor/M2SDK/include
    vendor/SteamAPI
    server/
    shared/
    launcher/
)


link_directories(
    # vendor/M2SDK/include
    ${PROJECT_SOURCE_DIR}/vendor/SteamAPI/lib
    ${PROJECT_SOURCE_DIR}/vendor/detours
)


if(MSVC)
    GroupSources(client)
    GroupSources(server)
    GroupSources(shared)
    GroupSources(launcher)
endif()

###
# Adding executables
# Linking libraries
###
add_executable(server "${PROJECT_SOURCE_DIR}/server/server.cpp")
target_link_libraries(server librg)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

    ## tha clienta
    add_library(client SHARED "${PROJECT_SOURCE_DIR}/client/client.cpp")
    target_link_libraries(client librg d3dx9 dxgi detours)

    ## tha lancunha
    add_executable(launcher "${PROJECT_SOURCE_DIR}/launcher/launcher.cpp")
    target_link_libraries(launcher Psapi steam_api)
    set_target_properties(launcher PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")

endif()
