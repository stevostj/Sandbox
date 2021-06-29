#include "pch.h"
#include <windows.h>

#include "plugin_test_fixture.h"

namespace {

    typedef int(__stdcall* InitializeFunc)();       // Initialize function signature with calling convention

}

namespace example_gep {
    TEST_F(PluginTestFixture, TestInitialize) {

        // resolve function address here
        InitializeFunc initialize_func = (InitializeFunc) ::GetProcAddress(hGetProcIDDLL, "GEP_Initialize");
        EXPECT_NE(initialize_func, (InitializeFunc) 0); // Initialize function found

        int initialize_rv = initialize_func();
        EXPECT_EQ(initialize_rv, 0); // initialize ok

    }
}