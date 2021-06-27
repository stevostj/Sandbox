#include "game_engine_plugin_api.h"

GEP_DECLSPEC int GEP_Initialize() 
{
	return GEP_INITIALIZE_OK;
}

GEP_DECLSPEC int GEP_HandleSimulationResponseMessages(CigiResponsePacket* packets, int max_num_packets, int* num_packets)
{
	return GEP_MESSAGES_HANDLED_OK;
}

GEP_DECLSPEC int GEP_HandleSimulationControlMessages(CigiControlPacket* packets, int max_num_packets, int* num_packets)
{
	return GEP_MESSAGES_HANDLED_OK;
}