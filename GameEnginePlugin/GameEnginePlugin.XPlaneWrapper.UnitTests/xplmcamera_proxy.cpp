#include "xplmcamera_proxy.h"

namespace gep_xpw_ut {

	MockXPLMCameraProxy* MockXPLMCameraProxy::instance_ = nullptr;

	MockXPLMCameraProxy::~MockXPLMCameraProxy() 
	{
		xplm_read_camera_position_handler_ = nullptr;
	}

}