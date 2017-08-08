###
# CMake entry point
###
cmake_minimum_required (VERSION 2.8)
project (ReGuider)

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

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

###
# Custom building flags
###
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


## trying to disable
set(ENTITYX_BUILD_TESTING false CACHE BOOL "Enable building of tests.")
set(ENTITYX_RUN_BENCHMARKS false CACHE BOOL "Run benchmarks (in conjunction with -DENTITYX_BUILD_TESTING=1).")
set(ENTITYX_MAX_COMPONENTS 64 CACHE STRING "Set the maximum number of components.")
set(ENTITYX_DT_TYPE double CACHE STRING "The type used for delta time in EntityX update methods.")
set(ENTITYX_BUILD_SHARED false CACHE BOOL "Build shared libraries?")

###
# Compile vendor dependencies
###
add_subdirectory(vendor/lib-enet)
add_subdirectory(vendor/lib-libuv-cmake)
add_subdirectory(vendor/lib-squirrel)
add_subdirectory(vendor/lib-sqrat)
add_subdirectory(vendor/lib-tinyxml2)
add_subdirectory(vendor/lib-entityx)

# TODO: fix later for beautiful c++17 code
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-delete-incomplete")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
endif()

###
# Hacks
# On Visual 2005 and above, this module can set the debug working directory
###
#cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/vendor/rpavlik-cmake-modules-fe2273")
# include(CreateLaunchers)
# include(MSVCMultipleProcessCompile) # /MP
set(CMAKE_MACOSX_RPATH 1)

###
# Includes for libs and remote sources
###
include_directories(
    include/
)

file(GLOB_RECURSE LIBRG_FILES
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
  "${PROJECT_SOURCE_DIR}/include/*.h"
  "${PROJECT_SOURCE_DIR}/include/*.hpp"
)

###
# Includes for current source executable
###
add_library(librg STATIC
    ${LIBRG_FILES}
)

macro(GroupSources curdir)
    file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)
    foreach (child ${children})
        if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
            GroupSources(${curdir}/${child})
        else()
            string(REPLACE "/" "\\" groupname ${curdir})
            source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
        endif()
    endforeach()
endmacro()

if(MSVC)
    GroupSources(src)
    GroupSources(include)
endif()

###
# Linking libraries
###
target_link_libraries(librg
    enet
    squirrel
    sqstdlib
    tinyxml2
    entityx
    uv
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(librg
        IPHLPAPI
        psapi
        shell32
        advapi32
        ws2_32
        winmm
        userenv
    )
endif()

target_include_directories(librg PUBLIC
    vendor/lib-enet/include
    vendor/lib-squirrel/include
    vendor/lib-libuv-cmake/libuv/include
    vendor/lib-math/include
    vendor/lib-sqrat/include
    vendor/lib-tinyxml2
    vendor/lib-entityx
)

file(GLOB_RECURSE TEST_FILES "${PROJECT_SOURCE_DIR}/test/*.cpp")
add_executable(rgtest ${TEST_FILES})

target_link_libraries(rgtest librg)

set_target_properties(enet              PROPERTIES FOLDER librg/vendors)
set_target_properties(squirrel          PROPERTIES FOLDER librg/vendors)
set_target_properties(sqstdlib          PROPERTIES FOLDER librg/vendors)
set_target_properties(tinyxml2          PROPERTIES FOLDER librg/vendors)
set_target_properties(entityx           PROPERTIES FOLDER librg/vendors)
set_target_properties(uv                PROPERTIES FOLDER librg/vendors)
set_target_properties(librg             PROPERTIES FOLDER librg)
set_target_properties(rgtest            PROPERTIES FOLDER librg)


# Xcode and Visual working directories
# set_target_properties(librg PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/librg/")
# create_target_launcher(librg WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/librg/")


# SOURCE_GROUP(shared REGULAR_EXPRESSION ".*/shared/.*" )

# if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
# add_custom_command(
#    TARGET server POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/server${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/server/"
# )

# elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

# endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
