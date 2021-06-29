#ifndef _GEPXPW_H_
#define _GEPXPW_H_

#include <string>
#include "set_xplm_api_hooks.h"
#include "set_gep_api_hooks.h"
#include "symbol_surface.h"

extern std::wstring DllPathAndName;
extern std::wstring DllPath;

extern XPLMDisplayApi XplmDisplayApi;
extern XPLMGraphicsApi XplmGraphicsApi;
extern XPLMProcessingApi XplmProcessingApi;
extern XPLMCameraApi XplmCameraApi;

extern GEPApi GepApi;

extern gep_xpw::SymbolSurface Symbols; // assume only a single surface

/// <summary>
/// Should only be called by X-Plane. A pointer to this function is registered as a callback. 
/// </summary>
extern int XPLMDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon);

/// <summary>
/// Should only be called by X-Plane. A pointer to this function is registered as a callback. 
/// </summary>
extern float XPLMFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon);

#endif
