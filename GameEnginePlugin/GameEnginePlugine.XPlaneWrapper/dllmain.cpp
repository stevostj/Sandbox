#include <string>

#ifdef IBM
#include <windows.h>
#endif

#include <GL/gl.h>

#include "XPLMDisplay.h"
#include "xplane_plugin.h"
#include "symbol_surface.h"

// globals
HMODULE hMod;
std::wstring DllPathAndName;
std::wstring DllPath;
XPLMDisplayApi XplmDisplayApi;
XPLMGraphicsApi XplmGraphicsApi;
XPLMProcessingApi XplmProcessingApi;
XPLMCameraApi XplmCameraApi;

gep_xpw::SymbolSurface Symbols;

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
        XplmDisplayApi.RegisterDrawCallback = &XPLMRegisterDrawCallback;
        XplmDisplayApi.GetScreenSize = &XPLMGetScreenSize;

        XplmGraphicsApi.SetGraphicsState = &XPLMSetGraphicsState;

        XplmProcessingApi.RegisterFlightLoopCallback = &XPLMRegisterFlightLoopCallback;

        XplmCameraApi.ReadCameraPosition = &XPLMReadCameraPosition;

        return SetXplmApiHooks(&XplmDisplayApi, &XplmGraphicsApi, &XplmProcessingApi, &XplmCameraApi);
    }

    /// <summary>
    /// Default GEP API hooks to null
    /// </summary>
    void SetDefaultGepApiHooks()
    {
        GepApi.Initialize = nullptr;
        GepApi.HandleSimulationControlMessages = nullptr;
        GepApi.HandleSimulationResponseMessages = nullptr;
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
            SetDefaultGepApiHooks();
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