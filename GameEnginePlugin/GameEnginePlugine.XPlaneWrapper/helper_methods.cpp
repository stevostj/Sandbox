#include "helper_methods.h"

namespace gep_xpw {

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
