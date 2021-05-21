#ifndef GAMEENGINEPLUGINXPLANEWRAPPERUNITTESTS_XPLMDISPLAYPROXY_H_
#define GAMEENGINEPLUGINXPLANEWRAPPERUNITTESTS_XPLMDISPLAYPROXY_H_

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "set_xplm_api_hooks.h"

namespace gep_xpw_ut {

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

        virtual XPLMDrawCallback_f get_XPLMDrawCallback() = 0;

        virtual void XPLMGetScreenSize(
            int* outWidth, 
            int* outHeight) = 0;

    };

    /// <summary>
    /// Google Mock Implementation class used for unit testing of calls to XPLM Display functions.
    /// </summary>
    class MockXPLMDisplayProxy : public XPLMDisplayProxy {
    public:

        /// <summary>
        /// An API hook to handle proxy calls to XPLM Display functions.
        /// </summary>
        /// <returns></returns>
        XPLMDisplayApi::RegisterDrawCallbackFunc const & get_XPLMRegisterDrawCallbackHandler() const {
            return xplm_register_draw_callback_handler_;
        }

        /// <summary>
        /// An API hook to handle proxy calls to XPLM Display functions.
        /// </summary>
        /// <returns></returns>
        XPLMDisplayApi::GetScreenSizeFunc const& get_XPLMGetScreenSizeHandler() const {
            return xplm_get_screen_size_handler_;
        }

        /// <summary>
        /// Singleton accessor
        /// </summary>
        /// <returns>The singleton object</returns>
        static MockXPLMDisplayProxy & get_instance() {
            if (MockXPLMDisplayProxy::instance_ == nullptr) {
                MockXPLMDisplayProxy::instance_ = new MockXPLMDisplayProxy();
            }

            return *MockXPLMDisplayProxy::instance_;
        }

        /// <summary>
        /// Singleton destroyer to allow for memory cleanup
        /// </summary>
        static void destroy() {
            if (MockXPLMDisplayProxy::instance_ != nullptr) {
                delete MockXPLMDisplayProxy::instance_;
                MockXPLMDisplayProxy::instance_ = nullptr;
            }
        }

        /// <summary>
        /// Forward the XPLMRegisterDrawCallback function call to the mock object
        /// </summary>
        /// <param name="inCallback">See XPLMDisplay XPLMRegisterDrawCallback</param>
        /// <param name="inPhase">See XPLMDisplay XPLMRegisterDrawCallback</param>
        /// <param name="inWantsBefore">See XPLMDisplay XPLMRegisterDrawCallback</param>
        /// <param name="inRefcon">See XPLMDisplay XPLMRegisterDrawCallback</param>
        /// <returns>See XPLMDisplay XPLMRegisterDrawCallback</returns>
        static int MockXPLMDisplayProxy::HandleXPLMRegisterDrawCallback(XPLMDrawCallback_f inCallback, XPLMDrawingPhase inPhase, int inWantsBefore, void* inRefcon)
        {
            return MockXPLMDisplayProxy::get_instance().XPLMRegisterDrawCallback(inCallback, inPhase, inWantsBefore, inRefcon);
        }


        /// <summary>
        /// Forward the XPLMGetScreenSize function call to the mock object
        /// </summary>
        /// <param name="outWidth">See XPLMDisplay XPLMGetScreenSize</param>
        /// <param name="outHeight">See XPLMDisplay XPLMGetScreenSize</param>
        static void MockXPLMDisplayProxy::HandleXPLMGetScreenSize(int* outWidth, int* outHeight)
        {
            MockXPLMDisplayProxy::get_instance().XPLMGetScreenSize(outWidth, outHeight);
        }

        MOCK_METHOD(int, XPLMRegisterDrawCallback, (
            XPLMDrawCallback_f   inCallback,
            XPLMDrawingPhase     inPhase,
            int                  inWantsBefore,
            void* inRefcon), (override));

        MOCK_METHOD(XPLMDrawCallback_f, get_XPLMDrawCallback, (), (override));

        MOCK_METHOD(void, XPLMGetScreenSize, (
            int* outWidth, 
            int* outHeight), (override)); 

        // Disable copying
        MockXPLMDisplayProxy(MockXPLMDisplayProxy& other) = delete;
   
        // Disable assigment
        void operator=(const MockXPLMDisplayProxy&) = delete;

    private:
        MockXPLMDisplayProxy() : 
            xplm_register_draw_callback_handler_(MockXPLMDisplayProxy::HandleXPLMRegisterDrawCallback), 
            xplm_get_screen_size_handler_(MockXPLMDisplayProxy::HandleXPLMGetScreenSize)
        {}

        ~MockXPLMDisplayProxy();

        XPLMDisplayApi::RegisterDrawCallbackFunc xplm_register_draw_callback_handler_;

        XPLMDisplayApi::GetScreenSizeFunc xplm_get_screen_size_handler_;


        static MockXPLMDisplayProxy* instance_;

    };

}

#endif