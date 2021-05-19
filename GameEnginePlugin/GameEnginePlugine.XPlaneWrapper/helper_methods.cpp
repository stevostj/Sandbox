#include "helper_methods.h"

namespace gep_xpw {

	/// <summary>
	/// Ensure that the API hooks are not null. 
	/// </summary>
	/// <param name="display_api_hooks"></param>
	/// <param name="graphics_api_hooks"></param>
	/// <param name="processing_api_hooks"></param>
	/// <returns></returns>
	bool CheckHookStructures(XPLMDisplayApi display_api_hooks, XPLMGraphicsApi graphics_api_hooks, XPLMProcessingApi processing_api_hooks)
	{
		bool display_ok =
			(display_api_hooks.RegisterDrawCallback != nullptr) &&
			(display_api_hooks.GetScreenSize != nullptr);

		bool graphics_ok =
			(graphics_api_hooks.SetGraphicsState != nullptr);

		return display_ok && graphics_ok;
	}
}
