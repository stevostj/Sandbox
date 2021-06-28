/** <pre>
 *  Copyright 2004 The Boeing Company
 *  Author: Lance Durham
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2.1 of the License, or (at
 *  your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * </pre>
 */

#ifndef _CIGI4TYPES_H_
#define _CIGI4TYPES_H_

#include "stdint.h"

#define CIGI_MAX_PACKET_SIZE       256  /* The largest allowable packet size */
#define CIGI_OPCODE_NONE        0x0f00  /* chas - Undefined opcode */
#define CIGI_VERSION                 4  /* chas - CIGI version number */

#define CIGI_DISABLED                0  /* Used to indicate a disabled state */
#define CIGI_ENABLED                 1  /* Used to indicate an enabled state */

#define CIGI_INACTIVE                0  /* Used to indicate an inactive state */
#define CIGI_ACTIVE                  1  /* Used to indicate an active state */

#define CIGI_INVALID                 0  /* Used to indicate an invalid state */
#define CIGI_VALID                   1  /* Used to indicate a valid state */

// C/C++ naming
#ifdef __cplusplus
extern "C" {
#endif

    struct CIGI_DUMMY_PACKET {
        uint16_t packet_size;
        uint16_t packet_id;
        uint16_t data1;
        uint16_t data2;
    };

#define CIGI_DUMMY_PACKET_OPCODE                0x0f00 //  chas
#define CIGI_DUMMY_PACKET_SIZE                  8

/********************************************************************************************/

    // CIGI IG Control
    struct CIGI_IG_CONTROL {
        uint16_t packet_size;                        /** Packet size = 24 bytes             */
        uint16_t packet_id;                          /** Packet ID = 0                      */
        uint8_t cigi_version;                       /** CIGI major version                 */
        int8_t db_number;                           /** Database number                    */
		uint8_t EntityTypeSubEn : 1;				//* chas Enitiy Type Substitution Enable */
		uint8_t					:7;
        uint8_t ig_mode                : 2;    /** IG mode control                    */
        uint8_t timestamp_valid        : 1;    /** Timestamp valid flag               */
        uint8_t extrap_interpol_enable : 1;    /** Extrapolation/interpolation enable */
        uint8_t minor_version          : 4;    /** CIGI minor version                 */
        uint32_t host_frame_num;                    /** Host frame number                  */
        uint32_t last_ig_frame_num;                 /** Last IG frame number               */
        uint32_t timestamp;                         /** Timing value/tag                   */
        unsigned int                        : 32;   /** Reserved                           */
    };

#define CIGI_IG_CONTROL_OPCODE                  0
#define CIGI_IG_CONTROL_SIZE                    24

    // IG mode
#define IG_CONTROL_IG_MODE_STANDBY              0
#define IG_CONTROL_IG_MODE_OPERATE              1
#define IG_CONTROL_IG_MODE_DEBUG                2

    // Byteswap magic numbers
// chas #define IG_CONTROL_SWAP                         0x0080
// chas #define IG_CONTROL_NOSWAP                       0x8000
#define IG_CONTROL_SWAP                         0x00ff
#define IG_CONTROL_NOSWAP                       0xff00

/********************************************************************************************/

    // CIGI Entity Position
    struct CIGI_ENTITY_POSITION {
        uint16_t packet_size;							/** Packet size = 48 bytes      */
        uint16_t packet_id;								/** Packet ID = 1               */
        unsigned char attach_state				: 1;	/** entity attach state      */
        unsigned char clamp_mode				: 2;	/** Ground/ocean clamp         */
		unsigned char							: 5;
		unsigned char							: 8;
        uint16_t entity_id;								/** Entity ID                   */
        uint16_t parent_id;								/** Parent ID                   */
        uint16_t 								: 16;
        float roll;										/** Entity roll                 */
        float pitch;									/** Entity pitch                */
        float yaw;										/** Entity yaw                  */
        double lat_x;									/** Entity latitude  / X offset */
        double lon_y;									/** Entity longitude / Y offset */
        double alt_z;									/** Entity altitude  / Z offset */
	};

#define CIGI_ENTITY_POSITION_OPCODE              1
#define CIGI_ENTITY_POSITION_SIZE                48

    /** Entity attached flag */
#define ENTITY_ATTACHED                         1
#define ENTITY_DETACHED                         0

    /** Entity Ground/Ocean Clamp state */
#define ENTITY_CLAMP_NONE						0
#define ENTITY_CLAMP_NON_CONFORMAL				1
#define ENTITY_CLAMP_CONFORMAL					2



    /** Conformal clamped entity position */
    struct CIGI_ENTITY_POSITION_CC {
        uint16_t packet_size;							/** Packet size = 32 bytes      */
        uint16_t packet_id;								/** Packet ID = 2               */
        uint16_t entity_id;
        uint16_t 								: 16;
        float yaw;
        uint32_t 								: 32;
        double latitude;
        double longitude;
    };

#define CIGI_ENTITY_POSITION_CC_OPCODE           2
#define CIGI_ENTITY_POSITION_CC_SIZE             32

 /********************************************************************************************/

   struct CIGI_COMPONENT_CONTROL {
        uint16_t packet_size;							/** Packet size = 40 bytes      */
        uint16_t packet_id;								/** Packet ID = 3               */
        uint16_t component_id;
        unsigned char component_class			: 6;
        unsigned char							: 2;
        unsigned char component_state;
        uint16_t instance_id;
        uint16_t								: 16;
        uint32_t								: 32;
        uint32_t data1;
        uint32_t data2;
        uint32_t data3;
        uint32_t data4;
        uint32_t data5;
        uint32_t data6;
    };

#define CIGI_COMPONENT_CONTROL_OPCODE           3
#define CIGI_COMPONENT_CONTROL_SIZE             40

/********************************************************************************************/

    struct CIGI_COMPONENT_CONTROL_S {
        uint16_t packet_size;							/** Packet size = 24 bytes      */
        uint16_t packet_id;								/** Packet ID = 4               */
        uint16_t component_id;
        unsigned char component_class			: 6;
        unsigned char							: 2;
        unsigned char component_state;
        uint16_t instance_id;
        uint16_t								: 16;
        uint32_t								: 32;
        uint32_t data1;
        uint32_t data2;
    };

#define CIGI_COMPONENT_CONTROL_S_OPCODE         4
#define CIGI_COMPONENT_CONTROL_S_SIZE           24

/********************************************************************************************/

    struct CIGI_ARTPART_CONTROL {
        uint16_t packet_size;							/** Packet size = 32 bytes      */
        uint16_t packet_id;								/** Packet ID = 5               */
        uint16_t entity_id;
        uint8_t artpart_id;
        uint8_t artpart_enable : 1;
        uint8_t xoffset_enable : 1;
        uint8_t yoffset_enable : 1;
        uint8_t zoffset_enable : 1;
        uint8_t roll_enable    : 1;
        uint8_t pitch_enable   : 1;
        uint8_t yaw_enable     : 1;
        float xoffset;
        float yoffset;
        float zoffset;
        float roll;
        float pitch;
        float yaw;
    };

#define CIGI_ARTPART_CONTROL_OPCODE             5
#define CIGI_ARTPART_CONTROL_SIZE               32

/********************************************************************************************/

   struct CIGI_ARTPART_CONTROL_S {
        uint16_t packet_size;							/** Packet size = 24 bytes      */
        uint16_t packet_id;								/** Packet ID = 6               */
        uint16_t entity_id;
        uint8_t artpart1_id;
        uint8_t artpart2_id;
        uint8_t dof1_select        : 3;
        uint8_t dof2_select        : 3;
        uint8_t artpart1_enable    : 1;
        uint8_t artpart2_enable    : 1;
        uint8_t                    : 8;
        uint16_t                   : 16;
        float dof1;
        float dof2;
        uint32_t                    : 32;
    };

#define CIGI_ARTPART_CONTROL_S_OPCODE           6
#define CIGI_ARTPART_CONTROL_S_SIZE             24

/********************************************************************************************/

    struct CIGI_VELOCITY_CONTROL {
        uint16_t packet_size;							/** Packet size = 32 bytes      */
        uint16_t packet_id;								/** Packet ID = 7               */
        uint16_t entity_id;
        uint8_t artpart_id;
        uint8_t apply_artpart  : 1;
        uint8_t coord_system   : 1;
        uint8_t                : 6;
        float x_rate;
        float y_rate;
        float z_rate;
        float roll_rate;
        float pitch_rate;
        float yaw_rate;
    };

#define CIGI_VELOCITY_CONTROL_OPCODE                7
#define CIGI_VELOCITY_CONTROL_SIZE                  32

/********************************************************************************************/

    struct CIGI_CELESTIAL_SPHERE_CONTROL {
        uint16_t packet_size;							/** Packet size = 24 bytes      */
        uint16_t packet_id;								/** Packet ID = 8               */
        uint8_t ephemeris_enable   : 1;
        uint8_t sun_enable         : 1;
        uint8_t moon_enable        : 1;
        uint8_t starfield_enable   : 1;
        uint8_t datetime_valid     : 1;
        uint8_t                    : 3;
        uint8_t                    : 8;
        uint8_t hour;
        uint8_t minute;
        float seconds;
        uint32_t date;
        float starfield_intensity;
        unsigned int                    : 32;
    };

#define CIGI_CELESTIAL_SPHERE_CONTROL_OPCODE    8
#define CIGI_CELESTIAL_SPHERE_CONTROL_SIZE      24

/********************************************************************************************/

    struct CIGI_ATMOSPHERE_CONTROL {
        uint16_t packet_size;							/** Packet size = 32 bytes      */
        uint16_t packet_id;								/** Packet ID = 9               */
        uint8_t global_humidity    : 7;
        uint8_t atm_model_enable   : 1;
        uint8_t                    : 8;
        uint16_t                    : 16;
        float global_air_temp;
        float global_visibility;
        float global_windspeed_horz;
        float global_windspeed_vert;
        float global_wind_direction;
        float global_baro_pressure;
    };

#define CIGI_ATMOSPHERE_CONTROL_OPCODE          9
#define CIGI_ATMOSPHERE_CONTROL_SIZE            32

/********************************************************************************************/

    struct CIGI_ENV_REGION_CONTROL {
        uint16_t packet_size;							/** Packet size = 48 bytes      */
        uint16_t packet_id;								/** Packet ID = 10              */
        uint8_t region_state       : 2;
        uint8_t merge_weather      : 1;
        uint8_t merge_aerosols     : 1;
        uint8_t merge_maritime     : 1;
        uint8_t merge_terrestrial  : 1;
        uint8_t                    : 2;
        uint8_t                    : 8;
        uint16_t region_id;
        double latitude;
        double longitude;
        float size_x;
        float size_y;
        float corner_radius;
        float rotation;
        float transition_perimeter;
        uint32_t                    : 32;
    };

#define CIGI_ENV_REGION_CONTROL_OPCODE          0x0A
#define CIGI_ENV_REGION_CONTROL_SIZE            48

/********************************************************************************************/

    struct CIGI_WEATHER_CONTROL {
        uint16_t packet_size;							/** Packet size = 72 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0b           */
        uint8_t layer_id;
        uint8_t humidity;
        uint8_t weather_enable     : 1;
        uint8_t bottom_scud_enable : 1;
        uint8_t random_winds       : 1;
        uint8_t random_lightning   : 1;
        uint8_t cloud_type         : 4;
        uint8_t scope              : 2;
        uint8_t severity           : 3;
        uint8_t top_scud_enable    : 1;
        uint8_t                    : 2;
        uint16_t entity_region_id;

        float air_temp;
        float visibility;
        float bottom_scud_freq;
        float coverage;
        float base_elevation;
        float thickness;
        float bottom_transition_band;
        float windspeed_horz;
        float windspeed_vert;
        float wind_direction;
        float baro_pressure;
        float aerosol_concentration;
        float top_scud_freq;
        float top_transition_band;
        unsigned int		           : 32;
    };

#define CIGI_WEATHER_CONTROL_OPCODE             0x0B
#define CIGI_WEATHER_CONTROL_SIZE               72

/********************************************************************************************/

    struct CIGI_MARITIME_CONDITIONS_CONTROL {
        uint16_t packet_size;							/** Packet size = 24 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0c           */
        uint8_t conditions_enable  : 1;
        uint8_t whitecap_enable    : 1;
        uint8_t scope              : 2;
        uint8_t                    : 4;
        uint8_t                    : 8;
        uint16_t entity_region_id;
        float surface_height;
        float surface_temp;
        float surface_clarity;
        unsigned int                : 32;
    };

#define CIGI_MARITIME_CONDITIONS_CONTROL_OPCODE 0x0c
#define CIGI_MARITIME_CONDITIONS_CONTROL_SIZE   24

/********************************************************************************************/

    struct CIGI_WAVE_CONTROL {
        uint16_t packet_size;							/** Packet size = 32 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0d          */
        uint8_t wave_id;
        uint8_t wave_enable        : 1;
        uint8_t scope              : 2;
        uint8_t breaker_type       : 2;
        uint8_t                    : 3;
        uint16_t entity_region_id;
        float wave_height;
        float wavelength;
        float period;
        float direction;
        float phase_offset;
        float leading;
    };

#define CIGI_WAVE_CONTROL_OPCODE                0x0d
#define CIGI_WAVE_CONTROL_SIZE                  32

/********************************************************************************************/

    struct CIGI_TERRESTRIAL_CONDITIONS_CONTROL {
        uint16_t packet_size;							/** Packet size = 16 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0e           */
        uint16_t entity_region_id;
        uint8_t condition_enable   : 1;
        uint8_t scope              : 2;
        uint8_t severity           : 5;
        uint8_t coverage;
        uint16_t condition_id;
        uint16_t        : 16;
        uint32_t        : 32;
    };

#define CIGI_TERRESTRIAL_CONDITIONS_CONTROL_OPCODE  0x0e
#define CIGI_TERRESTRIAL_CONDITIONS_CONTROL_SIZE    16

/********************************************************************************************/

    struct CIGI_VIEW_CONTROL {
        uint16_t packet_size;							/** Packet size = 40 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0f           */
        uint8_t group_id;
        uint8_t xoffset_enable : 1;
        uint8_t yoffset_enable : 1;
        uint8_t zoffset_enable : 1;
        uint8_t roll_enable    : 1;
        uint8_t pitch_enable   : 1;
        uint8_t yaw_enable     : 1;
        uint8_t                : 2;
        uint16_t view_id;
        uint16_t entity_id;
        uint16_t  : 16;
        float xoffset;
        float yoffset;
        float zoffset;
        float roll;
        float pitch;
        float yaw;
        uint32_t  : 32;
    };

#define CIGI_VIEW_CONTROL_OPCODE                0x0f
#define CIGI_VIEW_CONTROL_SIZE                  40

/********************************************************************************************/

    struct CIGI_SENSOR_CONTROL {
        uint16_t packet_size;							/** Packet size = 32 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x10            */
        uint8_t sensor_id;
        uint8_t sensor_on_off      : 1;
        uint8_t polarity           : 1;
        uint8_t line_dropout       : 1;
        uint8_t auto_gain          : 1;
        uint8_t track_white_black  : 1;
        uint8_t track_mode         : 3;
        uint8_t response_type      : 1;
        uint8_t                    : 7;
        uint8_t                    : 8;
        uint16_t view_id;
        uint16_t                    : 16;
        float gain;
        float level;
        float ac_coupling;
        float noise;
        uint32_t                    : 32;
    };

#define CIGI_SENSOR_CONTROL_OPCODE              0x10
#define CIGI_SENSOR_CONTROL_SIZE                32

/********************************************************************************************/

    struct CIGI_MOTION_TRACKER_CONTROL {
        uint16_t packet_size;							/** Packet size = 16 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x11            */
        uint8_t tracker_id;
        uint8_t tracker_enable         : 1;
        uint8_t boresight_enable       : 1;
        uint8_t x_enable               : 1;
        uint8_t y_enable               : 1;
        uint8_t z_enable               : 1;
        uint8_t roll_enable            : 1;
        uint8_t pitch_enable           : 1;
        uint8_t yaw_enable             : 1;
        uint8_t view_or_group_select   : 1;
        uint8_t                        : 7;
        uint8_t                        : 8;
        uint16_t view_or_group_id;
        uint16_t                        : 16;
        uint32_t                        : 32;
    };

#define CIGI_MOTION_TRACKER_CONTROL_OPCODE      0x11
#define CIGI_MOTION_TRACKER_CONTROL_SIZE        16

/********************************************************************************************/

    struct CIGI_ERM_DEF {
        uint16_t packet_size;							/** Packet size = 24 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x12           */
        uint8_t erm_enable				: 1;
        uint8_t							: 7;
        uint8_t							: 8;
        uint16_t						: 16;
        double equatorial_radius;
        double flattening;
    };

#define CIGI_ERM_DEF_OPCODE                 0x12
#define CIGI_ERM_DEF_SIZE                   24

/********************************************************************************************/

    struct CIGI_ACCELERATION_CONTROL_DEF {
        uint16_t packet_size;							/** Packet size = 32 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x13           */
        uint16_t entity_id;
        uint8_t artpart_id;
        uint8_t artpart_apply : 1;
        uint8_t coord_system  : 1;
        uint8_t               : 6;
        float accel_x;
        float accel_y;
        float accel_z;
        float accel_roll;
        float accel_pitch;
        float accel_yaw;
    };

#define CIGI_ACCELERATION_CONTROL_DEF_OPCODE          0x13
#define CIGI_ACCELERATION_CONTROL_DEF_SIZE            32

/********************************************************************************************/

    struct CIGI_VIEW_DEF {
        uint16_t packet_size;							/** Packet size = 40 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x14           */
        uint16_t view_id;
        uint8_t group_id;
        uint8_t near_enable					: 1;
        uint8_t far_enable					: 1;
        uint8_t left_enable					: 1;
        uint8_t right_enable				: 1;
        uint8_t top_enable					: 1;
        uint8_t bottom_enable				: 1;
        uint8_t mirror_mode					: 2;
		uint8_t pixel_replication_mode : 3;
        uint8_t projection_type        : 1;
        uint8_t reorder                : 1;
        uint8_t view_type              : 3;
        uint8_t                        : 8;
        uint16_t                       : 16;
        float fov_near;
        float fov_far;
        float fov_left;
        float fov_right;
        float fov_top;
        float fov_bottom;
        uint32_t							: 32;
    };

#define CIGI_VIEW_DEF_OPCODE                0x14
#define CIGI_VIEW_DEF_SIZE                  40

/********************************************************************************************/

    struct CIGI_COLL_SEGMENT_DEF {
        uint16_t packet_size;							/** Packet size = 40 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x15          */
        uint8_t segment_id;
        uint8_t segment_enable : 1;
        uint8_t                : 7;
        uint16_t entity_id;
        float x1;
        float y1;
        float z1;
        float x2;
        float y2;
        float z2;
        uint32_t material_mask;
        unsigned int                : 32;
    };

#define CIGI_COLL_SEGMENT_DEF_OPCODE    0x15
#define CIGI_COLL_SEGMENT_DEF_SIZE      40

/********************************************************************************************/

    struct CIGI_COLL_VOLUME_DEF {
        uint16_t packet_size;							/** Packet size = 48 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x16           */
        uint8_t volume_id;
        uint8_t volume_enable	: 1;
        uint8_t volume_type		: 1;
        uint8_t					: 6;
        uint16_t entity_id;
        float x;
        float y;
        float z;
        float height_radius;
        float width;
        float depth;
        float roll;
        float pitch;
        float yaw;
        unsigned int                : 32;
    };

#define CIGI_COLL_VOLUME_DEF_OPCODE     0x16
#define CIGI_COLL_VOLUME_DEF_SIZE       48

/********************************************************************************************/

    struct CIGI_HAT_HOT_REQUEST {
        uint16_t packet_size;							/** Packet size = 40 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x17          */
        uint16_t hat_hot_id;
        uint16_t entity_id;
        uint8_t request_type   : 2;
        uint8_t coord_system   : 1;
        uint8_t                : 5;
        uint8_t update_period;
		uint16_t				: 16;
        double lat_x;
        double lon_y;
        double alt_z;
    };

#define CIGI_HAT_HOT_REQUEST_OPCODE             0x17
#define CIGI_HAT_HOT_REQUEST_SIZE               40

/********************************************************************************************/

    struct CIGI_LOS_SEGMENT_REQUEST {
        uint16_t packet_size;							/** Packet size = 72 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x18           */
        uint16_t los_id;
        uint16_t source_entity_id;
        uint8_t request_type           : 1;
        uint8_t source_coord_system    : 1;
        uint8_t dest_coord_system      : 1;
        uint8_t response_coord_system  : 1;
        uint8_t dest_entity_id_valid   : 1;
        uint8_t                        : 3;
        uint8_t alpha_threshold;
        uint16_t dest_entity_id;
        uint8_t update_period;
        uint8_t                        : 8;
        uint16_t                       : 16;
        double source_lat_x;
        double source_lon_y;
        double source_alt_z;
        double dest_lat_x;
        double dest_lon_y;
        double dest_alt_z;
        uint32_t material_mask;
        uint32_t                        : 32;
    };

#define CIGI_LOS_SEGMENT_REQUEST_OPCODE     0x18
#define CIGI_LOS_SEGMENT_REQUEST_SIZE       72

/********************************************************************************************/

    struct CIGI_LOS_VECTOR_REQUEST {
        uint16_t packet_size;							/** Packet size = 64 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x19           */
        uint16_t los_id;
        uint16_t entity_id;
        uint8_t request_type           : 1;
        uint8_t source_coord_system    : 1;
        uint8_t resp_coord_system      : 1;
        uint8_t                        : 5;
        uint8_t alpha_threshold;
        uint8_t update_period;
        uint8_t                        : 8;
        uint32_t                        : 32;
        float azimuth;
        float elevation;
        float min_range;
        float max_range;
        double source_lat_x;
        double source_lon_y;
        double source_alt_z;
        uint32_t material_mask;
        uint32_t                        : 32;
    };

#define CIGI_LOS_VECTOR_REQUEST_OPCODE      0x19
#define CIGI_LOS_VECTOR_REQUEST_SIZE        64

/********************************************************************************************/

    struct CIGI_POSITION_REQUEST {
        uint16_t packet_size;							/** Packet size = 8 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x1a           */
        uint8_t artpart_id;
        uint8_t update_mode    : 1;
        uint8_t object_class   : 3;
        uint8_t coord_system   : 2;
        uint8_t                : 2;
        uint16_t object_id;
    };

#define CIGI_POSITION_REQUEST_OPCODE        0x1a
#define CIGI_POSITION_REQUEST_SIZE          8

/********************************************************************************************/

    struct CIGI_ENV_CONDITIONS_REQUEST {
        uint16_t packet_size;							/** Packet size = 32 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x1b           */
        uint8_t request_type   : 4;
        uint8_t                : 4;
        uint8_t request_id;
        uint16_t               : 16;
        double latitude;
        double longitude;
        double altitude;
    };

#define CIGI_ENV_CONDITIONS_REQUEST_OPCODE  0x1b
#define CIGI_ENV_CONDITIONS_REQUEST_SIZE    32

/********************************************************************************************/

    struct CIGI_SYMBOL_SURFACE_DEF {
        uint16_t packet_size;							/** Packet size = 64 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x1c           */
        uint16_t surface_id;
        uint16_t entity_or_view_id;
        uint8_t surface_state          : 1;
        uint8_t attach_type            : 1;
        uint8_t billboard              : 1;
        uint8_t persp_growth_enable    : 1;
        uint8_t                        : 4;
        uint8_t                        : 8;
        uint16_t                       : 16;
        float x_left;
        float y_right;
        float z_top;
        float yaw_bottom;
        float pitch;
        float roll;
        float width;
        float height;
        float min_u;
        float max_u;
        float min_v;
        float max_v;
        uint32_t                       : 32;
    };

#define CIGI_SYMBOL_SURFACE_DEF_OPCODE 0x1c
#define CIGI_SYMBOL_SURFACE_DEF_SIZE   64

/********************************************************************************************/

#define CIGI_SYMBOL_TEXT_MAX_TEXT_SIZE 65512
    struct CIGI_SYMBOL_TEXT_DEF {
        uint16_t packet_size;							/** Packet size = 256 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x1d           */
        uint8_t alignment      : 4;
        uint8_t orientation    : 2;
        uint8_t                : 2;
        uint8_t font_id;
        uint16_t symbol_id;
        float font_size;
        char text[CIGI_SYMBOL_TEXT_MAX_TEXT_SIZE];
    };

#define CIGI_SYMBOL_TEXT_DEF_OPCODE     0x1d
//#define CIGI_SYMBOL_TEXT_MAX_DEF_SIZE  65528
#define CIGI_SYMBOL_TEXT_DEF_BASE__SIZE       16
#define CIGI_SYMBOL_TEXT_DEF_BASE_SIZE  4

/********************************************************************************************/

    struct CIGI_CIRCLE_DEF_ELEMENT {
        float center_u;
        float center_v;
        float radius;
        float inner_radius;
        float start_angle;
        float end_angle;
    };

#define CIGI_SYMBOL_CIRCLE_DEF_MAX_ELEMENT_CNT  2729
    struct CIGI_SYMBOL_CIRCLE_DEF {
        uint16_t packet_size;							/** Packet size = 256 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x1e           */
        uint16_t symbol_id;
        uint16_t stipple_pattern;
        uint8_t drawing_style   : 1;
        uint8_t                 : 7;
        uint8_t                 : 8;
        uint16_t                : 16;
        float line_width;
        float stipple_pattern_length;
        uint32_t                : 32;
        CIGI_CIRCLE_DEF_ELEMENT circle[CIGI_SYMBOL_CIRCLE_DEF_MAX_ELEMENT_CNT];
        uint16_t                : 16;					// padding to fill size: 256 
    };

#define CIGI_SYMBOL_CIRCLE_DEF_OPCODE    0x1e
//#define CIGI_SYMBOL_CIRCLE_DEF_SIZE      65520
#define CIGI_SYMBOL_CIRCLE_DEF_BASE_SIZE 24
#define CIGI_SYMBOL_CIRCLE_ELEMENT_SIZE  24

/********************************************************************************************/

    struct CIGI_POLYGON_DEF_ELEMENT {
        float vertex_u;
        float vertex_v;
    };

#define CIGI_SYMBOL_POLYGON_DEF_MAX_VERTEX_CNT  8188

 // CHANGE NOTE: capped polygon vertex count to a much smaller number to allow for easier stack allocation
#define CIGI_SYMBOL_POLYGON_DEF_MAX_PRACTICAL_VERTEX_CNT 100
    struct CIGI_SYMBOL_POLYGON_DEF {
        uint16_t packet_size;							/** Packet size = 256 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x1f           */
        uint16_t symbol_id;
        uint16_t stipple_pattern;
        uint8_t primitive_type  : 4;
        uint8_t					: 4;
        uint8_t                 : 8;
        uint16_t                : 16;
        float line_width;
        float stipple_pattern_length;
        uint32_t                : 32;
        CIGI_POLYGON_DEF_ELEMENT line[CIGI_SYMBOL_POLYGON_DEF_MAX_PRACTICAL_VERTEX_CNT];
    };

#define CIGI_SYMBOL_POLYGON_DEF_OPCODE    0x1f
//#define CIGI_SYMBOL_POLYGON_DEF_SIZE      65528
#define CIGI_SYMBOL_POLYGON_DEF_BASE_SIZE 24
#define CIGI_SYMBOL_VERTEX_SIZE        8

 // CHANGE NOTE: precalculated practical default size of polygon packets
#define CIGI_SYMBOL_POLYGON_DEF_PRACTICAL_SIZE CIGI_SYMBOL_POLYGON_DEF_MAX_PRACTICAL_VERTEX_CNT*CIGI_SYMBOL_VERTEX_SIZE + CIGI_SYMBOL_POLYGON_DEF_BASE_SIZE



/********************************************************************************************/

    struct CIGI_SYMBOL_CLONE {
        uint16_t packet_size;							/** Packet size = 16 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x20           */
        uint16_t symbol_id;
        uint16_t source_id;
        uint8_t source_type    : 1;
        uint8_t                : 7;
        uint8_t                : 8;
        uint16_t				: 16;
        uint32_t				: 32;
    };

#define CIGI_SYMBOL_CLONE_OPCODE   0x20
#define CIGI_SYMBOL_CLONE_SIZE     16

/********************************************************************************************/

    struct CIGI_SYMBOL_CONTROL {
        uint16_t packet_size;							/** Packet size = 48 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x21           */
        uint16_t symbol_id;
        uint16_t parent_id;
        uint8_t symbol_state   : 2;
        uint8_t attach_state   : 1;
        uint8_t reset_flash    : 1;
        uint8_t inherit_color  : 1;
        uint8_t                : 3;
        uint8_t layer;
        uint8_t flash_duty_cycle;
        uint8_t                : 8;
        uint16_t surface_id;
        uint16_t       : 16;
        float flash_period;
        float pos_u;
        float pos_v;
        float rotation;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;
        float scale_u;
        float scale_v;
        uint16_t       : 16;
    };

#define CIGI_SYMBOL_CONTROL_OPCODE   0x21
#define CIGI_SYMBOL_CONTROL_SIZE     48

/********************************************************************************************/

    struct CIGI_SYMBOL_CONTROL_S {
        uint16_t packet_size;							/** Packet size = 24 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x22           */
        uint16_t symbol_id;
        uint8_t attribute1;
        uint8_t attribute2;
        uint8_t symbol_state   : 2;
        uint8_t attach_state   : 1;
        uint8_t reset_flash    : 1;
        uint8_t inherit_color  : 1;
        uint8_t                : 3;
        uint8_t                : 8;
        uint16_t       : 16;
        uint32_t attribute1_value;
        uint32_t attribute2_value;
        uint32_t       : 32;
    };

#define CIGI_SYMBOL_CONTROL_S_OPCODE   0x22
#define CIGI_SYMBOL_CONTROL_S_SIZE     24

/********************************************************************************************/

    struct CIGI_TEXTURED_CIRCLE_DEF_ELEMENT {
        float center_u;
        float center_v;
        float radius;
        float inner_radius;
        float start_angle;
        float end_angle;
		float texture_coord_s;
		float texture_coord_t;
        float texture_mapping_radius;
        float texture_mapping_rotation;
    };

#define CIGI_SYMBOL_TEXTURED_CIRCLE_DEF_MAX_ELEMENT_CNT  1637
    struct CIGI_SYMBOL_TEXTURED_CIRCLE_DEF {
        uint16_t packet_size;							/** Packet size = 256 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x23             */
        uint16_t symbol_id;
        uint16_t texture_id;
        uint8_t filter_mode   : 1;
        uint8_t repeat_clamp  : 1;
        uint8_t               : 6;
        uint8_t               : 8;
        uint16_t              : 16;
        uint32_t              : 32;
        CIGI_TEXTURED_CIRCLE_DEF_ELEMENT circle[CIGI_SYMBOL_TEXTURED_CIRCLE_DEF_MAX_ELEMENT_CNT];
    };

#define CIGI_SYMBOL_TEXTURED_CIRCLE_DEF_OPCODE    0x23
//#define CIGI_SYMBOL_TEXTURED_CIRCLE_DEF_SIZE      65496
#define CIGI_SYMBOL_TEXTURED_CIRCLE_DEF_BASE_SIZE 16
#define CIGI_SYMBOL_TEXTURED_CIRCLE_ELEMENT_SIZE  40

/********************************************************************************************/

    struct CIGI_TEXTURED_POLYGON_DEF_ELEMENT {
        float vertex_u;
        float vertex_v;
		float texture_coord_s;
		float texture_coord_t;
    };

#define CIGI_SYMBOL_TEXTURED_POLYGON_DEF_MAX_ELEMENT_CNT  4095
    struct CIGI_SYMBOL_TEXTURED_POLYGON_DEF {
        uint16_t packet_size;							/** Packet size = 256 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x24           */
        uint16_t symbol_id;
        uint16_t texture_id;
        uint8_t filter_mode     : 1;
        uint8_t repeat_clamp    : 1;
        uint8_t					: 2;
        uint8_t primitive_type  : 4;
        uint8_t                 : 8;
        uint16_t                : 16;
        uint32_t                : 32;
        CIGI_TEXTURED_POLYGON_DEF_ELEMENT line[CIGI_SYMBOL_TEXTURED_POLYGON_DEF_MAX_ELEMENT_CNT];
    };

#define CIGI_SYMBOL_TEXTURED_POLYGON_DEF_OPCODE    0x24
//#define CIGI_SYMBOL_TEXTURED_POLYGON_DEF_SIZE      65536
#define CIGI_SYMBOL_TEXTURED_POLYGON_DEF_BASE_SIZE 16
#define CIGI_SYMBOL_TEXTURED_POLYGON_ELEMENT_SIZE  16

/********************************************************************************************/

   // CIGI Entity Control
    struct CIGI_ENTITY_CONTROL {
        uint16_t packet_size;							/** Packet size = 16 bytes  */
        uint16_t packet_id;								/** Packet ID = 0x25        */
        uint8_t entity_state			: 2;  /** Entity State					*/
        uint8_t collision_detect		: 1;  /** Collision detection request	*/
        uint8_t inherit_alpha			: 1;  /** inherit alpha				    */
        uint8_t smoothing_enable		: 1;  /** smoothinh				        */
        uint8_t extended_entity_type	: 1;  /** extended entity type			*/
        uint8_t							: 2;  /** Reserved						*/
        uint8_t alpha;						/** Alpha                       */
        uint16_t entity_id;                 /** Entity ID                   */
		uint8_t entity_kind ;
		uint8_t entity_domain ;
		uint16_t entity_country_type ;
		uint8_t entity_category ;
		uint8_t entity_subcategory ;
		uint8_t entity_specific ;
		uint8_t entity_extra ;
    };

#define CIGI_ENTITY_CONTROL_OPCODE              0x25
#define CIGI_ENTITY_CONTROL_SIZE                16

    /** Entity state */
#define ENTITY_STATE_INACTIVE                   0
#define ENTITY_STATE_ACTIVE                     1
#define ENTITY_STATE_DESTROYED                  2

/********************************************************************************************/

    struct CIGI_ANIMATION_CONTROL {
        uint16_t packet_size;							/** Packet size = 16 bytes  */
        uint16_t packet_id;								/** Packet ID = 0x26        */
		uint8_t animation_state			: 1;
		uint8_t animation_frame_reset	: 1;
		uint8_t animation_loop_mode		: 1;
        uint8_t inherit_alpha			: 1;  /** inherit alpha				    */
		uint8_t							: 4;
        uint8_t alpha;						/** Alpha                       */
        uint16_t entity_id;                 /** Entity ID                   */
        uint16_t animation_id;              /** Animation ID                   */
        uint16_t						: 16;
        float animation_speed;              /** Animation Speed                   */
	};

#define CIGI_ANIMATION_CONTROL_OPCODE              0x26
#define CIGI_ANIMATION_CONTROL_SIZE                16

/********************************************************************************************/

    // CIGI Start of Frame (CIGI v4.0 version) // chas
    struct CIGI_START_OF_FRAME {
        uint16_t packet_size;                 /** Packet size = 24 bytes  */
        uint16_t packet_id;                   /** Packet ID = 0xffff      */
        uint8_t cigi_major_version;          /** CIGI version ID         */
        int8_t db_number;                    /** Database number         */
        int8_t		           : 8;
        int8_t ig_mode         : 2;    /** IG mode                 */
        int8_t timestamp_valid : 1;    /** Timestamp valid flag    */
        int8_t earth_ref_model : 1;    /** Earth Reference Model   */
        int8_t minor_version   : 4;    /** CIGI minor version      */
        uint32_t frame_ctr;                  /** Frame counter           */
        uint32_t timestamp;                  /** Timing value/tag        */
        uint32_t last_frame_ctr;
        uint16_t							: 8 ;
        uint16_t OverframingCondition       : 1 ;
        uint16_t PagingCondition            : 1 ;
        uint16_t ExcessVarLenDataCondition  : 1 ;
        uint16_t							: 5 ;
        uint16_t							: 16 ;
    };

#define CIGI_START_OF_FRAME_OPCODE       0xffff	// chas
#define CIGI_START_OF_FRAME_SIZE         24		// chas

/********************************************************************************************/

    struct CIGI_HAT_HOT_RESPONSE {
        uint16_t packet_size;							/** Packet size = 16 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x           */
        uint8_t valid          : 1;
        uint8_t response_type  : 1;
        uint8_t                : 2;
        uint8_t frame_num_lsn  : 4;
        uint8_t                : 8;
        uint16_t hat_hot_id;
        double height;
    };

#define CIGI_HAT_HOT_RESPONSE_OPCODE            0x0fff
#define CIGI_HAT_HOT_RESPONSE_SIZE              16

/********************************************************************************************/

    struct CIGI_HAT_HOT_EXT_RESPONSE {
        uint16_t packet_size;							/** Packet size =  bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0x0ffe      */
        uint8_t valid          : 1;
        uint8_t response_type  : 3;
        uint8_t frame_num_lsn  : 4;
        uint8_t                : 8;
        uint16_t hat_hot_id;
        double hat;
        double hot;
        uint32_t material_code;
        float normal_azimuth;
        float normal_elevation;
        uint32_t                : 32;
    };

#define CIGI_HAT_HOT_EXT_RESPONSE_OPCODE        0x0ffe
#define CIGI_HAT_HOT_EXT_RESPONSE_SIZE          40

/********************************************************************************************/

    struct CIGI_LOS_RESPONSE {
        uint16_t packet_size;							/** Packet size = 24 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0x0ffd      */
        uint16_t los_id;
        uint16_t entity_id;

        int8_t valid              : 1;
        int8_t entity_id_valid    : 1;
        int8_t visible            : 1;
        int8_t                    : 1;
        int8_t frame_num_lsn      : 4;
        int8_t response_count     : 8;
        uint16_t                  : 16;
        uint32_t                  : 32;
        double range;
    };

#define CIGI_LOS_RESPONSE_OPCODE            0x0ffd
#define CIGI_LOS_RESPONSE_SIZE              24

/********************************************************************************************/

    struct CIGI_LOS_EXT_RESPONSE {
        uint16_t packet_size;							/** Packet size = 64 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0ffc           */
        uint16_t los_id;
        uint16_t entity_id;
        int8_t valid              : 1;
        int8_t entity_id_valid    : 1;
        int8_t range_valid        : 1;
        int8_t visible            : 1;
        int8_t frame_num_lsn      : 4;
        int8_t response_count     : 8;
        int8_t				      : 8;
        double range;
        double lat_x;
        double lon_y;
        double alt_z;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;
        uint32_t material_code;
        float normal_azimuth;
        float normal_elevation;
    };

#define CIGI_LOS_EXT_RESPONSE_OPCODE        0x0ffc
#define CIGI_LOS_EXT_RESPONSE_SIZE          64

/********************************************************************************************/

    struct CIGI_SENSOR_RESPONSE {
        uint16_t packet_size;							/** Packet size = 24 bytes       */
        uint16_t packet_id;								/** Packet ID = 0x0ffc           */
        uint8_t sensor_id;
        uint8_t sensor_status  : 2;
        uint8_t                : 6;
        uint16_t view_id;
        uint16_t gate_x_size;
        uint16_t gate_y_size;
        float gate_x_position;
        float gate_y_position;
        uint32_t frame_counter;
    };

#define CIGI_SENSOR_RESPONSE_OPCODE         0x0ffb
#define CIGI_SENSOR_RESPONSE_SIZE           24

/********************************************************************************************/

    struct CIGI_SENSOR_EXT_RESPONSE {
        uint16_t packet_size;							/** Packet size = 56 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0x0ffa        */
        uint16_t view_id;
        uint16_t entity_id;
        uint8_t sensor_id;
        uint8_t sensor_status       : 2;
        uint8_t entity_id_valid     : 1;
        uint8_t                     : 5;
        uint16_t					: 16;
        uint16_t gate_x_size;
        uint16_t gate_y_size;
        float gate_x_position;
        float gate_y_position;
        uint32_t frame_counter;
        uint32_t						: 32;
        double lat;
        double lon;
        double alt;
    };

#define CIGI_SENSOR_EXT_RESPONSE_OPCODE         0x0ffa
#define CIGI_SENSOR_EXT_RESPONSE_SIZE           56

/********************************************************************************************/

    struct CIGI_POSITION_RESPONSE {
        uint16_t packet_size;							/** Packet size = 48 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0x0ff9           */
        uint8_t artpart_id;
        uint8_t object_class       : 3;
        uint8_t coord_system       : 2;
        uint8_t                    : 3;
        uint16_t object_id;
        double lat_x;
        double lon_y;
        double alt_z;
        float roll;
        float pitch;
        float yaw;
        unsigned int                    : 32;
    };

#define CIGI_POSITION_RESPONSE_OPCODE           0x0ff9
#define CIGI_POSITION_RESPONSE_SIZE             48

/********************************************************************************************/

    struct CIGI_WEATHER_RESPONSE {
        uint16_t packet_size;							/** Packet size = 32 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0ff8           */
        uint8_t request_id;
        uint8_t humidity;
        uint16_t          : 16;
        float air_temp;
        float visibility;
        float windspeed_horz;
        float windspeed_vert;
        float wind_direction;
        float baro_pressure;
    };

#define CIGI_WEATHER_RESPONSE_OPCODE        0x0ff8
#define CIGI_WEATHER_RESPONSE_SIZE          32

/********************************************************************************************/

    struct CIGI_AEROSOL_RESPONSE {
        uint16_t packet_size;							/** Packet size = 16 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0ff7           */
        uint8_t request_id;
        uint8_t layer_id;
        uint16_t          : 16;
        float concentration;
        uint32_t          : 32;
    };

#define CIGI_AEROSOL_RESPONSE_OPCODE        0x0ff7
#define CIGI_AEROSOL_RESPONSE_SIZE          16

/********************************************************************************************/

    struct CIGI_MARITIME_SURFACE_RESPONSE {
        uint16_t packet_size;							/** Packet size = 24 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0ff6           */
        uint8_t request_id;
        uint8_t					: 8;
        uint16_t				: 16;
        float surface_height;
        float surface_temp;
        float surface_clarity;
        uint32_t				: 32;
    };

#define CIGI_MARITIME_SURFACE_RESPONSE_OPCODE   0x0ff6
#define CIGI_MARITIME_SURFACE_RESPONSE_SIZE     24

/********************************************************************************************/

    struct CIGI_TERRESTRIAL_SURFACE_RESPONSE {
        uint16_t packet_size;							/** Packet size = 16 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0ff5           */
        uint8_t request_id;
        uint8_t					: 8;
        uint16_t				: 16;
        uint32_t condition_id;
        uint32_t				: 32;
    };

#define CIGI_TERRESTRIAL_SURFACE_RESPONSE_OPCODE    0x0ff5
#define CIGI_TERRESTRIAL_SURFACE_RESPONSE_SIZE      16

/********************************************************************************************/

    struct CIGI_COLL_SEGMENT_NOTIFICATION {
        uint16_t packet_size;							/** Packet size = 24 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0ff4           */
        uint16_t entity_id;
        uint16_t contacted_entity_id;
        uint8_t segment_id;
        uint8_t collision_type         : 1;
        uint8_t                        : 7;
        uint32_t material_code;
        float intersection_distance;
        uint32_t						: 32;
    };

#define CIGI_COLL_SEGMENT_NOTIFICATION_OPCODE   0x0ff4
#define CIGI_COLL_SEGMENT_NOTIFICATION_SIZE     24

/********************************************************************************************/

    struct CIGI_COLL_VOLUME_NOTIFICATION {
        uint16_t packet_size;							/** Packet size = 16 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0ff3           */
        uint16_t entity_id;
        uint16_t contacted_entity_id;
        uint8_t volume_id;
        uint8_t collision_type         : 1;
        uint8_t                        : 7;
        uint8_t contacted_volume_id;
        uint8_t                        : 8;
        uint32_t                       : 32;
    };

#define CIGI_COLL_VOLUME_NOTIFICATION_OPCODE    0x0ff3
#define CIGI_COLL_VOLUME_NOTIFICATION_SIZE      16

/********************************************************************************************/

    struct CIGI_ANIMATION_STOP_NOTIFICATION {
        uint16_t packet_size;							/** Packet size = 8 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0ff2           */
        uint16_t entity_id;
        uint16_t                : 16;
    };

#define CIGI_ANIMATION_STOP_NOTIFICATION_OPCODE     0x0ff2
#define CIGI_ANIMATION_STOP_NOTIFICATION_SIZE       8

/********************************************************************************************/

    struct CIGI_EVENT_NOTIFICATION {
        uint16_t packet_size;							/** Packet size = 24 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0ff1           */
        uint16_t event_id;
        uint16_t                : 16;
        uint32_t data1;
        uint32_t data2;
        uint32_t data3;
        uint32_t                : 32;
    };

#define CIGI_EVENT_NOTIFICATION_OPCODE              0x0ff1
#define CIGI_EVENT_NOTIFICATION_SIZE                24

/********************************************************************************************/

    struct CIGI_IG_MESSAGE {
        uint16_t packet_size;							/** Packet size =  bytes      */
        uint16_t packet_id;								/** Packet ID = 0x0ff0           */
        uint16_t message_id;
        uint16_t                : 16;
        char text[100];
    };

#define CIGI_IG_MESSAGE_OPCODE                      0x0ff0
#define CIGI_IG_MESSAGE_SIZE                        108
#define CIGI_IG_MESSAGE_BASE_SIZE                   8

/********************************************************************************************/

    struct CIGI_USER_DATA_PACKET {
        uint16_t packet_size;							/** Packet size = 8 bytes      */
        uint16_t packet_id;								/** Packet ID = 0x1000 - 0xfffe     */
        uint8_t *data_ptr;
    };

#define CIGI_USER_DATA_PACKET_OPCODE_MIN 0x1000
#define CIGI_USER_DATA_PACKET_OPCODE_MAX 0xfffe
#define CIGI_USER_DATA_PACKET_BASE_SIZE     4

/********************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // _CIGI4TYPES_H_