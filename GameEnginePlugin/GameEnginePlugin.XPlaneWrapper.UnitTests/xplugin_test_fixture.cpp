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
            SetXplmApiHooks();
            SetGepApiHooks();
            XPluginStart();
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

        void XPluginTestFixture::SetXplmApiHooks()
        {
            // Find function for hooking apis
            SetXplmApiHooksFunc setxplmapihooks_func = (SetXplmApiHooksFunc) ::GetProcAddress(hGetProcIDDLL, "SetXplmApiHooks");
            EXPECT_NE(setxplmapihooks_func, (SetXplmApiHooksFunc)0); // SetXplmApiHooks function found

            display_proxy_ = &gep_xpw_ut::MockXPLMDisplayProxy::get_instance();
            graphics_proxy_ = &gep_xpw_ut::MockXPLMGraphicsProxy::get_instance();
            processing_proxy_ = &gep_xpw_ut::MockXPLMProcessingProxy::get_instance();

            // setxplmapihooks returns error on null callbacks
            int setxplmapihooks_rv = setxplmapihooks_func(&(display_proxy_->get_XPLMDisplayApi()), &(graphics_proxy_->get_XPLMGraphicsApi()), &(processing_proxy_->get_XPLMProcessingApi()));
            EXPECT_EQ(setxplmapihooks_rv, -1);

            // Hook in alternative functions to XPLM APIs

            display_proxy_->get_XPLMDisplayApi().RegisterDrawCallback = display_proxy_->get_XPLMRegisterDrawCallbackHandler();
            display_proxy_->get_XPLMDisplayApi().GetScreenSize = display_proxy_->get_XPLMGetScreenSizeHandler();

            graphics_proxy_->get_XPLMGraphicsApi().SetGraphicsState = graphics_proxy_->get_XPLMSetGraphicsStateHandler();

            processing_proxy_->get_XPLMProcessingApi().RegisterFlightLoopCallback = processing_proxy_->get_XPLMRegisterFlightLoopCallbackHandler();

            setxplmapihooks_rv = setxplmapihooks_func(&(display_proxy_->get_XPLMDisplayApi()), &(graphics_proxy_->get_XPLMGraphicsApi()), &(processing_proxy_->get_XPLMProcessingApi()));
            EXPECT_EQ(setxplmapihooks_rv, SXPLMAH_INITIALIZE_OK); // setxplmapihooks ok

        }

        void XPluginTestFixture::SetGepApiHooks() 
        {
            // Find function for hooking apis
            SetGepApiHooksFunc setgepapihooks_func = (SetGepApiHooksFunc) ::GetProcAddress(hGetProcIDDLL, "SetGepApiHooks");
            EXPECT_NE(setgepapihooks_func, (SetGepApiHooksFunc)0); // SetGepApiHooks function found

            gep_proxy_ = &gep_xpw_ut::MockGEPProxy::get_instance();

            // setgepapihooks returns error on null callbacks
            int setgepapihooks_rv = setgepapihooks_func(&(gep_proxy_->get_GEPApi()));
            EXPECT_EQ(setgepapihooks_rv, -1);

            // Hook in alternative functions to GEP APIs
            gep_proxy_->get_GEPApi().Initialize = gep_proxy_->get_GEP_InitializeHandler();
            gep_proxy_->get_GEPApi().HandleStartOfFrameMessages = gep_proxy_->get_GEP_HandleStartOfFrameMessagesHandler();
            gep_proxy_->get_GEPApi().HandleSimulationControlMessages = gep_proxy_->get_GEP_HandleSimulationControlMessagesHandler();

            setgepapihooks_rv = setgepapihooks_func(&(gep_proxy_->get_GEPApi()));
            EXPECT_EQ(setgepapihooks_rv, SGEPAH_INITIALIZE_OK); // setgepapihooks ok
        
        }

        void XPluginTestFixture::XPluginStart() 
        {
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

    };