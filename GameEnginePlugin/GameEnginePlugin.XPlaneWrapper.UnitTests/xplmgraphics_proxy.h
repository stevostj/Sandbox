#ifndef GAMEENGINEPLUGINXPLANEWRAPPERUNITTESTS_XPLMGRAPHICSPROXY_H_
#define GAMEENGINEPLUGINXPLANEWRAPPERUNITTESTS_XPLMGRAPHICSPROXY_H_

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "set_xplm_api_hooks.h"

namespace gep_xpw_ut {

    /// <summary>
    /// Abstract base class for XPLMGraphics proxy implementations. 
    /// </summary>
    class XPLMGraphicsProxy
    {
    public:

        virtual void XPLMSetGraphicsState(
            int                  inEnableFog,
            int                  inNumberTexUnits,
            int                  inEnableLighting,
            int                  inEnableAlphaTesting,
            int                  inEnableAlphaBlending,
            int                  inEnableDepthTesting,
            int                  inEnableDepthWriting) = 0;
    };

    /// <summary>
    /// TODO: templatize the Singleton aspect of this
    /// Google Mock Implementation class used for unit testing of calls to XPLM Graphics functions.
    /// </summary>
    class MockXPLMGraphicsProxy : public XPLMGraphicsProxy {
    public:

        /// <summary>
        /// An API hook to handle proxy calls to XPLM Display functions.
        /// </summary>
        /// <returns></returns>
        XplmSetGraphicsStateFunc const& get_XPLMSetGraphicsStateHandler() const {
            return xplm_set_graphics_state_handler_;
        }

        /// <summary>
        /// Singleton accessor
        /// </summary>
        /// <returns>The singleton object</returns>
        static MockXPLMGraphicsProxy& get_instance() {
            if (MockXPLMGraphicsProxy::instance_ == nullptr) {
                MockXPLMGraphicsProxy::instance_ = new MockXPLMGraphicsProxy();
            }

            return *MockXPLMGraphicsProxy::instance_;
        }

        /// <summary>
        /// Singleton destroyer to allow for memory cleanup
        /// </summary>
        static void destroy() {
            if (MockXPLMGraphicsProxy::instance_ != nullptr) {
                delete MockXPLMGraphicsProxy::instance_;
                MockXPLMGraphicsProxy::instance_ = nullptr;
            }
        }

        /// <summary>
        /// Forward XPLM proxy function call to the mock object. 
        /// </summary>
        /// <param name="inEnableFog">See XPLM Graphics SetGraphicsState</param>
        /// <param name="inNumberTexUnits">See XPLM Graphics SetGraphicsState</param>
        /// <param name="inEnableLighting">See XPLM Graphics SetGraphicsState</param>
        /// <param name="inEnableAlphaTesting">See XPLM Graphics SetGraphicsState</param>
        /// <param name="inEnableAlphaBlending">See XPLM Graphics SetGraphicsState</param>
        /// <param name="inEnableDepthTesting">See XPLM Graphics SetGraphicsState</param>
        /// <param name="inEnableDepthWriting">See XPLM Graphics SetGraphicsState</param>
        /// <returns></returns>
        static void MockXPLMGraphicsProxy::HandleXPLMSetGraphicsState(
            int                  inEnableFog,
            int                  inNumberTexUnits,
            int                  inEnableLighting,
            int                  inEnableAlphaTesting,
            int                  inEnableAlphaBlending,
            int                  inEnableDepthTesting,
            int                  inEnableDepthWriting)
        {
            MockXPLMGraphicsProxy::get_instance()
                .XPLMSetGraphicsState(inEnableFog, inNumberTexUnits, inEnableLighting, inEnableAlphaTesting, 
                    inEnableAlphaBlending, inEnableDepthTesting, inEnableDepthWriting);
        }

        MOCK_METHOD(void, XPLMSetGraphicsState, (
            int                  inEnableFog,
            int                  inNumberTexUnits,
            int                  inEnableLighting,
            int                  inEnableAlphaTesting,
            int                  inEnableAlphaBlending,
            int                  inEnableDepthTesting,
            int                  inEnableDepthWriting), (override));

        // Disable copying
        MockXPLMGraphicsProxy(MockXPLMGraphicsProxy& other) = delete;

        // Disable assigment
        void operator=(const MockXPLMGraphicsProxy&) = delete;

    private:
        MockXPLMGraphicsProxy() : xplm_set_graphics_state_handler_(MockXPLMGraphicsProxy::HandleXPLMSetGraphicsState) {}

        ~MockXPLMGraphicsProxy();

        XplmSetGraphicsStateFunc xplm_set_graphics_state_handler_;

        static MockXPLMGraphicsProxy* instance_;

    };

}

#endif