
#include "set_xplm_api_hooks.h"
#include "helper_methods.h"
#include "xplane_plugin.h"

SXPLMAH_DECLSPEC int SetXplmApiHooks(XPLMDisplayApi * display_api_hooks, XPLMGraphicsApi * graphics_api_hooks, XPLMProcessingApi * processing_api_hooks, XPLMCameraApi * camera_api_hooks)
{
	int return_code = -1; // TODO: define error codes
	if (gep_xpw::CheckHookStructures(display_api_hooks, graphics_api_hooks, processing_api_hooks, camera_api_hooks))
	{
		return_code = SXPLMAH_INITIALIZE_OK; 
		XplmDisplayApi.RegisterDrawCallback = display_api_hooks->RegisterDrawCallback; // set global
		XplmDisplayApi.DrawCallback = display_api_hooks->DrawCallback = XPLMDrawCallback;
		XplmDisplayApi.GetScreenSize = display_api_hooks->GetScreenSize;

		XplmGraphicsApi.SetGraphicsState = graphics_api_hooks->SetGraphicsState; // set global

		XplmProcessingApi.RegisterFlightLoopCallback = processing_api_hooks->RegisterFlightLoopCallback; // set global
		XplmProcessingApi.FlightLoopCallback = processing_api_hooks->FlightLoopCallback = XPLMFlightLoopCallback;

		XplmCameraApi.ReadCameraPosition = camera_api_hooks->ReadCameraPosition; // set global
	}

	return return_code;
}
