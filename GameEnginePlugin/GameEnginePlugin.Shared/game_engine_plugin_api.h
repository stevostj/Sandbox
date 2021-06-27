#ifndef _GEPAPI_H_
#define _GEPAPI_H_

#ifdef _EXPORTGEP
#define GEP_DECLSPEC    __declspec(dllexport)
#else
#define GEP_DECLSPEC    __declspec(dllimport)
#endif

#include "cigi4types.h"

// C/C++ naming
#ifdef __cplusplus
extern "C" {
#endif

struct CigiControlPacket {
	union CigiControlPacketData {
		CIGI_IG_CONTROL ig_control;
		CIGI_ENTITY_CONTROL entity_control;
		CIGI_ENTITY_POSITION entity_position;
		CIGI_VIEW_CONTROL view_control;
		// TODO: Add complete set of CIGI control packets
	} data; 
};

struct CigiResponsePacket {
	union {
		CIGI_START_OF_FRAME start_of_frame;
		// TODO: Add complete set of CIGI reponse packets
	} data;
};

#define GEP_INITIALIZE_OK 0

#define GEP_MESSAGES_HANDLED_OK 0

GEP_DECLSPEC int GEP_Initialize();

GEP_DECLSPEC int GEP_HandleSimulationResponseMessages(CigiResponsePacket * packets, int max_num_packets, int * num_packets);

GEP_DECLSPEC int GEP_HandleSimulationControlMessages(CigiControlPacket * packets, int max_num_packets, int * num_packets);


#ifdef __cplusplus
}
#endif

#endif