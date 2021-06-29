#ifndef EXAMPLEGAMEENGINEPLUGINTESTS_PLUGINTESTFIXTURE_H_
#define EXAMPLEGAMEENGINEPLUGINTESTS_PLUGINTESTFIXTURE_H_

#include <windows.h>

#include "gtest/gtest.h"

namespace example_gep {

    class PluginTestFixture : public ::testing::Test {
    protected:
        void SetUp() override;

        void TearDown() override;

        HINSTANCE hGetProcIDDLL;

    private:

        /// <summary>
        /// Get a full path compatible with Google Test working directory and load library calls.
        /// </summary>
        /// <param name="filename">The name of the file to create a full path for. </param>
        /// <returns>The full compatible file path. </returns>
        std::wstring getLibraryPath(wchar_t* filename);
    };

}

#endif