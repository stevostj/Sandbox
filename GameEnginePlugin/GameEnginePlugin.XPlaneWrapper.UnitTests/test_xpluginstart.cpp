#include "pch.h"
#include <windows.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "..\GameEnginePlugine.XPlaneWrapper\xplmdisplay_proxy.h"

namespace {

    typedef int(__stdcall* XPluginStartFunc)(char *, char *, char *);       // XPluginStart function signature with calling convention

    class PluginTestFixture : public ::testing::Test {
    protected:
        void SetUp() override {

            std::wstring kXPlanePluginPath = L"\\plugins\\GameEnginePlugin.XPlaneWrapper\\64\\win.xpl";
            std::wstring lib_path = getLibraryPath(kXPlanePluginPath);

            hGetProcIDDLL = ::LoadLibrary((LPWSTR)lib_path.c_str());
            EXPECT_NE(hGetProcIDDLL, (HINSTANCE)0); // library loaded
        }

        void TearDown() override {
            BOOL unload_rv = ::FreeLibrary(hGetProcIDDLL);
            EXPECT_TRUE(unload_rv); // library unloaded
        }

        HINSTANCE hGetProcIDDLL = 0;

    private:

        /// <summary>
        /// Get a full path compatible with Google Test working directory and load library calls.
        /// </summary>
        /// <param name="filename">The name of the file to create a full path for. </param>
        /// <returns>The full compatible file path. </returns>
        std::wstring getLibraryPath(std::wstring const & filename)
        {
            wchar_t directory_buffer[256] = {};
            ::GetCurrentDirectory(256, directory_buffer);
            std::wstring lib_path(directory_buffer);
            lib_path.append(filename);
            return lib_path;
        }
    };

    class MockXPLMDisplayProxy : public XPLMDisplayProxy {
     public:
         MOCK_METHOD(void, XPLMGetMouseLocation, (int *outX, int* outY), (override));
    };

}

TEST_F(PluginTestFixture, TestXPluginStartGEPPresent) {

    // resolve function address here
    XPluginStartFunc xpluginstart_func = (XPluginStartFunc) ::GetProcAddress(hGetProcIDDLL, "XPluginStart");
    EXPECT_NE(xpluginstart_func, (XPluginStartFunc)0); // XPluginStart function found

    char name[256] = {};
    char sig[256] = {};
    char desc[256] = {};

    int xpluginstart_rv = xpluginstart_func(name, sig, desc);
    EXPECT_EQ(xpluginstart_rv, 1); // xpluginstart ok

    // Expected Xplugin calls are made to wire up symbology to the game engine plugin

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