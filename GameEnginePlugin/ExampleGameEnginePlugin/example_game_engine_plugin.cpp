#include "game_engine_plugin_api.h"

namespace {

	int GenerateSymbolSurface(CigiControlPacket* packets, int max_num_packets, int* num_packets)
	{

		CIGI_SYMBOL_SURFACE_DEF& surface = packets[*num_packets].data.symbol_surface_definition;
		surface.packet_size = CIGI_SYMBOL_SURFACE_DEF_SIZE;
		surface.packet_id = CIGI_SYMBOL_SURFACE_DEF_OPCODE;
		surface.entity_or_view_id = 0;
		surface.surface_state = CIGI_ACTIVE;
		surface.attach_type = 1; // attach to view
		surface.x_left = 0.0f; // top left is [0, 0], cartesian coordinates, optimized for 1024x768
		surface.y_right = 1.0f;
		surface.z_top = 1.0f;
		surface.yaw_bottom = 0.0f;
		surface.min_u = 0.0f; 
		surface.max_u = 1024.0f;
		surface.min_v = 0.0f;
		surface.max_v = 768.0f;

		*num_packets = *num_packets + 1;

		return GEP_MESSAGES_HANDLED_OK;
	}

	int GenerateHeadingArrow(CigiControlPacket* packets, int max_num_packets, int* num_packets)
	{
		static bool arrow_initialized = false;

		int original_num_packets = *num_packets;
		for (int i = 0; i < original_num_packets; ++i)
		{
			if (packets[i].data.entity_position.packet_id == CIGI_ENTITY_POSITION_OPCODE)
			{

				if (true) // (!arrow_initialized) FUTURE: don't redundantly draw symbols
				{
					CIGI_SYMBOL_POLYGON_DEF& main_line = packets[original_num_packets].data.symbol_polygon_definition;
					main_line.packet_size = CIGI_SYMBOL_POLYGON_DEF_BASE_SIZE + 2 * CIGI_SYMBOL_VERTEX_SIZE;
					main_line.packet_id = CIGI_SYMBOL_POLYGON_DEF_OPCODE;
					main_line.stipple_pattern = 0xFFFF; // solid line
					main_line.primitive_type = 1; // line
					main_line.line_width = 5.0f; // 5 pixels thick
					main_line.line[0].vertex_u = 100.0f; // 100 pixels long, in the upper left corner
					main_line.line[0].vertex_v = 100.0f;
					main_line.line[1].vertex_u = 100.0f;
					main_line.line[1].vertex_v = 200.0f;

					CIGI_SYMBOL_POLYGON_DEF& right_hook = packets[original_num_packets + 1].data.symbol_polygon_definition;
					right_hook.packet_size = CIGI_SYMBOL_POLYGON_DEF_BASE_SIZE + 2 * CIGI_SYMBOL_VERTEX_SIZE;
					right_hook.packet_id = CIGI_SYMBOL_POLYGON_DEF_OPCODE;
					right_hook.stipple_pattern = 0xFFFF; // solid line
					right_hook.primitive_type = 1; // line
					right_hook.line_width = 5.0f; // 5 pixels thick
					right_hook.line[0].vertex_u = 100.0f; // 14 pixels long, at a 45 deg angle to the main line
					right_hook.line[0].vertex_v = 100.0f;
					right_hook.line[1].vertex_u = 110.0f;
					right_hook.line[1].vertex_v = 110.0f;

					CIGI_SYMBOL_POLYGON_DEF& left_hook = packets[original_num_packets + 2].data.symbol_polygon_definition;
					left_hook.packet_size = CIGI_SYMBOL_POLYGON_DEF_BASE_SIZE + 2 * CIGI_SYMBOL_VERTEX_SIZE;
					left_hook.packet_id = CIGI_SYMBOL_POLYGON_DEF_OPCODE;
					left_hook.stipple_pattern = 0xFFFF; // solid line
					left_hook.primitive_type = 1; // line
					left_hook.line_width = 5.0f; // 5 pixels thick
					left_hook.line[0].vertex_u = 100.0f; // 14 pixels long, at a 45 deg angle to the main line
					left_hook.line[0].vertex_v = 100.0f;
					left_hook.line[1].vertex_u = 90.0f;
					left_hook.line[1].vertex_v = 110.0f;

					*num_packets += 3;

					arrow_initialized = true;
				}
				else
				{
					// FUTURE: add a symbol control instead
					// float heading = packets[i].data.entity_position.yaw;
				}



				break;
			}
		}

		return GEP_MESSAGES_HANDLED_OK;
	}
}

GEP_DECLSPEC int GEP_Initialize() 
{
	return GEP_INITIALIZE_OK;
}

GEP_DECLSPEC int GEP_HandleSimulationResponseMessages(CigiResponsePacket* packets, int max_num_packets, int* num_packets)
{
	// FUTURE: store response data for processing in the next simulation control cycle. 
	return GEP_MESSAGES_HANDLED_OK;
}

GEP_DECLSPEC int GEP_HandleSimulationControlMessages(CigiControlPacket* packets, int max_num_packets, int* num_packets)
{
	int rv = GEP_MESSAGES_HANDLED_OK;
	static bool surface_initialized = false;

	if (!surface_initialized) 
	{
		rv = GenerateSymbolSurface(packets, max_num_packets, num_packets);
		surface_initialized = true;
	}

	if(rv == GEP_MESSAGES_HANDLED_OK)
		rv = GenerateHeadingArrow(packets, max_num_packets, num_packets);

	return GEP_MESSAGES_HANDLED_OK;
}