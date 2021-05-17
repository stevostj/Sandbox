#include "xplmdisplay_proxy.h"

namespace gep_xpw_ut {

	MockXPLMDisplayProxy* MockXPLMDisplayProxy::instance_ = nullptr;

	MockXPLMDisplayProxy::~MockXPLMDisplayProxy() {
		xplm_register_draw_callback_handler_ = nullptr;
		xplm_get_screen_size_handler_ = nullptr;
	}

}