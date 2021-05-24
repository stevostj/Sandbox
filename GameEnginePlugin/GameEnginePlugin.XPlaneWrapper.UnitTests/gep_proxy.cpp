#include "gep_proxy.h"

namespace gep_xpw_ut {

	MockGEPProxy* MockGEPProxy::instance_ = nullptr;

	MockGEPProxy::~MockGEPProxy()
	{
		gep_initialize_handler_ = nullptr;
		gep_handlestartofframemessages_handler_ = nullptr;
	}

}