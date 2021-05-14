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
        XplmDisplayRegisterDrawCallbackFunc const & get_XPLMRegisterDrawCallbackHandler() const {
            return xplm_register_draw_callback_handler_;
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

        MOCK_METHOD(int, XPLMRegisterDrawCallback, (
            XPLMDrawCallback_f   inCallback,
            XPLMDrawingPhase     inPhase,
            int                  inWantsBefore,
            void* inRefcon), (override));

        // Disable copying
        MockXPLMDisplayProxy(MockXPLMDisplayProxy& other) = delete;
   
        // Disable assigment
        void operator=(const MockXPLMDisplayProxy&) = delete;

    private:
        MockXPLMDisplayProxy() : xplm_register_draw_callback_handler_(MockXPLMDisplayProxy::HandleXPLMRegisterDrawCallback) {}

        ~MockXPLMDisplayProxy();

        XplmDisplayRegisterDrawCallbackFunc xplm_register_draw_callback_handler_;

        static MockXPLMDisplayProxy* instance_;

    };

}

#endif