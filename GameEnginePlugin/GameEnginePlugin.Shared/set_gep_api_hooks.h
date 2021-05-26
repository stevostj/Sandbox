#ifndef GAMEENGINEPLUGINSHARED_SETGEPAPIHOOKS_H_
#define GAMEENGINEPLUGINSHARED_SETGEPAPIHOOKS_H_

#ifdef _EXPORTSGEPAH
#define SGEPAH_DECLSPEC    __declspec(dllexport)
#else
#define SGEPAH_DECLSPEC    __declspec(dllimport)
#endif

#define SGEPAH_INITIALIZE_OK 0

#include "game_engine_plugin_api.h"

// C/C++ naming
#ifdef __cplusplus
extern "C" {
#endif

	// Function signatures and pointers to GEP API
	struct GEPApi {

		typedef int(__stdcall* InitializeFunc)();
		InitializeFunc Initialize;

		typedef int(__stdcall* HandleStartOfFrameMessagesFunc)(char**, char**);
		HandleStartOfFrameMessagesFunc HandleStartOfFrameMessages;

		typedef int (__stdcall* HandleSimulationControlMessagesFunc)(CigiControlPacket*, short*, short);
		HandleSimulationControlMessagesFunc HandleSimulationControlMessages;

	};


	/// <summary>
	/// Passes in alternate pointers to functions to be called instead of the various GEP functions.
	/// </summary>
	/// <param name="gep_api_hooks">Group of pointers to GEP functions. </param>
	SGEPAH_DECLSPEC int SetGepApiHooks(GEPApi* gep_api_hooks);

#ifdef __cplusplus
}
#endif

#endif