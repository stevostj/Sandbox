#include "pch.h"
#include <windows.h>

#include "plugin_test_fixture.h"

namespace example_gep {

    void PluginTestFixture::SetUp() {

        std::wstring lib_path = getLibraryPath(L"\\ExampleGameEnginePlugin.dll");

        hGetProcIDDLL = ::LoadLibrary((LPWSTR)lib_path.c_str());
        EXPECT_NE(hGetProcIDDLL, (HINSTANCE)0); // library loaded
    }

    void PluginTestFixture::TearDown() {
        BOOL unload_rv = ::FreeLibrary(hGetProcIDDLL);
        EXPECT_TRUE(unload_rv); // library unloaded
    }

    /// <summary>
    /// Get a full path compatible with Google Test working directory and load library calls.
    /// </summary>
    /// <param name="filename">The name of the file to create a full path for. </param>
    /// <returns>The full compatible file path. </returns>
    std::wstring PluginTestFixture::getLibraryPath(wchar_t* filename)
    {
        wchar_t directory_buffer[256] = {};
        ::GetCurrentDirectory(256, directory_buffer);
        std::wstring lib_path(directory_buffer);
        lib_path.append(filename);
        return lib_path;
    }

}