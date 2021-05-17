#ifndef GAMEENGINEPLUGINXPLANEWRAPPERUNITTESTS_XPLMPROCESSINGPROXY_H_
#define GAMEENGINEPLUGINXPLANEWRAPPERUNITTESTS_XPLMPROCESSINGPROXY_H_

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "set_xplm_api_hooks.h"

namespace gep_xpw_ut {

    /// <summary>
    /// Abstract base class for XPLMProcessing proxy implementations. 
    /// </summary>
    class XPLMProcessingProxy
    {
    public:

        virtual void XPLMRegisterFlightLoopCallback(
            XPLMFlightLoop_f     inFlightLoop,
            float                inInterval,
            void* inRefcon) = 0;
    };

    /// <summary>
    /// TODO: templatize the Singleton aspect of this
    /// Google Mock Implementation class used for unit testing of calls to XPLM Processing functions.
    /// </summary>
    class MockXPLMProcessingProxy : public XPLMProcessingProxy {
    public:

        /// <summary>
        /// An API hook to handle proxy calls to XPLM Display functions.
        /// </summary>
        /// <returns></returns>
        XPLMProcessingApi::RegisterFlightLoopCallbackFunc const& get_XPLMRegisterFlightLoopCallbackHandler() const 
        {
            return xplm_register_flight_loop_callback_handler_;
        }

        /// <summary>
        /// Singleton accessor
        /// </summary>
        /// <returns>The singleton object</returns>
        static MockXPLMProcessingProxy& get_instance() 
        {
            if (MockXPLMProcessingProxy::instance_ == nullptr) {
                MockXPLMProcessingProxy::instance_ = new MockXPLMProcessingProxy();
            }

            return *MockXPLMProcessingProxy::instance_;
        }

        /// <summary>
        /// Singleton destroyer to allow for memory cleanup
        /// </summary>
        static void destroy() 
        {
            if (MockXPLMProcessingProxy::instance_ != nullptr) {
                delete MockXPLMProcessingProxy::instance_;
                MockXPLMProcessingProxy::instance_ = nullptr;
            }
        }

        /// <summary>
        /// Forward XPLM proxy function call to the mock object. 
        /// </summary>
        /// <param name="inFlightLoop">See XPLM Processing RegisterFlightLoopCallback</param>
        /// <param name="inInterval">See XPLM Processing RegisterFlightLoopCallback</param>
        /// <param name="inRefcon">See XPLM Processing RegisterFlightLoopCallback</param>
        /// <returns></returns>
        static void MockXPLMProcessingProxy::HandleXPLMRegisterFlightLoopCallback( XPLMFlightLoop_f inFlightLoop, float inInterval, void* inRefcon)
        {
            MockXPLMProcessingProxy::get_instance().XPLMRegisterFlightLoopCallback(inFlightLoop, inInterval, inRefcon);
        }

        MOCK_METHOD(void, XPLMRegisterFlightLoopCallback, (
            XPLMFlightLoop_f     inFlightLoop,
            float                inInterval,
            void* inRefcon), (override));

        // Disable copying
        MockXPLMProcessingProxy(MockXPLMProcessingProxy& other) = delete;

        // Disable assigment
        void operator=(const MockXPLMProcessingProxy&) = delete;

    private:
        MockXPLMProcessingProxy() : xplm_register_flight_loop_callback_handler_(MockXPLMProcessingProxy::HandleXPLMRegisterFlightLoopCallback) {}

        ~MockXPLMProcessingProxy();

        XPLMProcessingApi::RegisterFlightLoopCallbackFunc xplm_register_flight_loop_callback_handler_;

        static MockXPLMProcessingProxy* instance_;

    };

}

#endif