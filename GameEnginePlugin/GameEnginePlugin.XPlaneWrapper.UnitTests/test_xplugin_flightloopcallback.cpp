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
using ::testing::ElementsAreArray;
using ::testing::Contains;
using ::testing::Pointee;
using ::testing::Ge;
using ::testing::Ne;
using ::testing::Eq;
using ::testing::AllOf;
using ::testing::Field;
using ::testing::Truly;
using ::testing::Args;

namespace gep_xpw_ut {

    TEST_F(XPluginTestFixture, TestXPluginFlightLoopCallback)
    {

        XPLMFlightLoop_f flight_loop_cb = processing_proxy_->get_XPLMProcessingApi().FlightLoopCallback;
        EXPECT_NE(flight_loop_cb, nullptr);

        // TODO: test details of ig control message.
        EXPECT_CALL(*gep_proxy_, GEP_HandleSimulationControlMessages(NotNull(), 100, NotNull()));

        // TODO: emulate behavior of repeating flight loop calls
        float flight_loop_rv = flight_loop_cb(0.0f, 0.0f, 1, 0);
        EXPECT_EQ(flight_loop_rv, -1.0f); // Expect a return value that will schedule handling of the next flight loop.

    }

    bool ViewControlThatIsLockedToMainAircraftEntity(CigiControlPacket const & packet)
    {
        CIGI_VIEW_CONTROL const & view_control = packet.data.view_control;
        return (view_control.packet_size == CIGI_VIEW_CONTROL_SIZE && 
                view_control.packet_id == CIGI_VIEW_CONTROL_OPCODE &&
                view_control.view_id == 1 &&
                view_control.entity_id == 0 &&
                view_control.xoffset == 0.0f &&
                view_control.yoffset == 0.0f &&
                view_control.zoffset == 0.0f &&
                view_control.roll == 0.0f &&
                view_control.pitch == 0.0f &&
                view_control.yaw == 0.0f);
    }

    bool EntityControlThatIndicatesMainAircraftEntity(CigiControlPacket const& packet)
    {
        CIGI_ENTITY_CONTROL const& entity_control = packet.data.entity_control;
        return (entity_control.packet_size == CIGI_ENTITY_CONTROL_SIZE &&
            entity_control.packet_id == CIGI_ENTITY_CONTROL_OPCODE &&
            entity_control.entity_id == 0 && 
            entity_control.entity_state == CIGI_ACTIVE);
    }

    bool EntityPositionThatIndicatesMainAircraftCameraState(CigiControlPacket const& packet)
    {
        CIGI_ENTITY_POSITION const& entity_position = packet.data.entity_position;
        return (entity_position.packet_size == CIGI_ENTITY_POSITION_SIZE &&
            entity_position.packet_id == CIGI_ENTITY_POSITION_OPCODE &&
            entity_position.entity_id == 0 && 
            entity_position.lat_x == 0.0 && 
            entity_position.lon_y == 0.0 && 
            entity_position.alt_z == 0.0 && 
            entity_position.roll == 0.0f && 
            entity_position.pitch == 0.0f && 
            entity_position.yaw == 0.0f);
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

        // TODO: test details of ig control messages in more detail.
        CigiControlPacket expected_control_packet[3];
        CIGI_IG_CONTROL basic_ig_control = { CIGI_IG_CONTROL_SIZE, CIGI_IG_CONTROL_OPCODE };
        expected_control_packet[0].data.ig_control = basic_ig_control;

        CIGI_ENTITY_CONTROL expected_zero_entity_control =
        {
            CIGI_ENTITY_CONTROL_SIZE,
            CIGI_ENTITY_CONTROL_OPCODE
        };
        expected_zero_entity_control.entity_state = CIGI_ACTIVE;
        expected_zero_entity_control.collision_detect = CIGI_DISABLED;
        expected_zero_entity_control.inherit_alpha = CIGI_DISABLED;
        expected_zero_entity_control.smoothing_enable = CIGI_DISABLED;
        expected_zero_entity_control.extended_entity_type = CIGI_DISABLED;
        // 2 bits reserved
        expected_zero_entity_control.alpha = 0xFF; // Full opacity
        expected_zero_entity_control.entity_id = 0;
        expected_zero_entity_control.entity_kind = 0; // TODO: Support SISO-REF-010 entity types
        expected_zero_entity_control.entity_domain = 0;
        expected_zero_entity_control.entity_country_type = 0;
        expected_zero_entity_control.entity_category = 0;
        expected_zero_entity_control.entity_subcategory = 0;
        expected_zero_entity_control.entity_specific = 0;
        expected_zero_entity_control.entity_extra = 0;

        expected_control_packet[1].data.entity_control = expected_zero_entity_control;


        EXPECT_CALL(*gep_proxy_, GEP_HandleSimulationControlMessages(_, _, Pointee(Ge(2))))
            .With(Args<0, 1>(AllOf(
                Contains(Truly(ViewControlThatIsLockedToMainAircraftEntity)), 
                Contains(Truly(EntityControlThatIndicatesMainAircraftEntity)), 
                Contains(Truly(EntityPositionThatIndicatesMainAircraftCameraState)))))
            .WillOnce(Return(0));


        float flight_loop_rv = flight_loop_cb(0.0f, 0.0f, 1, 0);
        EXPECT_NE(flight_loop_rv, 0);

    }
}