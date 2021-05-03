#include <string>

#ifdef IBM
#include <windows.h>
#endif
#if LIN
#include <GL/gl.h>
#elif __GNUC__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "xplane_plugin.h"

HMODULE hMod;
std::wstring DllPathAndName;
std::wstring DllPath;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: case DLL_THREAD_ATTACH: case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    hMod = hModule;
    const int BUFSIZE = 4096;
    wchar_t buffer[BUFSIZE];
    if (::GetModuleFileName(hMod, buffer, BUFSIZE - 1) <= 0)
    {
        return TRUE;
    }

    DllPathAndName = buffer;

    size_t found = DllPathAndName.find_last_of(L"/\\");
    DllPath = DllPathAndName.substr(0, found);

    return TRUE;
}