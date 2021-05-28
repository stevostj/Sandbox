#include <windows.h>
#include <GL/gl.h>
#include <string>
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "game_engine_plugin_api.h"
#include "helper_methods.h"
#include "xplane_plugin.h"
#include "xplmdisplay_proxy.h"


#ifndef XPLM300
	#error This is made to be compiled against the XPLM300 SDK
#endif

HINSTANCE hGepHandle; //global


// TODO: Move this into its own file/class
int XPLMDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon)
{
	XplmGraphicsApi.SetGraphicsState(
		0,        // No fog, equivalent to glDisable(GL_FOG);
		1,        // One texture, equivalent to glEnable(GL_TEXTURE_2D);
		0,        // No lighting, equivalent to glDisable(GL_LIGHT0);
		0,        // No alpha testing, e.g glDisable(GL_ALPHA_TEST);
		1,        // Use alpha blending, e.g. glEnable(GL_BLEND);
		0,        // No depth read, e.g. glDisable(GL_DEPTH_TEST);
		0);        // No depth write, e.g. glDepthMask(GL_FALSE);

	return 1;
}

// TODO: Move this into its own file/class
float XPLMFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon)
{
	// TODO: convert xplane state information to structures to pass into game engine plugins.

	const short kMaxNumPackets = 100;
	CigiControlPacket packets[kMaxNumPackets];
	CIGI_IG_CONTROL & ig_ctrl = packets[0].data.ig_control;
	ig_ctrl.packet_size = CIGI_IG_CONTROL_SIZE;
	ig_ctrl.packet_id = CIGI_IG_CONTROL_OPCODE;
	ig_ctrl.cigi_version = CIGI_VERSION;
	ig_ctrl.ig_mode = IG_CONTROL_IG_MODE_OPERATE;
	// TODO: populate more relevant IG control fields

	short num_packets = 1;
	GepApi.HandleSimulationControlMessages(packets, &num_packets, kMaxNumPackets);

	return -1.0; // schedules xplane to call this function in the next flight loop
}

namespace {

	HINSTANCE LoadGameEnginePluginLibraries()
	{
		// TODO: find plugin(s) in the directory instead of hardcoding the name.
		std::wstring lib_path = DllPath + L"\\ExampleGameEnginePlugin.dll";

		return ::LoadLibrary((LPWSTR)lib_path.c_str());
	}

	bool SetGepApiHooksToPluginFunctions(HINSTANCE plugin_handle) 
	{
		GEPApi::InitializeFunc initialize_func = 
			(GEPApi::InitializeFunc) ::GetProcAddress(plugin_handle, "GEP_Initialize");
		GepApi.Initialize = initialize_func;

		GEPApi::HandleStartOfFrameMessagesFunc handlestartofframemessages_func = 
			(GEPApi::HandleStartOfFrameMessagesFunc) ::GetProcAddress(plugin_handle, "GEP_HandleStartOfFrameMessages");
		GepApi.HandleStartOfFrameMessages = handlestartofframemessages_func;

		GEPApi::HandleSimulationControlMessagesFunc handlesimulationcontrolmessages_func =
			(GEPApi::HandleSimulationControlMessagesFunc) ::GetProcAddress(plugin_handle, "GEP_HandleSimulationControlMessages");
		GepApi.HandleSimulationControlMessages = handlesimulationcontrolmessages_func;

		return gep_xpw::CheckHookStructures(&GepApi);
	}

	bool InitializeGep(HINSTANCE & plugin_handle, GEPApi & gep_api) 
	{
		bool rv = true;
		if (plugin_handle == 0 && !gep_xpw::CheckHookStructures(&GepApi)) // assumes api hooks to stub out plugin are done prior initialize
		{
			plugin_handle = LoadGameEnginePluginLibraries();
			rv = (hGepHandle != 0) && SetGepApiHooksToPluginFunctions(plugin_handle);
		}

		return rv;
	}

	// TODO: store this information into an object
	static int ScreenHeight;
	static int ScreenWidth;

	int InitializeSymbologyRendering()
	{
		int rv = XplmDisplayApi.RegisterDrawCallback(XPLMDrawCallback, xplm_Phase_LastCockpit, 1 /*end of phase*/, nullptr);
		XplmDisplayApi.DrawCallback = XPLMDrawCallback;
		XplmDisplayApi.GetScreenSize(&ScreenWidth, &ScreenHeight);

		return rv;
	}

	void InitializeSimulationFrameHandler() 
	{
		XplmProcessingApi.RegisterFlightLoopCallback(XPLMFlightLoopCallback, -1.0 /* schedule for first flight loop */, nullptr);
	}

}

PLUGIN_API int XPluginStart(
							char *		outName,
							char *		outSig,
							char *		outDesc)
{
	strcpy(outName, "Game Engine Plugin - XPlane Wrapper");
	strcpy(outSig, "gameengineplugin");
	strcpy(outDesc, "Loads and runs 'generic' plugins that easily adapt to different game engine systems. ");

	bool gep_initialize_ok = InitializeGep(hGepHandle, GepApi);

    hGepHandle = LoadGameEnginePluginLibraries();

	int xpluginstart_rv = (gep_initialize_ok && gep_xpw::CheckHookStructures(&XplmDisplayApi, &XplmGraphicsApi, &XplmProcessingApi)) ? 1 : 0;
	
	if (xpluginstart_rv != 0)
		xpluginstart_rv = InitializeSymbologyRendering(); // TODO: Create a 'canvas' class to store this information

	if(xpluginstart_rv != 0)
	  InitializeSimulationFrameHandler();

	return xpluginstart_rv;
}

PLUGIN_API void	XPluginStop(void)
{
	// TODO: Clean up resources
}

PLUGIN_API void XPluginDisable(void) { }
PLUGIN_API int  XPluginEnable(void)  { return 1; }
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam) { }
