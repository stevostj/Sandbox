#include <windows.h>
#include <GL/gl.h>
#include <string>
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "game_engine_plugin_api.h"
#include "xplane_plugin.h"
#include "xplmdisplay_proxy.h"


#ifndef XPLM300
	#error This is made to be compiled against the XPLM300 SDK
#endif

HINSTANCE hGepHandle;
int operating_context; // global variable used track whether the plugin is being run in unit test context or xplane context. 
SymbologyAdapter * symbology_adapter;

HINSTANCE LoadGameEnginePluginLibraries()
{

	// TODO: find plugin(s) in the directory instead of hardcoding the name.
	std::wstring lib_path = DllPath + L"\\ExampleGameEnginePlugin.dll";

	return ::LoadLibrary((LPWSTR)lib_path.c_str());
}



PLUGIN_API int XPluginStart(
							char *		outName,
							char *		outSig,
							char *		outDesc)
{
	strcpy(outName, "Game Engine Plugin - XPlane Wrapper");
	strcpy(outSig, "gameengineplugin");
	strcpy(outDesc, "Loads and runs 'generic' plugins that easily adapt to different game engine systems. ");

    hGepHandle = LoadGameEnginePluginLibraries();

	int xpluginstart_rv = (hGepHandle != 0) ? 1 : 0;
	
	XPLMDisplayProxy* displayProxy;
	symbology_adapter = new SymbologyAdapter(displayProxy);

	return xpluginstart_rv;
}

PLUGIN_API void	XPluginStop(void)
{
	// TODO: Clean up resources
	delete symbology_adapter;
	symbology_adapter = nullptr;
}

PLUGIN_API void XPluginDisable(void) { }
PLUGIN_API int  XPluginEnable(void)  { return 1; }
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam) { }

PLUGIN_API void XPluginSetOperatingContext(int inContext) 
{
	operating_context = inContext;
}