#ifndef GAMEENGINEPLUGINXPLANEWRAPPERUNITTESTS_XPLUGINTESTFIXTURE_H_
#define GAMEENGINEPLUGINXPLANEWRAPPERUNITTESTS_XPLUGINTESTFIXTURE_H_

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "xplmgraphics_proxy.h"
#include "xplmdisplay_proxy.h"
#include "xplmprocessing_proxy.h"
#include "xplmcamera_proxy.h"
#include "gep_proxy.h"

namespace gep_xpw_ut {


    typedef int(__stdcall* XPluginStartFunc)(char*, char*, char*);

    typedef int(__stdcall* SetXplmApiHooksFunc)(XPLMDisplayApi *, XPLMGraphicsApi *, XPLMProcessingApi *);

    typedef int(__stdcall* SetGepApiHooksFunc)(GEPApi*);


    /// <summary>
    /// Initializes objects and loads libraries necessary to test XPlugin functions.
    /// </summary>
    class XPluginTestFixture : public ::testing::Test {
     protected:

        void SetUp() override;

        void TearDown() override;

        HINSTANCE hGetProcIDDLL = 0;

        gep_xpw_ut::MockXPLMDisplayProxy* display_proxy_;
        gep_xpw_ut::MockXPLMGraphicsProxy* graphics_proxy_;
        gep_xpw_ut::MockXPLMProcessingProxy* processing_proxy_;
        gep_xpw_ut::MockXPLMCameraProxy* camera_proxy_;
        gep_xpw_ut::MockGEPProxy* gep_proxy_;

     private:

        std::wstring GetLibraryFullPath(std::wstring const& filename);

        void LoadPlugin();

        void SetXplmApiHooks();

        void SetGepApiHooks();

        void XPluginStart();

    };

}

#endif