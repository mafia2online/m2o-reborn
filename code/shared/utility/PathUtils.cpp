

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <string.h>
#endif

#include <utility/PathUtils.h>

namespace Utility
{
#ifdef _WIN32
    std::wstring MakeAbsolutePathW(const std::wstring &rel_to)
    {
        static std::wstring path;

        if (path.empty())
        {
            wchar_t buf[MAX_PATH] = { 0 };
            GetModuleFileNameW(nullptr, buf, MAX_PATH);

            //append string terminator
            for (size_t i = wcslen(buf); i > 0; --i)
            {
                if (buf[i] == '\\')
                {
                    buf[i + 1] = 0;
                    break;
                }
            }

            path = std::wstring(buf);
        }

        return path + rel_to;
    }

    std::string MakeAbsolutePathA(const std::string &rel_to)
    {
        static std::string path;

        if (path.empty())
        {
            char buf[MAX_PATH] = { 0 };
            GetModuleFileNameA(nullptr, buf, MAX_PATH);

            //append string terminator
            for (size_t i = strlen(buf); i > 0; --i)
            {
                if (buf[i] == '\\')
                {
                    buf[i + 1] = 0;
                    break;
                }
            }

            path = std::string(buf);
        }

        return path + rel_to;
    }
#else

    std::wstring GetAbsolutePathW(const std::wstring &rel_to_unused)
    {
        (void)rel_to_unused;

        static std::wstring empty = L"";
        return empty;
    }

    std::string MakeAbsolutePathA(const std::string &rel_to)
    {
        static std::string path;

        if (path.empty())
        {
            char buf[512];
            (void)readlink("/proc/self/exe", buf, sizeof(buf));

            char* dir_ptr = strrchr(buf, '/');

            dir_ptr[1] = '\0';

            path = std::string(buf);
        }

        return path + rel_to;
    }

#endif
}
