#include "set_gep_api_hooks.h"
#include "helper_methods.h"
#include "xplane_plugin.h"

GEPApi GepApi;

SGEPAH_DECLSPEC int SetGepApiHooks(GEPApi* gep_api_hooks)
{
	int return_code = -1; // TODO: define error codes
	if (gep_xpw::CheckHookStructures(gep_api_hooks))
	{
		return_code = SGEPAH_INITIALIZE_OK;
		GepApi.Initialize = gep_api_hooks->Initialize; // set global
		GepApi.HandleSimulationResponseMessages = gep_api_hooks->HandleSimulationResponseMessages; // set global
		GepApi.HandleSimulationControlMessages = gep_api_hooks->HandleSimulationControlMessages; // set global
	}

	return return_code;
}
