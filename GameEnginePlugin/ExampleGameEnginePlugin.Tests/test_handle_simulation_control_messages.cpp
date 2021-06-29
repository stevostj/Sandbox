#include "pch.h"
#include <windows.h>

#include "plugin_test_fixture.h"

#include "game_engine_plugin_api.h"

namespace {

    typedef int(__stdcall* HandleSimulationControlMessagesFunc)(CigiControlPacket*, int, int*); // GEP_HandleSimulationControlMessages function signature with calling convention

}

namespace example_gep {

    /// <summary>
    /// TODO: Move this into the fixture so that tests can be broken into smaller chunks in the future without redundantly
    ///  loading the plugin. 
    /// </summary>
    TEST_F(PluginTestFixture, TestHandleSimulationControlMessages) 
    {

        // resolve function address here
        HandleSimulationControlMessagesFunc handlesimulationcontrolmessages_func = 
            (HandleSimulationControlMessagesFunc) ::GetProcAddress(hGetProcIDDLL, "GEP_HandleSimulationControlMessages");
        EXPECT_NE(handlesimulationcontrolmessages_func, (HandleSimulationControlMessagesFunc)0); // function found


        // IN PROGRESS: add test to check that the 'RADAR' has the correct symbols drawn

    }
}