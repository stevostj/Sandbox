#include "pch.h"
#include <windows.h>

namespace {

    typedef int(__stdcall* Initialize_t)();       // Initialize function signature with calling convention

    class PluginTestFixture : public ::testing::Test {
    protected:
        void SetUp() override {

            std::wstring libPath = getLibraryPath(L"\\ExampleGameEnginePlugin.dll");

            hGetProcIDDLL = ::LoadLibrary((LPWSTR)libPath.c_str());
            EXPECT_NE(hGetProcIDDLL, (HINSTANCE)0); // library loaded
        }

        void TearDown() override {
            BOOL unload_rv = ::FreeLibrary(hGetProcIDDLL);
            EXPECT_TRUE(unload_rv); // library unloaded
        }

        HINSTANCE hGetProcIDDLL;

    private:

        /// <summary>
        /// Get a full path compatible with Google Test working directory and load library calls.
        /// </summary>
        /// <param name="filename">The name of the file to create a full path for. </param>
        /// <returns>The full compatible file path. </returns>
        std::wstring getLibraryPath(wchar_t * filename)
        {
            wchar_t dirBuffer[256] = {};
            ::GetCurrentDirectory(256, dirBuffer);
            std::wstring libPath(dirBuffer);
            libPath.append(filename);
            return libPath;
        }
    };

}

TEST_F(PluginTestFixture, TestInitialize) {

    // resolve function address here
    Initialize_t initialize_func = (Initialize_t) ::GetProcAddress(hGetProcIDDLL, "Initialize");
    EXPECT_NE(initialize_func, (Initialize_t) 0); // Initialize function found

    int initialize_rv = initialize_func();
    EXPECT_EQ(initialize_rv, 0); // initialize ok

}