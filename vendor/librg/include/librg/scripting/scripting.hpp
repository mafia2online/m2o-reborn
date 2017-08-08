// Copyright ReGuider Team, 2016-2017
//
#ifndef _scripting
#define _scripting

#include <squirrel.h>
#include <sqrat.h>
#include <sqstdio.h>
#include <sqstdblob.h>
#include <sqstdmath.h>
#include <sqstdsystem.h>
#include <sqstdstring.h>

#include <librg/core.h>
#include <librg/scripting/registry.hpp>

namespace librg
{
    namespace scripting
    {
        #ifdef SQUNICODE
        #define scvprintf vwprintf
        #else
        #define scvprintf vprintf
        #endif

        constexpr unsigned char LIB_IO   = 0x01;                                              ///< Input/Output library
        constexpr unsigned char LIB_BLOB = 0x02;                                              ///< Blob library
        constexpr unsigned char LIB_MATH = 0x04;                                              ///< Math library
        constexpr unsigned char LIB_SYST = 0x08;                                              ///< System library
        constexpr unsigned char LIB_STR  = 0x10;                                              ///< String library
        constexpr unsigned char LIB_ALL  = LIB_IO | LIB_BLOB | LIB_MATH | LIB_SYST | LIB_STR; ///< All libraries

        /**
         * Main print handler method
         * Uses default output stream to print information
         */
        inline static void printHandler(HSQUIRRELVM /*v*/, const SQChar *s, ...)
        {
            va_list args;
            char szBuffer[512];
            va_start(args, s);
            vsnprintf(szBuffer, sizeof(szBuffer), s, args);
            va_end(args);

            core::log(szBuffer);
        }

        /**
         * Main error message handler method
         * Uses default output stream to print information
         */
        inline static void errorHandler(HSQUIRRELVM /*v*/, const SQChar *szFormat, ...)
        {
            va_list args;
            char szBuffer[512];
            va_start( args, szFormat );
            vsnprintf( szBuffer, sizeof(szBuffer), szFormat, args );
            va_end( args );

            char * tmp = (char *)szBuffer;
            size_t offstart = 0, offend = 0;
            size_t len = strlen( tmp );

            // special hnalders for \n\r\t sequences
            for( size_t i = 0; i < len; ++i ) {
                switch (tmp[i]) { case ' ': case '\r': case '\n': case '\t': ++offstart; break; default: i = len - 1; break; }
            }

            tmp += offstart;
            len -= offstart;

            // same
            for (size_t i = len - 1; i > 0; --i) {
                switch (tmp[i]) { case ' ': case '\r': case '\n': case '\t': ++offend; break; default: i = 1; break; }
            }

            tmp[ len - offend ] = '\0';
            core::log(tmp);
        }

        /**
         * Main compile error handler
         * Uses default output stream to print information
         */
        inline static void compilerErrorHandler(HSQUIRRELVM v, const SQChar* desc, const SQChar* source, SQInteger line, SQInteger column)
        {
            core::error("Squirrel compile error: %s on file %s (line %d, column %d)", (const char *)desc, (const char *)source, line, column);
        }

        /**
         * Main public method of the vm registration
         * Should be used as entry point for any vm to register all default needed stuff
         * @param vm you want to register to
         * @param libsToLoad std sq libraries which supposed to be loaded alongside with vm
         */
        inline static void install(HSQUIRRELVM vm, unsigned char libsToLoad = LIB_ALL)
        {
            sq_pushroottable(vm);
            if (libsToLoad & LIB_IO)
                sqstd_register_iolib(vm);
            if (libsToLoad & LIB_BLOB)
                sqstd_register_bloblib(vm);
            if (libsToLoad & LIB_MATH)
                sqstd_register_mathlib(vm);
            if (libsToLoad & LIB_SYST)
                sqstd_register_systemlib(vm);
            if (libsToLoad & LIB_STR)
                sqstd_register_stringlib(vm);
            sq_pop(vm, 1);

            // registering stuff
            sqstd_seterrorhandlers(vm);
            sq_setprintfunc(vm, printHandler, errorHandler);
            sq_setcompilererrorhandler(vm, compilerErrorHandler);

            // create our main natives table
            Sqrat::Table nativeTable(vm);

            // install our bindings
            registry_install(nativeTable);

            // push table into the vm
            Sqrat::RootTable(vm).Bind("native", nativeTable);
        }
    }
}

#endif // _scripting
