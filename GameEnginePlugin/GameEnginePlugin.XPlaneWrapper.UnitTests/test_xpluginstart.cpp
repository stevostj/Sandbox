#include "pch.h"
#include <windows.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "set_xplm_api_hooks.h"

#include "xplmgraphics_proxy.h"
#include "xplmdisplay_proxy.h"
#include "xplmprocessing_proxy.h"

using ::testing::NotNull;
using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::Ge;
using ::testing::Ne;

namespace gep_xpw_ut {

    typedef int(__stdcall* XPluginStartFunc)(char *, char *, char *);

    typedef int(__stdcall* SetXplmApiHooksFunc)(XPLMDisplayApi, XPLMGraphicsApi, XPLMProcessingApi);

    /// <summary>
    /// Initializes objects and loads libraries necessary to test XPlugin functions.
    /// </summary>
    class XPluginTestFixture : public ::testing::Test {
     protected:

        void SetUp() override {

            LoadPlugin();

            SetApiHooks();

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
        gep_xpw_ut::MockXPLMProcessingProxy* processing_proxy_;

     private:

        /// <summary>
        /// Get a full path compatible with Google Test working directory and load library calls.
        /// </summary>
        /// <param name="filename">The name of the file to create a full path for. </param>
        /// <returns>The full compatible file path. </returns>
        std::wstring GetLibraryFullPath(std::wstring const & filename)
        {
            wchar_t directory_buffer[256] = {};
            ::GetCurrentDirectory(256, directory_buffer);
            std::wstring lib_path(directory_buffer);
            lib_path.append(filename);
            return lib_path;
        }

        void LoadPlugin() 
        {
            const std::wstring kXPlaneRelativePluginPath = L"\\plugins\\GameEnginePlugin.XPlaneWrapper\\64\\win.xpl";

            std::wstring lib_path = GetLibraryFullPath(kXPlaneRelativePluginPath);

            // check library loads ok
            const wchar_t* lib_cstr_path = lib_path.c_str();
            hGetProcIDDLL = ::LoadLibrary(lib_cstr_path);
            DWORD error_code = ::GetLastError(); // useful when debugging
            EXPECT_NE(hGetProcIDDLL, (HINSTANCE)0);
        }

        void SetApiHooks() 
        {
            // Find function for hooking apis
            SetXplmApiHooksFunc setxplmapihooks_func = (SetXplmApiHooksFunc) ::GetProcAddress(hGetProcIDDLL, "SetXplmApiHooks");
            EXPECT_NE(setxplmapihooks_func, (SetXplmApiHooksFunc)0); // SetXplmApiHooks function found

            // setxplmapihooks returns error on null callbacks
            XPLMDisplayApi display_api_hooks{};
            XPLMGraphicsApi graphics_api_hooks{};
            XPLMProcessingApi processing_api_hooks{};

            int setxplmapihooks_rv = setxplmapihooks_func(display_api_hooks, graphics_api_hooks, processing_api_hooks);
            EXPECT_EQ(setxplmapihooks_rv, -1);

            // Hook in alternative functions to XPLM APIs
            display_proxy_ = &gep_xpw_ut::MockXPLMDisplayProxy::get_instance();
            display_api_hooks.RegisterDrawCallback = display_proxy_->get_XPLMRegisterDrawCallbackHandler();
            display_api_hooks.GetScreenSize = display_proxy_->get_XPLMGetScreenSizeHandler();

            graphics_proxy_ = &gep_xpw_ut::MockXPLMGraphicsProxy::get_instance();
            graphics_api_hooks.SetGraphicsState = graphics_proxy_->get_XPLMSetGraphicsStateHandler();

            processing_proxy_ = &gep_xpw_ut::MockXPLMProcessingProxy::get_instance();
            processing_api_hooks.RegisterFlightLoopCallback = processing_proxy_->get_XPLMRegisterFlightLoopCallbackHandler();

            setxplmapihooks_rv = setxplmapihooks_func(display_api_hooks, graphics_api_hooks, processing_api_hooks);
            EXPECT_EQ(setxplmapihooks_rv, SXPLMAH_INITIALIZE_OK); // setxplmapihooks ok
        }
    };

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