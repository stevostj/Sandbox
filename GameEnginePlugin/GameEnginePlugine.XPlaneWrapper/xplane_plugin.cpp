#include <windows.h>
#include <GL/gl.h>
#include <string>
#include <list>

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
		// FUTURE: find plugin(s) in the directory instead of hardcoding the name.
		std::wstring lib_path = DllPath + L"\\ExampleGameEnginePlugin.dll";

		return ::LoadLibrary((LPWSTR)lib_path.c_str());
	}

	/// <summary>
	/// Find functions dynamically a plugin and store pointers to them in a global variable. 
	/// </summary>
	bool SetGepApiHooksToPluginFunctions(HINSTANCE plugin_handle)
	{
		GEPApi::InitializeFunc initialize_func =
			(GEPApi::InitializeFunc) ::GetProcAddress(plugin_handle, "GEP_Initialize");
		GepApi.Initialize = initialize_func;

		GEPApi::HandleSimulationResponseMessagesFunc handlesimulationresponsemessages_func =
			(GEPApi::HandleSimulationResponseMessagesFunc) ::GetProcAddress(plugin_handle, "GEP_HandleSimulationResponseMessages");
		GepApi.HandleSimulationResponseMessages = handlesimulationresponsemessages_func;

		GEPApi::HandleSimulationControlMessagesFunc handlesimulationcontrolmessages_func =
			(GEPApi::HandleSimulationControlMessagesFunc) ::GetProcAddress(plugin_handle, "GEP_HandleSimulationControlMessages");
		GepApi.HandleSimulationControlMessages = handlesimulationcontrolmessages_func;

		return gep_xpw::CheckHookStructures(&GepApi);
	}

	/// <summary>
	/// Sets GEP functions if they have not already been injected by the host. 
	/// </summary>
	bool InitializeGep(HINSTANCE& plugin_handle, GEPApi& gep_api)
	{
		bool rv = true;
		if (plugin_handle == 0 && !gep_xpw::CheckHookStructures(&GepApi)) // assumes api hooks to stub out plugin in unit test context are done prior to initialize
		{
			plugin_handle = LoadGameEnginePluginLibraries();
			rv = (hGepHandle != 0) && SetGepApiHooksToPluginFunctions(plugin_handle);
		}

		return rv;
	}

	/// <summary>
	/// Registers the draw callback with X-Plane and initializes the symbol surface with the necessary XPLM APIs. 
	/// </summary>
	int InitializeSymbologyRendering()
	{
		int rv = XplmDisplayApi.RegisterDrawCallback(XPLMDrawCallback, xplm_Phase_LastCockpit, 1 /*end of phase*/, nullptr);
		XplmDisplayApi.DrawCallback = XPLMDrawCallback;
		if(rv != 0)
		  rv = Symbols.Initialize(&XplmDisplayApi, &XplmGraphicsApi);

		return rv;
	}

	/// <summary>
	/// Registers the flight loop callback with X-Plane.
	/// </summary>
	void InitializeSimulationFrameHandler()
	{
		XplmProcessingApi.RegisterFlightLoopCallback(XPLMFlightLoopCallback, -1.0 /* schedule for first flight loop */, nullptr);
	}

}

CIGI_VIEW_CONTROL GenerateViewControl()
{
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
	entity_control.entity_kind = 0; // FUTURE: Support SISO-REF-010 entity types
	entity_control.entity_domain = 0;
	entity_control.entity_country_type = 0;
	entity_control.entity_category = 0;
	entity_control.entity_subcategory = 0;
	entity_control.entity_specific = 0;
	entity_control.entity_extra = 0;

	return entity_control;
}

CIGI_ENTITY_POSITION GenerateEntityPosition(XPLMCameraPosition_t & camera_position)
{
	// Assume that the 'main' view is in a fixed position relative to the aircraft.
	// This more or less means the 'camera position' that is reported is actually
	// the aircraft position in the local coordinate system. 

	// FUTURE: transform to geodetic coordinates
	CIGI_ENTITY_POSITION entity_position = {};
	entity_position.packet_size = CIGI_ENTITY_POSITION_SIZE;
	entity_position.packet_id = CIGI_ENTITY_POSITION_OPCODE;
	entity_position.entity_id = 0;
	entity_position.lat_x = camera_position.x;
	entity_position.lon_y = camera_position.y;
	entity_position.alt_z = camera_position.z;
	entity_position.roll = camera_position.roll;
	entity_position.pitch = camera_position.pitch;
	entity_position.yaw = camera_position.heading;

	return entity_position;
}

CIGI_IG_CONTROL GenerateIgControl()
{
	CIGI_IG_CONTROL ig_ctrl = {};
	ig_ctrl.packet_size = CIGI_IG_CONTROL_SIZE;
	ig_ctrl.packet_id = CIGI_IG_CONTROL_OPCODE;
	ig_ctrl.cigi_version = CIGI_VERSION;
	ig_ctrl.ig_mode = IG_CONTROL_IG_MODE_OPERATE;
	// FUTURE: populate more relevant IG control fields

	return ig_ctrl;
}

CIGI_START_OF_FRAME GenerateStartOfFrame()
{
	CIGI_START_OF_FRAME start_of_frame = {};
	start_of_frame.packet_size = CIGI_START_OF_FRAME_SIZE;
	start_of_frame.packet_id = CIGI_START_OF_FRAME_OPCODE;
	start_of_frame.cigi_major_version = CIGI_VERSION;
	start_of_frame.minor_version = 0;
	// FUTURE: populate more relevant start of frame fields

	return start_of_frame;
}

/// <summary>
/// Find symbology messages and update the symbol surface.
/// </summary>
void UpdateSymbols(CigiControlPacket* packets, int num_packets) 
{
	std::list<CIGI_SYMBOL_POLYGON_DEF> polygons;
	for (int i = 0; i < num_packets; ++i)
	{
		if (packets[i].data.symbol_surface_definition.packet_id == CIGI_SYMBOL_SURFACE_DEF_OPCODE)
		{
			Symbols.Update(packets[i].data.symbol_surface_definition);
		}
		else if (packets[i].data.symbol_polygon_definition.packet_id == CIGI_SYMBOL_POLYGON_DEF_OPCODE) 
		{
			polygons.push_back(packets[i].data.symbol_polygon_definition);
		}

	}

	Symbols.Update(polygons);
}

/// <summary>
/// Renders symbols and sends simulation response messages to game engine plugins. 
/// </summary>
int XPLMDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon)
{

	Symbols.Render();

	const int kMaxNumPackets = 100;
	CigiResponsePacket packets[kMaxNumPackets];
	packets[0].data.start_of_frame = GenerateStartOfFrame();

	int num_packets = 1;
	GepApi.HandleSimulationResponseMessages(packets, kMaxNumPackets, &num_packets);

	return 1;
}

/// <summary>
/// Sends simulation control messages to game engine plugins based on state information from X-Plane, and processes
/// messages added by game engine plugins.
/// </summary>
float XPLMFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon)
{
	const int kMaxNumPackets = 100;
	CigiControlPacket packets[kMaxNumPackets];

	XPLMCameraPosition_t camera_position = {};
	XplmCameraApi.ReadCameraPosition(&camera_position);

	packets[0].data.ig_control = GenerateIgControl();
	packets[1].data.entity_control = GenerateEntityControl();
	packets[2].data.entity_position = GenerateEntityPosition(camera_position);
	packets[3].data.view_control = GenerateViewControl();

	int num_packets = 4;
	GepApi.HandleSimulationControlMessages(packets, kMaxNumPackets, &num_packets);

	UpdateSymbols(packets, num_packets);

	return -1.0; // schedules xplane to call this function in the next flight loop
}

/// <summary>
/// Called by X-Plane during its initialization
/// </summary>
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
		xpluginstart_rv = InitializeSymbologyRendering();

	if(xpluginstart_rv != 0)
	  InitializeSimulationFrameHandler();

	return xpluginstart_rv;
}

/// <summary>
/// Called by X-Plane during its shutdown.
/// </summary>
PLUGIN_API void	XPluginStop(void)
{
	// FUTURE: Clean up resources
}

PLUGIN_API void XPluginDisable(void) { }
PLUGIN_API int  XPluginEnable(void)  { return 1; }
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam) { }
