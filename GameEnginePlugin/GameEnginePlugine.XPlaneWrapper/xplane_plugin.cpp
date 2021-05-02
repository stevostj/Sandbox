
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include <string>

#ifdef IBM
	#include <windows.h>
#endif
#if LIN
	#include <GL/gl.h>
#elif __GNUC__
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

#include "game_engine_plugin_api.h""


#ifndef XPLM300
	#error This is made to be compiled against the XPLM300 SDK
#endif


static HINSTANCE hGepHandle;

HINSTANCE LoadGameEnginePluginLibraries()
{

	// TODO: find plugin(s) in the directory instead of hardcoding the name.
	std::string lib_path = (".\\Resources\\plugins\\GameEnginePlugin.XPlaneWrapper\\64\\ExampleGameEnginePlugin.dll");

	return ::LoadLibrary((LPCSTR)lib_path.c_str());
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
	
	return xpluginstart_rv;
}

PLUGIN_API void	XPluginStop(void)
{
	// TODO: Clean up resources
}

PLUGIN_API void XPluginDisable(void) { }
PLUGIN_API int  XPluginEnable(void)  { return 1; }
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam) { }