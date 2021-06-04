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

}