#ifndef GAMEENGINEPLUGINXPLANEWRAPPER_HELPERMETHODS_H_
#define GAMEENGINEPLUGINXPLANEWRAPPER_HELPERMETHODS_H_

#include "set_xplm_api_hooks.h"

namespace gep_xpw {

	bool CheckHookStructures(XPLMDisplayApi const * display_api_hooks, XPLMGraphicsApi const * graphics_api_hooks, XPLMProcessingApi const * processing_api_hooks);

}


#endif