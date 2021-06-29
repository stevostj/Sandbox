#include <windows.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "helper_methods.h"
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

        // FUTURE: test details of ig control message.
        EXPECT_CALL(*gep_proxy_, GEP_HandleSimulationControlMessages(NotNull(), 100, NotNull()));

        // FUTURE: emulate behavior of repeating flight loop calls
        float flight_loop_rv = flight_loop_cb(0.0f, 0.0f, 1, 0);
        EXPECT_EQ(flight_loop_rv, -1.0f); // Expect a return value that will schedule handling of the next flight loop.

    }

    bool IgControlThatIsValid(CigiControlPacket const& packet)
    {
        CIGI_IG_CONTROL const& ig_control = packet.data.ig_control;
        return (ig_control.packet_size == CIGI_IG_CONTROL_SIZE &&
            ig_control.packet_id == CIGI_IG_CONTROL_OPCODE &&
            ig_control.ig_mode == IG_CONTROL_IG_MODE_OPERATE);
    }

    bool EntityControlThatIndicatesMainAircraftEntity(CigiControlPacket const& packet)
    {
        CIGI_ENTITY_CONTROL const& entity_control = packet.data.entity_control;
        return (entity_control.packet_size == CIGI_ENTITY_CONTROL_SIZE &&
            entity_control.packet_id == CIGI_ENTITY_CONTROL_OPCODE &&
            entity_control.entity_id == 0 && 
            entity_control.entity_state == CIGI_ACTIVE);
    }

    // FUTURE: do a conversion from local plane coordinates to geodetic
    bool EntityPositionThatIndicatesMainAircraftCameraState(CigiControlPacket const& packet)
    {
        static const double kToleranceDouble = 0.001;
        static const float kTolerance = 0.001;
        CIGI_ENTITY_POSITION const& entity_position = packet.data.entity_position;
        return (entity_position.packet_size == CIGI_ENTITY_POSITION_SIZE &&
            entity_position.packet_id == CIGI_ENTITY_POSITION_OPCODE &&
            entity_position.entity_id == 0 && 
            ApproximatelyEqual(entity_position.lat_x, 12.34, kToleranceDouble) &&
            ApproximatelyEqual(entity_position.lon_y, 56.78, kToleranceDouble) &&
            ApproximatelyEqual(entity_position.alt_z, 9.0, kToleranceDouble) &&
            ApproximatelyEqual(entity_position.roll, 78.9f, kTolerance) &&
            ApproximatelyEqual(entity_position.pitch, 12.3f, kTolerance) &&
            ApproximatelyEqual(entity_position.yaw, 45.6f, kTolerance));
    }

    bool ViewControlThatIsLockedToMainAircraftEntity(CigiControlPacket const& packet)
    {
        CIGI_VIEW_CONTROL const& view_control = packet.data.view_control;
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

    TEST_F(XPluginTestFixture, TestXPluginFlightLoopCallbackForwardsCameraStatesToGep)
    {

        XPLMFlightLoop_f flight_loop_cb = processing_proxy_->get_XPLMProcessingApi().FlightLoopCallback;
        EXPECT_NE(flight_loop_cb, nullptr);

        // Set up XPLM Camera calls
        // TODO: Test position in each +- x/y/z combination
        XPLMCameraPosition_t zero_camera_position;
        zero_camera_position.x = 12.34f;
        zero_camera_position.y = 56.78f;
        zero_camera_position.z = 9.0f;
        zero_camera_position.pitch = 12.3f;
        zero_camera_position.heading = 45.6f;
        zero_camera_position.roll = 78.9f;
        zero_camera_position.zoom = 1.0f;

        EXPECT_CALL(*camera_proxy_, XPLMReadCameraPosition(NotNull())).WillOnce(DoAll(SetArgPointee<0>(zero_camera_position)));


        EXPECT_CALL(*gep_proxy_, GEP_HandleSimulationControlMessages(NotNull(), Ge(4), Pointee(Ge(4))))
            .With(Args<0, 1>(AllOf(
                Contains(Truly(IgControlThatIsValid)), 
                Contains(Truly(ViewControlThatIsLockedToMainAircraftEntity)), 
                Contains(Truly(EntityControlThatIndicatesMainAircraftEntity)), 
                Contains(Truly(EntityPositionThatIndicatesMainAircraftCameraState)))))
            .WillOnce(Return(0));

        float flight_loop_rv = flight_loop_cb(0.0f, 0.0f, 1, 0);
        EXPECT_NE(flight_loop_rv, 0);

    }
}