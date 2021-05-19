#include "xplmgraphics_proxy.h"

namespace gep_xpw_ut {

	MockXPLMGraphicsProxy* MockXPLMGraphicsProxy::instance_ = nullptr;

	MockXPLMGraphicsProxy::~MockXPLMGraphicsProxy() {
		xplm_set_graphics_state_handler_ = nullptr;
	}

}