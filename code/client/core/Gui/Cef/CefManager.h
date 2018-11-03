#pragma once

#include <list>

#include <Gui/Cef/CefFrame.h>
#include <Gui/Cef/CefAppImpl.h>

namespace nmd::gui
{
    class CefManager
    {
        CefRefPtr<CefApp> app;

        std::list<std::shared_ptr<CefFrame>> frames;

    public:

        bool Initialize();
        void Shutdown();

        // frame
        std::weak_ptr<CefFrame> AddFrame(const std::string &url, const CefFrameInfo &info);
        void RemoveFrame(std::weak_ptr<CefFrame> frame);

        void DoFrames();
    };
}

