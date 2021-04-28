#include "pch.h"
#include <Windows.h>

namespace {

    typedef int(__stdcall* Initialize_t)();       // Initialize function signature with calling convention

    class PluginTestFixture : public ::testing::Test {
    protected:
        void SetUp() override {
            hGetProcIDDLL = ::LoadLibrary((LPCWSTR)".\\ExampleGameEnginePlugin.dll");
            EXPECT_NE(hGetProcIDDLL, (HINSTANCE)0); // library loaded
        }

        void TearDown() override {
            BOOL unload_rv = ::FreeLibrary(hGetProcIDDLL);
            EXPECT_TRUE(unload_rv); // library unloaded
        }

        HINSTANCE hGetProcIDDLL;
    };

}

TEST_F(PluginTestFixture, TestInitialize) {

    // resolve function address here
    Initialize_t initialize_func = (Initialize_t) ::GetProcAddress(hGetProcIDDLL, "Initialize");
    EXPECT_NE(initialize_func, (Initialize_t) 0); // Initialize function found

    int initialize_rv = initialize_func();
    EXPECT_EQ(initialize_rv, 0); // initialize ok

}