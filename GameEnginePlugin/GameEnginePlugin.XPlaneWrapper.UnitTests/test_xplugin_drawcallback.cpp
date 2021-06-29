#include <windows.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "xplmgraphics_proxy.h"
#include "xplmdisplay_proxy.h"
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

    bool StartOfFrameThatIsValid(CigiResponsePacket const& packet)
    {
        CIGI_START_OF_FRAME const& start_of_frame = packet.data.start_of_frame;
        return (start_of_frame.packet_size == CIGI_START_OF_FRAME_SIZE &&
            start_of_frame.packet_id == CIGI_START_OF_FRAME_OPCODE);
    }
    TEST_F(XPluginTestFixture, TestXPluginDrawCallback)
    {

        // Each time XPlane triggers draw callbacks, the XPLMGraphics SetGraphicsState function will be called
        // to 'reset the canvas'. 
        EXPECT_CALL(*graphics_proxy_, XPLMSetGraphicsState(0, _, 0, _, _, _, _));

        // draw callbacks will trigger a start of frame to the game engine plugins
        EXPECT_CALL(*gep_proxy_, GEP_HandleSimulationResponseMessages(NotNull(), Ge(1), Pointee(Ge(1))))
            .With(Args<0, 1>(Contains(Truly(StartOfFrameThatIsValid))))
            .WillOnce(Return(0));

        XPLMDrawCallback_f draw_cb = display_proxy_->get_XPLMDisplayApi().DrawCallback;
        EXPECT_NE(draw_cb, nullptr);

        int draw_rv = draw_cb(xplm_Phase_LastCockpit, 0, nullptr);
        EXPECT_NE(draw_rv, 0);
    }
}