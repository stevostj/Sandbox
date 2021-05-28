#include "pch.h"
#include <windows.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "xplmgraphics_proxy.h"
#include "xplmdisplay_proxy.h"
#include "xplmprocessing_proxy.h"
#include "xplugin_test_fixture.h"

using ::testing::NotNull;
using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::Ge;
using ::testing::Ne;

namespace gep_xpw_ut {

    TEST_F(XPluginTestFixture, TestXPluginDrawCallback) 
    {

        // Each time XPlane triggers draw callbacks, the XPLMGraphics SetGraphicsState function will be called
        // to 'reset the canvas'. 
        EXPECT_CALL(*graphics_proxy_, XPLMSetGraphicsState(0, _, 0, _, _, _, _));

        // draw callbacks will trigger a start of frame to the game engine plugins
        // TODO: change the SoF function signature to use cigi structures instead of 
        // pointers to a byte buffer. 
        EXPECT_CALL(*gep_proxy_, GEP_HandleStartOfFrameMessages(NotNull(), NotNull()));

        XPLMDrawCallback_f draw_cb = display_proxy_->get_XPLMDisplayApi().DrawCallback;
        EXPECT_NE(draw_cb, nullptr);

        int draw_rv = draw_cb(xplm_Phase_LastCockpit, 0, nullptr);
        EXPECT_NE(draw_rv, 0);
    }

    TEST_F(XPluginTestFixture, TestXPluginFlightLoopCallback)
    {

        XPLMFlightLoop_f flight_loop_cb = processing_proxy_->get_XPLMProcessingApi().FlightLoopCallback;
        EXPECT_NE(flight_loop_cb, nullptr);

        // TODO: test details of ig control message.
        EXPECT_CALL(*gep_proxy_, GEP_HandleSimulationControlMessages(NotNull(), NotNull(), 100));

        // TODO: emulate behavior of repeating flight loop calls
        int flight_loop_rv = flight_loop_cb(0.0f, 0.0f, 1, 0);
        EXPECT_NE(flight_loop_rv, 0);

    }

    TEST_F(XPluginTestFixture, TestXPluginFlightLoopCallbackForwardsCameraStatesToGep)
    {

        XPLMFlightLoop_f flight_loop_cb = processing_proxy_->get_XPLMProcessingApi().FlightLoopCallback;
        EXPECT_NE(flight_loop_cb, nullptr);

        // Set up XPLM Camera calls
        XPLMCameraPosition_t camera_position = {};
        EXPECT_CALL(*camera_proxy_, XPLMReadCameraPosition(_)).WillOnce(DoAll(SetArgPointee<0>(camera_position)));

        // TODO: test details of messages in/out.
        EXPECT_CALL(*gep_proxy_, GEP_HandleSimulationControlMessages(NotNull(), NotNull(), 100));

        int flight_loop_rv = flight_loop_cb(0.0f, 0.0f, 1, 0);
        EXPECT_NE(flight_loop_rv, 0);

    }
}