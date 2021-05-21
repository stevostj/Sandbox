#include "pch.h"
#include <windows.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "set_xplm_api_hooks.h"

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


TEST_F(XPluginTestFixture, TestXPluginStartWithGEPPresent) {

    // resolve function address here
    XPluginStartFunc xpluginstart_func = (XPluginStartFunc) ::GetProcAddress(hGetProcIDDLL, "XPluginStart");
    EXPECT_NE(xpluginstart_func, (XPluginStartFunc)0); // XPluginStart function found

    // XpluginStart calls wires up the wrapper plugin to respond to callbacks
    EXPECT_CALL(*display_proxy_, XPLMRegisterDrawCallback(NotNull(), xplm_Phase_LastCockpit, 1 /*end of phase*/, _))
        .WillOnce(Return(1));

    EXPECT_CALL(*processing_proxy_, XPLMRegisterFlightLoopCallback(NotNull(), Ne(0.0f), _))
        .Times(1);

    // The screen size will be retrieved in order to scale symbology
    // TODO: why is this giving false positive when it is not being called?
    EXPECT_CALL(*display_proxy_, XPLMGetScreenSize(_, _))
        .WillOnce(DoAll(SetArgPointee<0>(100), SetArgPointee<1>(100)));

    char name[256] = {};
    char sig[256] = {};
    char desc[256] = {};

    int xpluginstart_rv = xpluginstart_func(name, sig, desc);
    EXPECT_EQ(xpluginstart_rv, 1); // xpluginstart ok

}

// TODO: Test failure condition by deleting game engine plugin file(s)
//TEST_F(PluginTestFixture, TestXPluginStartGEPNotPresent) {
//
//    // resolve function address here
//    XPluginStartFunc xpluginstart_func = (XPluginStartFunc) ::GetProcAddress(hGetProcIDDLL, "XPluginStart");
//    EXPECT_NE(xpluginstart_func, (XPluginStartFunc)0); // XPluginStart function found
//
//    char name[256] = {};
//    char sig[256] = {};
//    char desc[256] = {};
//
//    int xpluginstart_rv = xpluginstart_func(name, sig, desc);
//    EXPECT_EQ(xpluginstart_rv, 0); // xpluginstart failure
//
//}

}