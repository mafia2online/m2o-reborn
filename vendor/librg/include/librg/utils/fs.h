// Copyright ReGuider Team, 2016-2017
//
#ifndef librg_utils_fs
#define librg_utils_fs

#include <uv.h>
#include <string>
#include <functional>

namespace librg
{
    
    // TODO: Move docs from cpp here.
    namespace fs
    {
        struct fs_result_t {
            int handle;
            uint64_t length;
            uint64_t fplength;
            char* content;
            char* filepath;
            std::function<void(fs_result_t*)> callback;
        };

        void private_onread(uv_fs_t *req);

#ifdef WIN32
        static char separator = '\\';
#else
        static char separator = '/';
#endif
        
        using callback = std::function<void(fs_result_t*)>;
        using result_t = fs_result_t;
        using byte     = char;
        
        // async
        
        /**
        * read file contents and pass result
        * of type fs:result_t to the callback
        *
        * @param  filename
        * @param  callback
        * @return result of operation
        */
        bool read(std::string filename, callback callback);
        
        /**
        * write file contents and pass result
        * of type fs:result_t to the callback
        *
        * @note   Caller frees data
        * @param  dataSize
        * @param  data
        * @param  filename
        * @param  callback
        * @return result of operation
        */
        bool write(std::string filename, size_t dataSize, byte *data, callback callback);

        // sync
        
        /**
        * Create directory
        *
        * @param  filename
        * @return result of operation
        */
        int mkdir(std::string filename);
        
        
        /**
        * Remove directory
        *
        * @param  filename
        * @return result of operation
        */
        int rmdir(std::string filename);

        // ugly
        using _s = std::string;
        static char _p = separator;
        static inline _s path(_s a1, _s a2) {return a1+_p+a2;}
        static inline _s path(_s a1, _s a2, _s a3) {return a1+_p+a2+_p+a3;}
        static inline _s path(_s a1, _s a2, _s a3, _s a4) {return a1+_p+a2+_p+a3+_p+a4;}
        static inline _s path(_s a1, _s a2, _s a3, _s a4, _s a5) {return a1+_p+a2+_p+a3+_p+a4+_p+a5;}
        static inline _s path(_s a1, _s a2, _s a3, _s a4, _s a5, _s a6) {return a1+_p+a2+_p+a3+_p+a4+_p+a5+_p+a6;}
        static inline _s path(_s a1, _s a2, _s a3, _s a4, _s a5, _s a6, _s a7) {return a1+_p+a2+_p+a3+_p+a4+_p+a5+_p+a6+_p+a7;}
        static inline _s path(_s a1, _s a2, _s a3, _s a4, _s a5, _s a6, _s a7, _s a8) {return a1+_p+a2+_p+a3+_p+a4+_p+a5+_p+a6+_p+a7+_p+a8;}
    }
}

#endif //librg_utils_fs
