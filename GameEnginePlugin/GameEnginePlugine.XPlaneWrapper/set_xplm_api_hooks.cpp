
#include "set_xplm_api_hooks.h"
#include "helper_methods.h"
#include "xplane_plugin.h"

SXPLMAH_DECLSPEC int SetXplmApiHooks(XPLMDisplayApi display_api_hooks, XPLMGraphicsApi graphics_api_hooks)
{
	int return_code = -1; // TODO: define error codes
	if (gep_xpw::CheckHookStructures(display_api_hooks, graphics_api_hooks))
	{
		return_code = SXPLMAH_INITIALIZE_OK; 
		XplmDisplayApi = display_api_hooks; // set global
		XplmGraphicsApi = graphics_api_hooks; // set global
	}

	return return_code;
}
