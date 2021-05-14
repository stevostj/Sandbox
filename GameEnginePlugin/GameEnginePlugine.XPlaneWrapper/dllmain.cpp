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
XplmDisplayRegisterDrawCallbackFunc XPLMDisplayRegisterDrawCallback;

namespace {

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

    void SetDefaultXplmApiHooks() {
        XPLMDisplayRegisterDrawCallback = &XPLMRegisterDrawCallback;
    }

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: case DLL_THREAD_ATTACH: case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    
    SetDllPaths(hModule);

    SetDefaultXplmApiHooks();

    return TRUE;
}