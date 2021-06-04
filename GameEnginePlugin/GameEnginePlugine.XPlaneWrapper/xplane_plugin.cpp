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

	bool InitializeGep(HINSTANCE& plugin_handle, GEPApi& gep_api)
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

CIGI_VIEW_CONTROL GenerateViewControl()
{
	XPLMCameraPosition_t camera_position = {};
	XplmCameraApi.ReadCameraPosition(&camera_position);

	// Assume that the 'main' view is in a fixed position relative to the aircraft.
	// This more or less means the 'camera position' that is reported is actually
	// the aircraft position in the local coordinate system. 
	CIGI_VIEW_CONTROL view_control = {};
	view_control.packet_size = CIGI_VIEW_CONTROL_SIZE;
	view_control.packet_id = CIGI_VIEW_CONTROL_OPCODE;
	view_control.group_id = 0;
	view_control.xoffset_enable = 1;
	view_control.yoffset_enable = 1;
	view_control.zoffset_enable = 1;
	view_control.roll_enable = 1;
	view_control.pitch_enable = 1;
	view_control.yaw_enable = 1;
	// 2  reserved bits
	view_control.view_id = 1;
	view_control.entity_id = 0;
	// 16 reserved bits
	view_control.xoffset = 0.0f;;
	view_control.yoffset = 0.0f;
	view_control.zoffset = 0.0f;
	view_control.roll = 0.0f;
	view_control.pitch = 0.0f;
	view_control.yaw = 0.0f;
	// 32 reserved bits

	return view_control;
}


CIGI_ENTITY_CONTROL GenerateEntityControl()
{
	CIGI_ENTITY_CONTROL entity_control = {};
	entity_control.packet_size = CIGI_ENTITY_CONTROL_SIZE;
	entity_control.packet_id = CIGI_ENTITY_CONTROL_OPCODE;
	entity_control.entity_state = CIGI_ACTIVE;
	entity_control.collision_detect = CIGI_DISABLED;
	entity_control.inherit_alpha = CIGI_DISABLED;
	entity_control.smoothing_enable = CIGI_DISABLED;
	entity_control.extended_entity_type = CIGI_DISABLED;
	// 2 bits reserved
	entity_control.alpha = 0xFF; // Full opacity
	entity_control.entity_id = 0; // Assume that the initial aircraft being flown is entity 0
	entity_control.entity_kind = 0; // TODO: Support SISO-REF-010 entity types
	entity_control.entity_domain = 0;
	entity_control.entity_country_type = 0;
	entity_control.entity_category = 0;
	entity_control.entity_subcategory = 0;
	entity_control.entity_specific = 0;
	entity_control.entity_extra = 0;

	return entity_control;
}


CIGI_IG_CONTROL GenerateIgControl()
{
	CIGI_IG_CONTROL ig_ctrl = {};
	ig_ctrl.packet_size = CIGI_IG_CONTROL_SIZE;
	ig_ctrl.packet_id = CIGI_IG_CONTROL_OPCODE;
	ig_ctrl.cigi_version = CIGI_VERSION;
	ig_ctrl.ig_mode = IG_CONTROL_IG_MODE_OPERATE;
	// TODO: populate more relevant IG control fields

	return ig_ctrl;
}


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
	const int kMaxNumPackets = 100;
	CigiControlPacket packets[kMaxNumPackets];
	packets[0].data.ig_control = GenerateIgControl();
	packets[1].data.entity_control = GenerateEntityControl();
	packets[2].data.view_control = GenerateViewControl();

	short num_packets = 2;
	GepApi.HandleSimulationControlMessages(packets, kMaxNumPackets, &num_packets);

	return -1.0; // schedules xplane to call this function in the next flight loop
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

	int xpluginstart_rv = (gep_initialize_ok && gep_xpw::CheckHookStructures(&XplmDisplayApi, &XplmGraphicsApi, &XplmProcessingApi, &XplmCameraApi)) ? 1 : 0;
	
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
