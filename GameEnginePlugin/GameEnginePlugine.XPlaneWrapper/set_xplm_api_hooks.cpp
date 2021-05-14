
#include "set_xplm_api_hooks.h"
#include "xplane_plugin.h"

SXPLMAH_DECLSPEC int SetXplmApiHooks(XplmDisplayRegisterDrawCallbackFunc reg_draw_cb)
{
	int return_code = -1; // TODO: define error codes
	if (reg_draw_cb == nullptr)
	{
		return_code = SXPLMAH_INITIALIZE_OK; 
		XPLMDisplayRegisterDrawCallback = reg_draw_cb;
	}

	return return_code;
}
