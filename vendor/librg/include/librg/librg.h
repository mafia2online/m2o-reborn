// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_h
#define librg_h

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4312)
#pragma warning(disable:4577)
#endif

#if WIN32
// #include <windows.h>
inline void uv_sleep(int milis) {
    // Sleep(milis);
}
#else
#include <unistd.h>
inline void uv_sleep(int milis) {
    usleep(milis * 1000);
}
#endif

#define LIBRG_VERSION_MAJOR 1
#define LIBRG_VERSION_MINOR 0
#define LIBRG_VERSION_PATCH 0
#define LIBRG_VERSION_CREATE(major, minor, patch) (((major)<<16) | ((minor)<<8) | (patch))
#define LIBRG_VERSION_GET_MAJOR(version) (((version)>>16)&0xFF)
#define LIBRG_VERSION_GET_MINOR(version) (((version)>>8)&0xFF)
#define LIBRG_VERSION_GET_PATCH(version) ((version)&0xFF)
#define LIBRG_VERSION LIBRG_VERSION_CREATE(LIBRG_VERSION_MAJOR, LIBRG_VERSION_MINOR, LIBRG_VERSION_PATCH)

/**
 * All includes
 */
#include <librg/core.h>
#include <librg/events.h>
#include <librg/entities.h>
#include <librg/network.h>
#include <librg/resources.h>
#include <librg/streamer.h>

#include <librg/components/streamable.h>
#include <librg/components/interpolable.h>

#endif // librg_h
