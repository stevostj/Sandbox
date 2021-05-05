#ifndef __XPLMDISPLAYPROXY__
#define __XPLMDISPLAYPROXY__

/// <summary>
/// Abstract base class for XPLMDisplay proxy implementations. 
/// </summary>
class XPLMDisplayProxy
{
 public:
	virtual void XPLMGetMouseLocation(int* outX, int* outY) = 0;

	virtual ~XPLMDisplayProxy() = 0;
};

#endif