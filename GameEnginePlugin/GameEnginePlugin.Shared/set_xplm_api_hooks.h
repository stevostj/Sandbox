#ifndef GAMEENGINEPLUGINSHARED_SETXPLMAPIHOOKS_H_
#define GAMEENGINEPLUGINSHARED_SETXPLMAPIHOOKS_H_

#ifdef _EXPORTSXPLMAH
#define SXPLMAH_DECLSPEC    __declspec(dllexport)
#else
#define SXPLMAH_DECLSPEC    __declspec(dllimport)
#endif

#include "XPLMDisplay.h"

#define SXPLMAH_INITIALIZE_OK 0

// C/C++ naming
#ifdef __cplusplus
extern "C" {
#endif

	typedef int(__stdcall* XplmDisplayRegisterDrawCallbackFunc)(XPLMDrawCallback_f, XPLMDrawingPhase, int, void*); // function signature for XPLMDisplay RegistrDrawCallback

	/// <summary>
	/// Passes in alternate pointers to functions to be called instead of the various XPLM functions.
	/// </summary>
	/// <param name="reg_draw_cb"></param>
	/// <returns>An error code indicating any problems that occurred while setting the api hooks. </returns>
	SXPLMAH_DECLSPEC int SetXplmApiHooks(XplmDisplayRegisterDrawCallbackFunc reg_draw_cb);

#ifdef __cplusplus
}
#endif

#endif