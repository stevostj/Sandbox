#ifndef _GEPXPW_H_
#define _GEPXPW_H_

#include <string>
#include "set_xplm_api_hooks.h"
#include "symbology_adapter.h"

extern std::wstring DllPathAndName;
extern std::wstring DllPath;

extern XPLMDisplayApi XplmDisplayApi;
extern XPLMGraphicsApi XplmGraphicsApi;
extern XPLMProcessingApi XplmProcessingApi;

// TODO: Move this into its own file/class
extern int XPLMDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon);

// TODO: Move this into its own file/class
float XPLMFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon);

#endif
