#include <string>

#ifdef IBM
#include <windows.h>
#endif

#include <GL/gl.h>

#include "XPLMDisplay.h"
#include "xplane_plugin.h"

// globals
HMODULE hMod;
std::wstring DllPathAndName;
std::wstring DllPath;
XPLMDisplayApi XplmDisplayApi;
XPLMGraphicsApi XplmGraphicsApi;
XPLMProcessingApi XplmProcessingApi;

namespace {

    /// <summary>
    /// Determines the full path to this dll and sets global path variables
    /// </summary>
    /// <param name="hModule">Handle to this dll</param>
    void SetDllPaths(HMODULE hModule) 
    {
        bool paths_set = false;
        hMod = hModule;
        const int BUFSIZE = 4096;
        wchar_t buffer[BUFSIZE];
        if (::GetModuleFileName(hMod, buffer, BUFSIZE - 1) <= 0)
        {
            return;
        }

        DllPathAndName = buffer;

        size_t found = DllPathAndName.find_last_of(L"/\\");
        DllPath = DllPathAndName.substr(0, found);
    }

    /// <summary>
    /// Default API hooks to XPlane functions
    /// </summary>
    int SetDefaultXplmApiHooks() 
    {
        XPLMDisplayApi display_api_hooks;
        display_api_hooks.RegisterDrawCallback = &XPLMRegisterDrawCallback;

        XPLMGraphicsApi graphics_api_hooks;
        graphics_api_hooks.SetGraphicsState = &XPLMSetGraphicsState;

        XPLMProcessingApi processing_api_hooks;
        processing_api_hooks.RegisterFlightLoopCallback = &XPLMRegisterFlightLoopCallback;

        return SetXplmApiHooks(display_api_hooks, graphics_api_hooks, processing_api_hooks);
    }

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    BOOL rv = TRUE;
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH: 
        {
            SetDllPaths(hModule);
            if (SetDefaultXplmApiHooks() != 0)
                rv = FALSE;

            break;
        }
        case DLL_THREAD_ATTACH: 
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return rv;
}