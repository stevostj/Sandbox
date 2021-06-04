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
		CIGI_HAT_HOT_RESPONSE hat_hot_response;
		// TODO: Add complete set of CIGI reponse packets
	} data;
};

#define GEP_INITIALIZE_OK 0

GEP_DECLSPEC int GEP_Initialize();

GEP_DECLSPEC int GEP_HandleStartOfFrameMessages(char ** messages_in, char ** messages_out);

GEP_DECLSPEC int GEP_HandleSimulationControlMessages(CigiControlPacket * packets, int max_num_packets, short * num_packets);


#ifdef __cplusplus
}
#endif

#endif