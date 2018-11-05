#include <host.h>
#include <windows.h>

#if 0

#include <vendor/http.h>

#define HTTP_IMPLEMENTATION

namespace Host
{
    void UpdateCheck()
    {
        auto req = http_get("http://www.mattiasgustavsson.com/http_test.txt", nullptr);

        if (!req)
        {
            MessageBoxW(nullptr, L"Failed to check for updates!", FXNAME_WIDE, MB_ICONWARNING);

            return;
        }

        http_status_t status = HTTP_STATUS_PENDING;
        int prev_size = -1;

        while (status == HTTP_STATUS_PENDING)
        {
            status = http_process(req);

            if (prev_size != (int)req->response_size)
            {
                prev_size = (int)req->response_size;
            }
        }

        // TODO : extract, etc

        if (status == HTTP_STATUS_FAILED)
        {
            http_release(req);

            return;
        }

        http_release(req);
    }
}

#endif
