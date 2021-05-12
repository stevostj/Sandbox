#ifndef __XPLANEWRAPPERUNITTESTS_XPLMDISPLAYPROXY__
#define __XPLANEWRAPPERUNITTESTS_XPLMDISPLAYPROXY__

#include "XPLMDisplay.h"

/// <summary>
/// Abstract base class for XPLMDisplay proxy implementations. 
/// </summary>
class XPLMDisplayProxy
{
public:
    virtual int XPLMRegisterDrawCallback(
        XPLMDrawCallback_f   inCallback,
        XPLMDrawingPhase     inPhase,
        int                  inWantsBefore,
        void* inRefcon) = 0;

};

class XPLMDisplayProxyForwarder
{
public:
    //XPLMDisplayProxyForwarder(XPLMDisplayProxy* proxy);
};

#endif