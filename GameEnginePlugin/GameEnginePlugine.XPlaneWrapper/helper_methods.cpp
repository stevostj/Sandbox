#include "helper_methods.h"

namespace gep_xpw {

	/// <summary>
	/// Ensure that the API hooks are not null. 
	/// </summary>
	/// <param name="display_api_hooks"></param>
	/// <param name="graphics_api_hooks"></param>
	/// <param name="processing_api_hooks"></param>
	/// <returns></returns>
	bool CheckHookStructures(XPLMDisplayApi const * display_api_hooks, XPLMGraphicsApi const *graphics_api_hooks, XPLMProcessingApi const * processing_api_hooks, XPLMCameraApi const * camera_api_hooks)
	{
		bool display_ok =
			(display_api_hooks != nullptr) && 
			(display_api_hooks->RegisterDrawCallback != nullptr) &&
			(display_api_hooks->GetScreenSize != nullptr);

		bool graphics_ok =
			(graphics_api_hooks != nullptr) && 
			(graphics_api_hooks->SetGraphicsState != nullptr);

		bool processing_ok =
			(processing_api_hooks != nullptr) &&
			(processing_api_hooks->RegisterFlightLoopCallback != nullptr);

		bool camera_ok =
			(camera_api_hooks != nullptr) &&
			(camera_api_hooks->ReadCameraPosition != nullptr);

		return display_ok && graphics_ok && processing_ok && camera_ok;
	}

	/// <summary>
	/// Ensure that the API hooks are not null. 
	/// </summary>
	/// <param name="gep_api_hooks">Contains pointers to functions to</param>
	/// <returns></returns>
	bool CheckHookStructures(GEPApi const* gep_api_hooks)
	{
		bool hooks_ok =
			(gep_api_hooks != nullptr) &&
			(gep_api_hooks->Initialize != nullptr) &&
			(gep_api_hooks->HandleSimulationControlMessages != nullptr) && 
			(gep_api_hooks->HandleSimulationResponseMessages != nullptr);

		return hooks_ok;
	}
}
