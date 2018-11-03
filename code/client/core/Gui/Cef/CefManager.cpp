
#include <Utility/PathUtils.h>
#include <Gui/Cef/CefManager.h>

#include <delayimp.h>

namespace nmd::gui
{
    bool CefManager::Initialize()
    {
        LoadLibraryW(Utility::MakeAbsolutePathW(L"bin/chrome_elf.dll").c_str());
        LoadLibraryW(Utility::MakeAbsolutePathW(L"bin/libEGL.dll").c_str());
        LoadLibraryW(Utility::MakeAbsolutePathW(L"bin/libGLESv2.dll").c_str());

        HMODULE libcef = LoadLibraryW(Utility::MakeAbsolutePathW(L"bin/libcef.dll").c_str());

        if (!libcef)
        {
            MessageBoxW(nullptr, L"Failed to initialize libcef!", FXNAME_WIDE, MB_ICONERROR);

            return false;
        }

        // Delay-Load CEF (path allready mounted)
        __HrLoadAllImportsForDll("libcef.dll");

        const CefMainArgs args;
        app = new CefAppImpl(*this);

        // CEF Process
        const auto exitCode = CefExecuteProcess(args, app, nullptr);
        if (exitCode >= 0)
        {
            ExitProcess(0);
            return false;
        }

        // CEF Settings
        CefSettings cefSettings;
        cefSettings.no_sandbox = true;
        cefSettings.multi_threaded_message_loop = true;
        cefSettings.remote_debugging_port = 13172; // config
        cefSettings.pack_loading_disabled = false;
        cefSettings.windowless_rendering_enabled = true;

        // Locale
        CefString(&cefSettings.locale)
            .FromString("en-US");

        // custom cef path mounts
#if 0
        CefString(&cefSettings.cache_path).FromWString(app_dir + L"\\cache\\cef");
        CefString(&cefSettings.log_file).FromWString(app_dir + L"\\logs\\cef.log");
        CefString(&cefSettings.browser_subprocess_path).FromWString(app_dir + L"\\cefapp.exe");
        CefString(&cefSettings.locales_dir_path).FromWString(app_dir + L"\\data\\locales");
#endif
        return true;
    }

    void CefManager::Shutdown()
    {
        frames.clear();
        CefShutdown();
    }

    std::weak_ptr<CefFrame> CefManager::AddFrame(const std::string & url, const CefFrameInfo &info)
    {
        auto frame = std::make_shared<CefFrame>(url, info);

        frames.push_back(frame);

        return frame;
    }

    void CefManager::RemoveFrame(std::weak_ptr<CefFrame> frame)
    {
        auto element = frame.lock();

        if (element)
        {
            auto e = find(frames.begin(), frames.end(), element);

            if (e != frames.end()) frames.erase(e);
        }
    }
}