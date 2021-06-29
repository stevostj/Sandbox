#ifndef GAMEENGINEPLUGINXPLANEWRAPPERUNITTESTS_XPLMCAMERAPROXY_H_
#define GAMEENGINEPLUGINXPLANEWRAPPERUNITTESTS_XPLMCAMERAPROXY_H_

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "set_xplm_api_hooks.h"

namespace gep_xpw_ut {

    /// <summary>
    /// Abstract base class for XPLMCamera proxy implementations. 
    /// </summary>
    class XPLMCameraProxy {
    public:

        virtual void XPLMReadCameraPosition(XPLMCameraPosition_t * outCameraPosition) = 0;

        XPLMCameraApi & get_XPLMCameraApi() {
            return xplm_camera_api_;
        }

    protected:
        XPLMCameraProxy() {
            xplm_camera_api_.ReadCameraPosition = nullptr;
        }

    private:
        XPLMCameraApi xplm_camera_api_;
    };

    /// <summary>
    /// TODO: templatize the Singleton aspect of this
    /// Google Mock Implementation class used for unit testing of calls to XPLM Camera functions.
    /// </summary>
    class MockXPLMCameraProxy : public XPLMCameraProxy {
    public:

        /// <summary>
        /// An API hook to handle proxy calls to XPLM Camera functions.
        /// </summary>
        /// <returns></returns>
        XPLMCameraApi::ReadCameraPositionFunc const & get_XPLMReadCameraPositionHandler() const
        {
            return xplm_read_camera_position_handler_;
        }

        /// <summary>
        /// Singleton accessor
        /// </summary>
        /// <returns>The singleton object</returns>
        static MockXPLMCameraProxy & get_instance()
        {
            if (MockXPLMCameraProxy::instance_ == nullptr) {
                MockXPLMCameraProxy::instance_ = new MockXPLMCameraProxy();
            }

            return *MockXPLMCameraProxy::instance_;
        }

        /// <summary>
        /// Singleton destroyer to allow for memory cleanup
        /// </summary>
        static void destroy()
        {
            if (MockXPLMCameraProxy::instance_ != nullptr) {
                delete MockXPLMCameraProxy::instance_;
                MockXPLMCameraProxy::instance_ = nullptr;
            }
        }

        /// <summary>
        /// Forward XPLM proxy function call to the mock object. 
        /// </summary>
        /// <param name="inFlightLoop">See XPLM Camera RegisterFlightLoopCallback</param>
        /// <param name="inInterval">See XPLM Camera RegisterFlightLoopCallback</param>
        /// <param name="inRefcon">See XPLM Camera RegisterFlightLoopCallback</param>
        /// <returns></returns>
        static void MockXPLMCameraProxy::HandleXPLMReadCameraPosition(XPLMCameraPosition_t * outCameraPosition)
        {
            MockXPLMCameraProxy::get_instance().XPLMReadCameraPosition(outCameraPosition);
        }

        MOCK_METHOD(void, XPLMReadCameraPosition, (XPLMCameraPosition_t * outCameraPosition), (override));

        // Disable copying
        MockXPLMCameraProxy(MockXPLMCameraProxy& other) = delete;

        // Disable assigment
        void operator=(const MockXPLMCameraProxy&) = delete;

    private:
        MockXPLMCameraProxy() : xplm_read_camera_position_handler_(MockXPLMCameraProxy::HandleXPLMReadCameraPosition) {}

        ~MockXPLMCameraProxy();

        XPLMCameraApi::ReadCameraPositionFunc xplm_read_camera_position_handler_;

        static MockXPLMCameraProxy* instance_;

    };

}

#endif