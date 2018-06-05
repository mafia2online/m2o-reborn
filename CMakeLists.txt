cmake_minimum_required (VERSION 2.8)
project (m2o-reborn)

if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)")
endif()
if (CMAKE_SOURCE_DIR MATCHES " ")
    message("Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause.")
endif()
if (CMAKE_BINARY_DIR MATCHES " ")
    message("Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Zi" CACHE STRING "Debug compiler flags" FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /Zi" CACHE STRING "Release compiler flags" FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

add_subdirectory(projects/m2sdk)
add_subdirectory(projects/launcher)
add_subdirectory(projects/cef_worker)
add_subdirectory(projects/server)



if (WIN32)
    include(misc/cmake/FindDirectX.cmake)

    if (NOT DirectX_D3DX9_INCLUDE_FOUND)
        #message(FATAL_ERROR "Cannot find D3DX9 (DirectX 9 extenions). Make sure you installed DirectX SDK August 2010.")
    endif()

    find_path(D3DX9_INCLUDE d3dx9.h HINTS ${DirectX_INC_SEARCH_PATH} DOC "The directory where ${header} resides" CMAKE_FIND_ROOT_PATH_BOTH)
    find_path(D3DX9_LIBRARY d3dx9.lib HINTS ${DirectX_LIB_SEARCH_PATH} DOC "The directory where ${header} resides" CMAKE_FIND_ROOT_PATH_BOTH)
endif()


###
# Includes for libs and remote sources
###
include_directories(
    vendor/librg/include
    # vendor/steam_api/include
    vendor/detours/include
    vendor/nuklear/include
    vendor/discord_rpc/include
    vendor/cef/include
    vendor/cef
    projects/m2sdk/include
    projects/client/
    # projects/server/
    projects/shared/
    # projects/launcher/
    ${D3DX9_INCLUDE}
)

link_directories(
    ${PROJECT_SOURCE_DIR}/vendor/discord_rpc/${CMAKE_BUILD_TYPE}/
    ${PROJECT_SOURCE_DIR}/vendor/steam_api/lib
    ${PROJECT_SOURCE_DIR}/vendor/detours/lib
    ${PROJECT_SOURCE_DIR}/vendor/cef/${CMAKE_BUILD_TYPE}/
    ${D3DX9_LIBRARY}
)

# file(GLOB_RECURSE SERVER_FILES
#     "${PROJECT_SOURCE_DIR}/projects/server/*.cpp"
#     "${PROJECT_SOURCE_DIR}/projects/server/*.h"
#     "${PROJECT_SOURCE_DIR}/projects/shared/*.h"
# )

file(GLOB_RECURSE CLIENT_FILES
    "${PROJECT_SOURCE_DIR}/projects/client/*.cpp"
    "${PROJECT_SOURCE_DIR}/projects/client/*.h"
    "${PROJECT_SOURCE_DIR}/projects/client/*.hpp"
    "${PROJECT_SOURCE_DIR}/projects/shared/*.h"
    "${PROJECT_SOURCE_DIR}/projects/m2sdk/include/**.h*"
    "${PROJECT_SOURCE_DIR}/vendor/librg/include/*.h"
    "${PROJECT_SOURCE_DIR}/vendor/steam_api/include/*.h"
    "${PROJECT_SOURCE_DIR}/vendor/detours/include/*.h"
    "${PROJECT_SOURCE_DIR}/vendor/nuklear/include/*.h"
    "${PROJECT_SOURCE_DIR}/vendor/discord_rpc/include/*.h"
    "${PROJECT_SOURCE_DIR}/vendor/cef/include/*.h"
)


# set(SERVER_LIBRARIES)
# set(LAUNCHER_LIBRARIES psapi steam_api)
set(CLIENT_LIBRARIES
    ws2_32
    winmm
    d3d9
    d3dx9
    dxgi
    m2sdk
    dinput8
    dxguid
    discord-rpc
    libcef
    libcef_dll_wrapper
)

# if (WIN32)
#     set(SERVER_LIBRARIES ${SERVER_LIBRARIES} ws2_32 winmm)
# elseif (UNIX)
#     set(SERVER_LIBRARIES ${SERVER_LIBRARIES} m pthread dl)
# endif()

###
# Adding executables
# Linking libraries
###
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(SERVER_LIBRARIES ${SERVER_LIBRARIES} winmm psapi ws2_32)

    ## tha clienta
    add_library(client SHARED ${CLIENT_FILES})
    target_link_libraries(client winmm psapi ws2_32 ${CLIENT_LIBRARIES})
    set_target_properties(client PROPERTIES LINK_FLAGS_DEBUG "/SAFESEH:NO")
    set_target_properties(client PROPERTIES LINK_FLAGS_RELEASE "/SAFESEH:NO")

    # ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_TARGET}" "exe")

    # ## tha lancunha
    # add_executable(launcher "${PROJECT_SOURCE_DIR}/projects/launcher/launcher.cpp")
    # target_link_libraries(launcher ${LAUNCHER_LIBRARIES})
    # set_target_properties(launcher PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    # set_target_properties(launcher PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")

    # add_executable(cef_worker "${PROJECT_SOURCE_DIR}/projects/cef_worker/cef_worker.cpp")
    # target_link_libraries(cef_worker libcef libcef_dll_wrapper)

    # set_target_properties(cef_worker PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    # set_target_properties(cef_worker PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    # set_target_properties(cef_worker PROPERTIES LINK_FLAGS "/ignore:4099")
endif()

# ## tha servera
# add_executable(server ${SERVER_FILES})
# target_link_libraries(server ${SERVER_LIBRARIES})

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # if (WIN32)
    #     set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
    # endif()

    add_custom_command(TARGET client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/client.dll" "${CMAKE_CURRENT_SOURCE_DIR}/binary/"
    )

    # add_custom_command(TARGET cef_worker POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/cef_worker.exe" "${CMAKE_CURRENT_SOURCE_DIR}/binary/"
    # )

    # add_custom_command(TARGET launcher POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/launcher.exe" "${CMAKE_CURRENT_SOURCE_DIR}/binary/"
    # )

    add_custom_command(
        TARGET client
        POST_BUILD
        COMMAND "mt.exe" -manifest "${CMAKE_CURRENT_SOURCE_DIR}\\projects\\client\\manifest.xml" -outputresource:"${CMAKE_CURRENT_SOURCE_DIR}/binary/client.dll"\;\#2
        COMMENT "Adding manifest..."
    )


endif()

# add_custom_command(TARGET server POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/server${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/binary/"
# )

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
    GroupSources(projects/)
    GroupSources(vendor/)
endif()

