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
using ::testing::SetArrayArgument;
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
        EXPECT_EQ(flight_loop_rv, -1.0f); // Expect a return value that will schedule handling of the next flight loop.

    }

    TEST_F(XPluginTestFixture, TestXPluginFlightLoopCallbackForwardsCameraStatesToGep)
    {

        XPLMFlightLoop_f flight_loop_cb = processing_proxy_->get_XPLMProcessingApi().FlightLoopCallback;
        EXPECT_NE(flight_loop_cb, nullptr);

        // Set up XPLM Camera calls
        XPLMCameraPosition_t zero_camera_position;
        zero_camera_position.x = 0.0f;
        zero_camera_position.y = 0.0f;
        zero_camera_position.z = 0.0f;
        zero_camera_position.pitch = 0.0f;
        zero_camera_position.heading = 0.0f;
        zero_camera_position.roll = 0.0f;
        zero_camera_position.zoom = 1.0f;

        EXPECT_CALL(*camera_proxy_, XPLMReadCameraPosition(_)).WillOnce(DoAll(SetArgPointee<0>(zero_camera_position)));

        // TODO: test details of messages in/out.
        CigiControlPacket expected_control_packet[2];
        CIGI_IG_CONTROL basic_ig_control = { CIGI_IG_CONTROL_SIZE, CIGI_IG_CONTROL_OPCODE };
        expected_control_packet[0].data.ig_control = basic_ig_control;

        CIGI_VIEW_CONTROL expected_zero_view_control =
        {
            CIGI_VIEW_CONTROL_SIZE,
            CIGI_VIEW_CONTROL_OPCODE,
        };
        expected_zero_view_control.group_id = 0;
        expected_zero_view_control.xoffset_enable = 1;
        expected_zero_view_control.yoffset_enable = 1;
        expected_zero_view_control.zoffset_enable = 1;
        expected_zero_view_control.roll_enable = 1;
        expected_zero_view_control.pitch_enable = 1;
        expected_zero_view_control.yaw_enable = 1;
        // 2  reserved bits
        expected_zero_view_control.view_id = 1;
        expected_zero_view_control.entity_id = 0;
        // 16 reserved bits
        expected_zero_view_control.xoffset = 0.0f;
        expected_zero_view_control.yoffset = 0.0f;
        expected_zero_view_control.zoffset = 0.0f;
        expected_zero_view_control.roll = 0.0f;
        expected_zero_view_control.pitch = 0.0f;
        expected_zero_view_control.yaw = 0.0f;
        // 32 reserved bits

        expected_control_packet[1].data.view_control = expected_zero_view_control;

        EXPECT_CALL(*gep_proxy_, GEP_HandleSimulationControlMessages(NotNull(), NotNull(), 100))
            .WillOnce(DoAll(SetArrayArgument<0>(expected_control_packet, expected_control_packet + 2), Return(0)));

        int flight_loop_rv = flight_loop_cb(0.0f, 0.0f, 1, 0);
        EXPECT_NE(flight_loop_rv, 0);

    }
}