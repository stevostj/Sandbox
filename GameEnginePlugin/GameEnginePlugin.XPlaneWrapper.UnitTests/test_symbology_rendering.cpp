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

    TEST_F(XPluginTestFixture, TestSymbologyLineRendering)
    {
        XPLMFlightLoop_f flight_loop_cb = processing_proxy_->get_XPLMProcessingApi().FlightLoopCallback;
        EXPECT_NE(flight_loop_cb, nullptr);

        // Emulate a plugin adding minimal symbology to draw a line
        // At a minimum, this would require a Symbol Surface and a Symbol Polygon packet.
        CigiControlPacket const kDontCareIgControl = {};
        CigiControlPacket const kDontCareEntityControl = {};
        CigiControlPacket const kDontCareEntityPosition = {};
        CigiControlPacket const kDontCareViewControl = {};

        CigiControlPacket fullScreenSymbolSurface;
        CIGI_SYMBOL_SURFACE_DEF& surface = fullScreenSymbolSurface.data.symbol_surface_definition;
        surface.packet_size = CIGI_SYMBOL_SURFACE_DEF_SIZE;
        surface.packet_id = CIGI_SYMBOL_SURFACE_DEF_OPCODE;
        surface.entity_or_view_id = 0;
        surface.surface_state = CIGI_ACTIVE;
        surface.attach_type = 1; // attach to view
        surface.x_left = 0.0f;
        surface.y_right = 1.0f;
        surface.z_top = 1.0f;
        surface.yaw_bottom = 0.0f;
        surface.min_u = 0.0f;
        surface.max_u = 1920.f;
        surface.min_v = 0.0f;
        surface.max_v = 1080.0f;

        CigiControlPacket verticalLine;
        CIGI_SYMBOL_POLYGON_DEF& line = verticalLine.data.symbol_polygon_definition;
        line.packet_size = CIGI_SYMBOL_POLYGON_DEF_PRACTICAL_SIZE;
        line.packet_id = CIGI_SYMBOL_POLYGON_DEF_OPCODE;
        line.stipple_pattern = 0xFFFF; // solid line
        line.primitive_type = 1; // line
        line.line_width = 1.0f; // 1 pixel thick
        line.line[0].vertex_u = 1000.0f; // starts at bottom, goes up 1000 pixels
        line.line[0].vertex_v = 0.0f;
        line.line[1].vertex_u = 1000.0f;
        line.line[1].vertex_v = 1000.0f;

        int const kExpectedNumSymbols = 6;
        CigiControlPacket line_symbols[] =
        {
            kDontCareIgControl,
            kDontCareEntityControl,
            kDontCareEntityPosition,
            kDontCareViewControl, 
            fullScreenSymbolSurface, 
            verticalLine
        };

        ON_CALL(*gep_proxy_, GEP_HandleSimulationControlMessages(NotNull(), Ge(kExpectedNumSymbols), Pointee(Ge(4))))
            .WillByDefault(DoAll(
                SetArrayArgument<0>(line_symbols, line_symbols + kExpectedNumSymbols),
                SetArgPointee<2>(kExpectedNumSymbols), // TODO: make this additive to the value of num_packets
                Return(GEP_MESSAGES_HANDLED_OK)));

        float flight_loop_rv = flight_loop_cb(0.0f, 0.0f, 1, 0);
        EXPECT_EQ(flight_loop_rv, -1.0f); // Expect a return value that will schedule handling of the next flight loop.

        // FUTURE: Test that a vertical line has actually been rendered to a 'screen'. 
        // Will involve initializing OpenGL in the unit test project, which is a rabbit hole
        // I don't particularly go down at this point in time. If I were getting paid to do this, absolutely. 

    }

}