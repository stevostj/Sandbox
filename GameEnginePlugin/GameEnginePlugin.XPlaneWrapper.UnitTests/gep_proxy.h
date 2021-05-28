#ifndef GAMEENGINEPLUGINXPLANEWRAPPERUNITTESTS_GEPPROXY_H_
#define GAMEENGINEPLUGINXPLANEWRAPPERUNITTESTS_GEPPROXY_H_

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "game_engine_plugin_api.h"
#include "set_gep_api_hooks.h"

using ::testing::NotNull;
using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::Ge;
using ::testing::Ne;


namespace gep_xpw_ut {

    /// <summary>
    /// Abstract base class for Game Engine Plugin proxy implementations. 
    /// </summary>
    class GEPProxy {
     public:

        virtual int GEP_Initialize() = 0;

        virtual int GEP_HandleStartOfFrameMessages(char** messages_in, char** messages_out) = 0;

        virtual int GEP_HandleSimulationControlMessages(CigiControlPacket* packets, short* num_packets, short max_num_packets) = 0;

        GEPApi & get_GEPApi() {
            return gep_api_;
        }

    protected:
        GEPProxy() 
        {
            gep_api_.Initialize = nullptr;
            gep_api_.HandleStartOfFrameMessages = nullptr;
            gep_api_.HandleSimulationControlMessages = nullptr;
        }

     private:
         GEPApi gep_api_;

    };

    /// <summary>
    /// Google Mock Implementation class used for unit testing of calls to GEP functions.
    /// </summary>
    class MockGEPProxy : public GEPProxy {
    public:

        /// <summary>
        /// An API hook to handle proxy calls to GEP functions.
        /// </summary>
        /// <returns></returns>
        GEPApi::InitializeFunc const& get_GEP_InitializeHandler() const {
            return gep_initialize_handler_;
        }

        /// <summary>
        /// An API hook to handle proxy calls to GEP functions.
        /// </summary>
        /// <returns></returns>
        GEPApi::HandleStartOfFrameMessagesFunc const& get_GEP_HandleStartOfFrameMessagesHandler() const {
            return gep_handlestartofframemessages_handler_;
        }

        /// <summary>
        /// An API hook to handle proxy calls to GEP functions.
        /// </summary>
        /// <returns></returns>
        GEPApi::HandleSimulationControlMessagesFunc const& get_GEP_HandleSimulationControlMessagesHandler() const {
            return gep_handlesimulationcontrolmessages_handler_;
        }

        /// <summary>
        /// Singleton accessor
        /// </summary>
        /// <returns>The singleton object</returns>
        static MockGEPProxy& get_instance() {
            if (MockGEPProxy::instance_ == nullptr) {
                MockGEPProxy::instance_ = new MockGEPProxy();
            }

            return *MockGEPProxy::instance_;
        }

        /// <summary>
        /// Singleton destroyer to allow for memory cleanup
        /// </summary>
        static void destroy() {
            if (MockGEPProxy::instance_ != nullptr) {
                delete MockGEPProxy::instance_;
                MockGEPProxy::instance_ = nullptr;
            }
        }

        /// <summary>
        /// Forward the GEP_Initialize function call to the mock object
        /// </summary>
        /// <returns>See GEP_Initialize</returns>
        static int MockGEPProxy::HandleInitialize()
        {
            return MockGEPProxy::get_instance().GEP_Initialize();
        }


        /// <summary>
        /// Forward the GEP_HandleStartOfFrameMessages function call to the mock object
        /// </summary>
        /// <param name="messages_in">See GEP_HandleStartOfFrameMessages</param>
        /// <param name="messages_out">See GEP_HandleStartOfFrameMessages</param>
        /// <returns>See GEP_HandleStartOfFrameMessages</returns>
        static int MockGEPProxy::HandleHandleStartOfFrameMessages(char** messages_in, char** messages_out)
        {
            return MockGEPProxy::get_instance().GEP_HandleStartOfFrameMessages(messages_in, messages_out);
        }

        /// <summary>
        /// Forward the GEP_HandleSimulationControlMessages function call to the mock object
        /// </summary>
        /// <param name="packets">See GEP_HandleSimulationControlMessages</param>
        /// <param name="num_packets">See GEP_HandleSimulationControlMessages</param>
        /// <param name="max_num_packets">See GEP_HandleSimulationControlMessages</param>
        /// <returns>See GEP_HandleSimulationControlMessages</returns>
        static int MockGEPProxy::HandleHandleSimulationControlMessages(CigiControlPacket* packets, short* num_packets, short max_num_packets)
        {
            return MockGEPProxy::get_instance().GEP_HandleSimulationControlMessages(packets, num_packets, max_num_packets);
        }

        MOCK_METHOD(int, GEP_Initialize, (), (override));

        MOCK_METHOD(int, GEP_HandleStartOfFrameMessages, (
            char** messages_in,
            char** messages_out), (override));

        MOCK_METHOD(int, GEP_HandleSimulationControlMessages, (
            CigiControlPacket* packets, 
            short* num_packets, 
            short max_num_packets), (override));

        // Disable copying
        MockGEPProxy(MockGEPProxy& other) = delete;

        // Disable assigment
        void operator=(const MockGEPProxy&) = delete;

    private:
        MockGEPProxy() :
            gep_initialize_handler_(MockGEPProxy::HandleInitialize),
            gep_handlestartofframemessages_handler_(MockGEPProxy::HandleHandleStartOfFrameMessages), 
            gep_handlesimulationcontrolmessages_handler_(MockGEPProxy::HandleHandleSimulationControlMessages) 
        {
        }

        ~MockGEPProxy();

        GEPApi::InitializeFunc gep_initialize_handler_;

        GEPApi::HandleStartOfFrameMessagesFunc gep_handlestartofframemessages_handler_;

        GEPApi::HandleSimulationControlMessagesFunc gep_handlesimulationcontrolmessages_handler_;

        static MockGEPProxy* instance_;

    };

}

#endif