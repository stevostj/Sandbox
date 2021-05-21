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


        void XPluginTestFixture::SetUp() 
        {
            LoadPlugin();
            SetApiHooks();
        }

        void XPluginTestFixture::TearDown() 
        {
            // library unloaded ok
            BOOL unload_rv = ::FreeLibrary(hGetProcIDDLL);
            EXPECT_TRUE(unload_rv);

            display_proxy_->destroy();
        }


        /// <summary>
        /// Get a full path compatible with Google Test working directory and load library calls.
        /// </summary>
        /// <param name="filename">The name of the file to create a full path for. </param>
        /// <returns>The full compatible file path. </returns>
        std::wstring XPluginTestFixture::GetLibraryFullPath(std::wstring const& filename)
        {
            wchar_t directory_buffer[256] = {};
            ::GetCurrentDirectory(256, directory_buffer);
            std::wstring lib_path(directory_buffer);
            lib_path.append(filename);
            return lib_path;
        }


        void XPluginTestFixture::LoadPlugin()
        {
            const std::wstring kXPlaneRelativePluginPath = L"\\plugins\\GameEnginePlugin.XPlaneWrapper\\64\\win.xpl";

            std::wstring lib_path = GetLibraryFullPath(kXPlaneRelativePluginPath);

            // check library loads ok
            const wchar_t* lib_cstr_path = lib_path.c_str();
            hGetProcIDDLL = ::LoadLibrary(lib_cstr_path);
            DWORD error_code = ::GetLastError(); // useful when debugging
            EXPECT_NE(hGetProcIDDLL, (HINSTANCE)0);
        }

        void XPluginTestFixture::SetApiHooks()
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