#include "pch.h"
#include "xplmdisplay_proxy.h"

MockXPLMDisplayProxy* MockXPLMDisplayProxy::instance_ = nullptr;

MockXPLMDisplayProxy::~MockXPLMDisplayProxy() {
    xplm_register_draw_callback_handler_ = nullptr;
}
