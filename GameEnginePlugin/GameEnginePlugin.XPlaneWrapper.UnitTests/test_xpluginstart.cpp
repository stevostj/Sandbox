#include "pch.h"
#include <windows.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "set_xplm_api_hooks.h"

#include "xplmgraphics_proxy.h"
#include "xplmdisplay_proxy.h"

using ::testing::NotNull;
using ::testing::_;
using ::testing::Return;

namespace {

    typedef int(__stdcall* XPluginStartFunc)(char *, char *, char *);

    typedef int(__stdcall* SetXplmApiHooksFunc)(XplmDisplayRegisterDrawCallbackFunc);

        class PluginTestFixture : public ::testing::Test {
    protected:
        void SetUp() override {

            const std::wstring kXPlaneRelativePluginPath = L"\\plugins\\GameEnginePlugin.XPlaneWrapper\\64\\win.xpl";

            std::wstring lib_path = getLibraryFullPath(kXPlaneRelativePluginPath);

            // check library loads ok
            const wchar_t* lib_cstr_path = lib_path.c_str();
            hGetProcIDDLL = ::LoadLibrary(lib_cstr_path);
            DWORD error_code = ::GetLastError(); // useful when debugging
            EXPECT_NE(hGetProcIDDLL, (HINSTANCE)0);

            // Find function for hooking apis
            SetXplmApiHooksFunc setxplmapihooks_func = (SetXplmApiHooksFunc) ::GetProcAddress(hGetProcIDDLL, "SetXplmApiHooks");
            EXPECT_NE(setxplmapihooks_func, (SetXplmApiHooksFunc)0); // SetXplmApiHooks function found

            // setxplmapihooks returns error on null callback
            int setxplmapihooks_rv = setxplmapihooks_func(nullptr);
            EXPECT_EQ(setxplmapihooks_rv, -1); 


            // Hook in alternative functions to XPLM APIs
            display_proxy_ = &gep_xpw_ut::MockXPLMDisplayProxy::get_instance();
            setxplmapihooks_rv = setxplmapihooks_func(display_proxy_->get_XPLMRegisterDrawCallbackHandler());
            EXPECT_EQ(setxplmapihooks_rv, SXPLMAH_INITIALIZE_OK); // setxplmapihooks ok

        }

        void TearDown() override {

            // library unloaded ok
            BOOL unload_rv = ::FreeLibrary(hGetProcIDDLL);
            EXPECT_TRUE(unload_rv);

            display_proxy_->destroy();
        }

        HINSTANCE hGetProcIDDLL = 0;

        gep_xpw_ut::MockXPLMDisplayProxy * display_proxy_;

        gep_xpw_ut::MockXPLMGraphicsProxy* graphics_proxy_;

    private:

        /// <summary>
        /// Get a full path compatible with Google Test working directory and load library calls.
        /// </summary>
        /// <param name="filename">The name of the file to create a full path for. </param>
        /// <returns>The full compatible file path. </returns>
        std::wstring getLibraryFullPath(std::wstring const & filename)
        {
            wchar_t directory_buffer[256] = {};
            ::GetCurrentDirectory(256, directory_buffer);
            std::wstring lib_path(directory_buffer);
            lib_path.append(filename);
            return lib_path;
        }
    };

}

TEST_F(PluginTestFixture, TestXPluginStartGEPPresent) {

    // resolve function address here
    XPluginStartFunc xpluginstart_func = (XPluginStartFunc) ::GetProcAddress(hGetProcIDDLL, "XPluginStart");
    EXPECT_NE(xpluginstart_func, (XPluginStartFunc)0); // XPluginStart function found

    // XpluginStart calls wires up the wrapper plugin to respond to drawing callbacks
    EXPECT_CALL(*display_proxy_, XPLMRegisterDrawCallback(NotNull(), xplm_Phase_LastCockpit, 1 /*end of phase*/, _))
        .WillOnce(Return(1));

    EXPECT_CALL(*graphics_proxy_, XPLMSetGraphicsState(0, _, 0, _, _, _, _))
        .Times(1);

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
