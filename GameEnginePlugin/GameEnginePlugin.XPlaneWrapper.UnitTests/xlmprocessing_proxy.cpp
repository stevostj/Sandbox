#include "xplmprocessing_proxy.h"

namespace gep_xpw_ut {

	MockXPLMProcessingProxy* MockXPLMProcessingProxy::instance_ = nullptr;

	MockXPLMProcessingProxy::~MockXPLMProcessingProxy() {
		xplm_register_flight_loop_callback_handler_ = nullptr;
	}

}