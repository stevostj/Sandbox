#ifndef GAMEENGINEPLUGINSHARED_SETXPLMAPIHOOKS_H_
#define GAMEENGINEPLUGINSHARED_SETXPLMAPIHOOKS_H_

#ifdef _EXPORTSXPLMAH
#define SXPLMAH_DECLSPEC    __declspec(dllexport)
#else
#define SXPLMAH_DECLSPEC    __declspec(dllimport)
#endif

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMProcessing.h"
#include "XPLMCamera.h"

#define SXPLMAH_INITIALIZE_OK 0

// C/C++ naming
#ifdef __cplusplus
extern "C" {
#endif

	// Function signatures and pointers to XPLMDisplay API
	struct XPLMDisplayApi {

		typedef int(__stdcall* RegisterDrawCallbackFunc)(XPLMDrawCallback_f, XPLMDrawingPhase, int, void*);
		RegisterDrawCallbackFunc RegisterDrawCallback;

		typedef void(__stdcall* GetScreenSizeFunc)(int*, int*);
		GetScreenSizeFunc GetScreenSize;

		XPLMDrawCallback_f DrawCallback;

	};

	// Function signatures and pointers to XPLMGraphics API
	struct XPLMGraphicsApi {
		typedef void(__stdcall* SetGraphicsStateFunc)(int, int, int, int, int, int, int);
		SetGraphicsStateFunc SetGraphicsState;
	};


	// Function signatures and pointers to XPLMProcessing API
	struct XPLMProcessingApi {
		typedef void(__stdcall* RegisterFlightLoopCallbackFunc)(XPLMFlightLoop_f, float, void*);
		RegisterFlightLoopCallbackFunc RegisterFlightLoopCallback;

		XPLMFlightLoop_f FlightLoopCallback;
	};

	// Function signatures and pointers to XPLMCamera API
	struct XPLMCameraApi {
		typedef void(__stdcall* ReadCameraPositionFunc)(XPLMCameraPosition_t*);
		ReadCameraPositionFunc ReadCameraPosition;
	};

	/// <summary>
	/// Passes in alternate pointers to functions to be called instead of the various XPLM functions.
	/// </summary>
	/// <param name="display_api_hooks">Group of pointers to XPLMDisplay functions. </param>
	/// <param name="graphics_api_hooks">Group of pointers to XPLMGraphics functions. </param>
	/// <param name="processing_api_hooks">Group of pointers to XPLMProcessing functions. </param>
	/// <param name="camera_api_hooks">Group of pointers to XPLMCamera functions. </param>
	/// <returns>An error code indicating any problems that occurred while setting the api hooks. </returns>
	SXPLMAH_DECLSPEC int SetXplmApiHooks(XPLMDisplayApi * display_api_hooks, XPLMGraphicsApi * graphics_api_hooks, XPLMProcessingApi * processing_api_hooks, XPLMCameraApi * camera_api_hooks);

#ifdef __cplusplus
}
#endif

#endif