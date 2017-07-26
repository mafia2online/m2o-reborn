###
# CMake entry point
###
cmake_minimum_required (VERSION 2.8)
project (urgmp)


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

set(SDL_SHARED_ENABLED_BY_DEFAULT OFF CACHE BOOL "Generate the DLL project if true.")

###
# Compile vendor dependencies
###
add_subdirectory(vendor/reguider)

###
# Includes for libs and remote sources
###
include_directories(
    vendor/reguider/include
    client/hooking/MinHook/include
    server/
    shared/
)

###
# Includes for current source executable
###
file(GLOB_RECURSE SERVER_FILES "${PROJECT_SOURCE_DIR}/server/*.cpp")

file(GLOB_RECURSE CLIENT_FILES
  "${PROJECT_SOURCE_DIR}/client/*.cpp"
  "${PROJECT_SOURCE_DIR}/client/*.c"
  "${PROJECT_SOURCE_DIR}/client/*.h"
  "${PROJECT_SOURCE_DIR}/client/*.hpp"
)

if(MSVC)
    GroupSources(client)
    GroupSources(server)
    GroupSources(shared)
endif()

###
# Adding executables
###
add_executable(server ${SERVER_FILES})
add_library(client SHARED ${CLIENT_FILES})

###
# Linking libraries
###
target_link_libraries(server librg)
target_link_libraries(client librg d3d11 dxgi)
