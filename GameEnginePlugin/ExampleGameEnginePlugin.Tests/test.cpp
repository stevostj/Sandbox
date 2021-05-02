#include "pch.h"
#include <windows.h>

namespace {

    typedef int(__stdcall* InitializeFunc)();       // Initialize function signature with calling convention

    class PluginTestFixture : public ::testing::Test {
    protected:
        void SetUp() override {

            std::wstring lib_path = getLibraryPath(L"\\ExampleGameEnginePlugin.dll");

            hGetProcIDDLL = ::LoadLibrary((LPWSTR)lib_path.c_str());
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
            wchar_t directory_buffer[256] = {};
            ::GetCurrentDirectory(256, directory_buffer);
            std::wstring lib_path(directory_buffer);
            lib_path.append(filename);
            return lib_path;
        }
    };

}

TEST_F(PluginTestFixture, TestInitialize) {

    // resolve function address here
    InitializeFunc initialize_func = (InitializeFunc) ::GetProcAddress(hGetProcIDDLL, "GEP_Initialize");
    EXPECT_NE(initialize_func, (InitializeFunc) 0); // Initialize function found

    int initialize_rv = initialize_func();
    EXPECT_EQ(initialize_rv, 0); // initialize ok

}