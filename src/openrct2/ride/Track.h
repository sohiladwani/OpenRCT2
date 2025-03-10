/*****************************************************************************
 * Copyright (c) 2014-2019 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../common.h"
#include "../object/Object.h"
#include "Ride.h"

constexpr const uint16_t RideConstructionSpecialPieceSelected = 0x100;

constexpr const int32_t BLOCK_BRAKE_BASE_SPEED = 0x20364;

using track_type_t = uint16_t;

#pragma pack(push, 1)
struct rct_trackdefinition
{
    uint8_t type;
    uint8_t vangle_end;
    uint8_t vangle_start;
    uint8_t bank_end;
    uint8_t bank_start;
    int8_t preview_z_offset;
    uint8_t pad[2] = {};
};
assert_struct_size(rct_trackdefinition, 8);
#pragma pack(pop)

/* size 0x0A */
struct rct_preview_track
{
    uint8_t index; // 0x00
    int16_t x;     // 0x01
    int16_t y;     // 0x03
    int16_t z;     // 0x05
    uint8_t var_07;
    QuarterTile var_08;
    uint8_t flags;
};

/* size 0x0A */
struct rct_track_coordinates
{
    int8_t rotation_begin; // 0x00
    int8_t rotation_end;   // 0x01
    int16_t z_begin;       // 0x02
    int16_t z_end;         // 0x04
    int16_t x;             // 0x06
    int16_t y;             // 0x08
};

enum
{
    RCT_PREVIEW_TRACK_FLAG_0 = (1 << 0),
    RCT_PREVIEW_TRACK_FLAG_1 = (1 << 1),
    RCT_PREVIEW_TRACK_FLAG_IS_VERTICAL = (1 << 2),
};

enum
{
    TRACK_ELEMENT_FLAG_TERMINAL_STATION = 1 << 3,
    TD6_TRACK_ELEMENT_FLAG_INVERTED = 1 << 6,
};

enum
{
    TRACK_ELEMENT_FLAGS2_CHAIN_LIFT = 1 << 0,
    TRACK_ELEMENT_FLAGS2_INVERTED = 1 << 1,
    // Used for giga coaster
    TRACK_ELEMENT_FLAGS2_CABLE_LIFT = 1 << 2,
    TRACK_ELEMENT_FLAGS2_HIGHLIGHT = 1 << 3,
    TRACK_ELEMENT_FLAGS2_HAS_GREEN_LIGHT = 1 << 4,
    TRACK_ELEMENT_FLAGS2_BLOCK_BRAKE_CLOSED = 1 << 5,
    TRACK_ELEMENT_FLAGS2_INDESTRUCTIBLE_TRACK_PIECE = 1 << 6,
};

enum
{
    TRACK_ELEMENT_COLOUR_SCHEME_MASK = 0b00000011,
    // Not colour related, but shares the field.
    TRACK_ELEMENT_COLOUR_DOOR_A_MASK = 0b00011100,
    TRACK_ELEMENT_COLOUR_DOOR_B_MASK = 0b11100000,
    TRACK_ELEMENT_COLOUR_SEAT_ROTATION_MASK = 0b11110000,
};

#define MAX_STATION_PLATFORM_LENGTH 32
constexpr uint16_t const MAX_TRACK_HEIGHT = 254 * COORDS_Z_STEP;
constexpr uint8_t const DEFAULT_SEAT_ROTATION = 4;

enum
{
    TRACK_NONE = 0,

    TRACK_FLAT = 0,
    TRACK_STRAIGHT,
    TRACK_STATION_END,
    TRACK_LIFT_HILL,
    TRACK_LIFT_HILL_STEEP,
    TRACK_LIFT_HILL_CURVE,
    TRACK_FLAT_ROLL_BANKING,
    TRACK_VERTICAL_LOOP,
    TRACK_SLOPE,
    TRACK_SLOPE_STEEP,
    TRACK_SLOPE_LONG,
    TRACK_SLOPE_CURVE,
    TRACK_SLOPE_CURVE_STEEP,
    TRACK_S_BEND,
    TRACK_CURVE_VERY_SMALL,
    TRACK_CURVE_SMALL,
    TRACK_CURVE,
    TRACK_TWIST,
    TRACK_HALF_LOOP,
    TRACK_CORKSCREW,
    TRACK_TOWER_BASE,
    TRACK_HELIX_SMALL,
    TRACK_HELIX_LARGE,
    TRACK_HELIX_LARGE_UNBANKED,
    TRACK_BRAKES,
    TRACK_25,
    TRACK_ON_RIDE_PHOTO,
    TRACK_WATER_SPLASH,
    TRACK_SLOPE_VERTICAL,
    TRACK_BARREL_ROLL,
    TRACK_POWERED_LIFT,
    TRACK_HALF_LOOP_LARGE,
    TRACK_SLOPE_CURVE_BANKED,
    TRACK_LOG_FLUME_REVERSER,
    TRACK_HEARTLINE_ROLL,
    TRACK_REVERSER,
    TRACK_REVERSE_FREEFALL,
    TRACK_SLOPE_TO_FLAT,
    TRACK_BLOCK_BRAKES,
    TRACK_SLOPE_ROLL_BANKING,
    TRACK_SLOPE_STEEP_LONG,
    TRACK_CURVE_VERTICAL,
    TRACK_42,
    TRACK_LIFT_HILL_CABLE,
    TRACK_LIFT_HILL_CURVED,
    TRACK_QUARTER_LOOP,
    TRACK_SPINNING_TUNNEL,
    TRACK_ROTATION_CONTROL_TOGGLE,
    TRACK_BOOSTER = TRACK_ROTATION_CONTROL_TOGGLE,
    TRACK_INLINE_TWIST_UNINVERTED,
    TRACK_INLINE_TWIST_INVERTED,
    TRACK_QUARTER_LOOP_UNINVERTED,
    TRACK_QUARTER_LOOP_INVERTED,
    TRACK_RAPIDS,
    TRACK_HALF_LOOP_UNINVERTED,
    TRACK_HALF_LOOP_INVERTED,

    TRACK_WATERFALL,
    TRACK_WHIRLPOOL,
    TRACK_BRAKE_FOR_DROP,
    TRACK_CORKSCREW_UNINVERTED,
    TRACK_CORKSCREW_INVERTED,
    TRACK_HEARTLINE_TRANSFER,
    TRACK_MINI_GOLF_HOLE,

    TRACK_GROUP_COUNT,
};

enum
{
    TRACK_CURVE_LEFT_VERY_SMALL = 5,
    TRACK_CURVE_LEFT_SMALL = 3,
    TRACK_CURVE_LEFT = 1,
    TRACK_CURVE_LEFT_LARGE = 7,
    TRACK_CURVE_NONE = 0,
    TRACK_CURVE_RIGHT_LARGE = 8,
    TRACK_CURVE_RIGHT = 2,
    TRACK_CURVE_RIGHT_SMALL = 4,
    TRACK_CURVE_RIGHT_VERY_SMALL = 6
};

enum
{
    TRACK_SLOPE_NONE = 0,
    TRACK_SLOPE_UP_25 = 2,
    TRACK_SLOPE_UP_60 = 4,
    TRACK_SLOPE_DOWN_25 = 6,
    TRACK_SLOPE_DOWN_60 = 8,
    TRACK_SLOPE_UP_90 = 10,
    TRACK_SLOPE_DOWN_90 = 18,

    TRACK_VANGLE_TOWER = 10,
    TRACK_VANGLE_REVERSE_FREEFALL = 10
};

enum
{
    TRACK_BANK_NONE = 0,
    TRACK_BANK_LEFT = 2,
    TRACK_BANK_RIGHT = 4,
    TRACK_BANK_UPSIDE_DOWN = 15,
};

enum
{
    TRACK_ELEM_FLAG_ONLY_UNDERWATER = (1 << 0),
    TRACK_ELEM_FLAG_TURN_LEFT = (1 << 1),
    TRACK_ELEM_FLAG_TURN_RIGHT = (1 << 2),
    TRACK_ELEM_FLAG_TURN_BANKED = (1 << 3),
    TRACK_ELEM_FLAG_TURN_SLOPED = (1 << 4),
    TRACK_ELEM_FLAG_DOWN = (1 << 5),
    TRACK_ELEM_FLAG_UP = (1 << 6),
    TRACK_ELEM_FLAG_NORMAL_TO_INVERSION = (1 << 7),
    TRACK_ELEM_FLAG_IS_GOLF_HOLE = (1 << 7),
    TRACK_ELEM_FLAG_STARTS_AT_HALF_HEIGHT = (1 << 8),
    TRACK_ELEM_FLAG_ONLY_ABOVE_GROUND = (1 << 9),
    TRACK_ELEM_FLAG_IS_STEEP_UP = (1 << 10), // Used to allow steep backwards lifts on roller coasters that do not allow steep
                                             // forward lift hills
    TRACK_ELEM_FLAG_HELIX = (1 << 11),
    TRACK_ELEM_FLAG_ALLOW_LIFT_HILL = (1 << 12),
    TRACK_ELEM_FLAG_CURVE_ALLOWS_LIFT = (1 << 13),
    TRACK_ELEM_FLAG_INVERSION_TO_NORMAL = (1 << 14),
    TRACK_ELEM_FLAG_BANKED = (1 << 15), // Also set on Spinning Tunnel and Log Flume reverser, probably to save a flag.
};

enum
{
    TRACK_ELEM_FLAT,
    TRACK_ELEM_END_STATION,
    TRACK_ELEM_BEGIN_STATION,
    TRACK_ELEM_MIDDLE_STATION,
    TRACK_ELEM_25_DEG_UP,
    TRACK_ELEM_60_DEG_UP,
    TRACK_ELEM_FLAT_TO_25_DEG_UP,
    TRACK_ELEM_25_DEG_UP_TO_60_DEG_UP,
    TRACK_ELEM_60_DEG_UP_TO_25_DEG_UP,
    TRACK_ELEM_25_DEG_UP_TO_FLAT,
    TRACK_ELEM_25_DEG_DOWN = 10,
    TRACK_ELEM_60_DEG_DOWN,
    TRACK_ELEM_FLAT_TO_25_DEG_DOWN,
    TRACK_ELEM_25_DEG_DOWN_TO_60_DEG_DOWN,
    TRACK_ELEM_60_DEG_DOWN_TO_25_DEG_DOWN,
    TRACK_ELEM_25_DEG_DOWN_TO_FLAT,
    TRACK_ELEM_LEFT_QUARTER_TURN_5_TILES,
    TRACK_ELEM_RIGHT_QUARTER_TURN_5_TILES,
    TRACK_ELEM_FLAT_TO_LEFT_BANK,
    TRACK_ELEM_FLAT_TO_RIGHT_BANK,
    TRACK_ELEM_LEFT_BANK_TO_FLAT = 20,
    TRACK_ELEM_RIGHT_BANK_TO_FLAT,
    TRACK_ELEM_BANKED_LEFT_QUARTER_TURN_5_TILES,
    TRACK_ELEM_BANKED_RIGHT_QUARTER_TURN_5_TILES,
    TRACK_ELEM_LEFT_BANK_TO_25_DEG_UP,
    TRACK_ELEM_RIGHT_BANK_TO_25_DEG_UP,
    TRACK_ELEM_25_DEG_UP_TO_LEFT_BANK,
    TRACK_ELEM_25_DEG_UP_TO_RIGHT_BANK,
    TRACK_ELEM_LEFT_BANK_TO_25_DEG_DOWN,
    TRACK_ELEM_RIGHT_BANK_TO_25_DEG_DOWN,
    TRACK_ELEM_25_DEG_DOWN_TO_LEFT_BANK = 30,
    TRACK_ELEM_25_DEG_DOWN_TO_RIGHT_BANK,
    TRACK_ELEM_LEFT_BANK,
    TRACK_ELEM_RIGHT_BANK,
    TRACK_ELEM_LEFT_QUARTER_TURN_5_TILES_25_DEG_UP,
    TRACK_ELEM_RIGHT_QUARTER_TURN_5_TILES_25_DEG_UP,
    TRACK_ELEM_LEFT_QUARTER_TURN_5_TILES_25_DEG_DOWN,
    TRACK_ELEM_RIGHT_QUARTER_TURN_5_TILES_25_DEG_DOWN,
    TRACK_ELEM_S_BEND_LEFT,
    TRACK_ELEM_S_BEND_RIGHT,
    TRACK_ELEM_LEFT_VERTICAL_LOOP = 40,
    TRACK_ELEM_RIGHT_VERTICAL_LOOP,
    TRACK_ELEM_LEFT_QUARTER_TURN_3_TILES,
    TRACK_ELEM_RIGHT_QUARTER_TURN_3_TILES,
    TRACK_ELEM_LEFT_QUARTER_TURN_3_TILES_BANK,
    TRACK_ELEM_RIGHT_QUARTER_TURN_3_TILES_BANK,
    TRACK_ELEM_LEFT_QUARTER_TURN_3_TILES_25_DEG_UP,
    TRACK_ELEM_RIGHT_QUARTER_TURN_3_TILES_25_DEG_UP,
    TRACK_ELEM_LEFT_QUARTER_TURN_3_TILES_25_DEG_DOWN,
    TRACK_ELEM_RIGHT_QUARTER_TURN_3_TILES_25_DEG_DOWN,
    TRACK_ELEM_LEFT_QUARTER_TURN_1_TILE = 50,
    TRACK_ELEM_RIGHT_QUARTER_TURN_1_TILE,
    TRACK_ELEM_LEFT_TWIST_DOWN_TO_UP,
    TRACK_ELEM_RIGHT_TWIST_DOWN_TO_UP,
    TRACK_ELEM_LEFT_TWIST_UP_TO_DOWN,
    TRACK_ELEM_RIGHT_TWIST_UP_TO_DOWN,
    TRACK_ELEM_HALF_LOOP_UP,
    TRACK_ELEM_HALF_LOOP_DOWN,
    TRACK_ELEM_LEFT_CORKSCREW_UP,
    TRACK_ELEM_RIGHT_CORKSCREW_UP,
    TRACK_ELEM_LEFT_CORKSCREW_DOWN = 60,
    TRACK_ELEM_RIGHT_CORKSCREW_DOWN,
    TRACK_ELEM_FLAT_TO_60_DEG_UP,
    TRACK_ELEM_60_DEG_UP_TO_FLAT,
    TRACK_ELEM_FLAT_TO_60_DEG_DOWN,
    TRACK_ELEM_60_DEG_DOWN_TO_FLAT,
    TRACK_ELEM_TOWER_BASE,
    TRACK_ELEM_TOWER_SECTION,
    TRACK_ELEM_FLAT_COVERED,
    TRACK_ELEM_25_DEG_UP_COVERED,
    TRACK_ELEM_60_DEG_UP_COVERED = 70,
    TRACK_ELEM_FLAT_TO_25_DEG_UP_COVERED,
    TRACK_ELEM_25_DEG_UP_TO_60_DEG_UP_COVERED,
    TRACK_ELEM_60_DEG_UP_TO_25_DEG_UP_COVERED,
    TRACK_ELEM_25_DEG_UP_TO_FLAT_COVERED,
    TRACK_ELEM_25_DEG_DOWN_COVERED,
    TRACK_ELEM_60_DEG_DOWN_COVERED,
    TRACK_ELEM_FLAT_TO_25_DEG_DOWN_COVERED,
    TRACK_ELEM_25_DEG_DOWN_TO_60_DEG_DOWN_COVERED,
    TRACK_ELEM_60_DEG_DOWN_TO_25_DEG_DOWN_COVERED,
    TRACK_ELEM_25_DEG_DOWN_TO_FLAT_COVERED = 80,
    TRACK_ELEM_LEFT_QUARTER_TURN_5_TILES_COVERED,
    TRACK_ELEM_RIGHT_QUARTER_TURN_5_TILES_COVERED,
    TRACK_ELEM_S_BEND_LEFT_COVERED,
    TRACK_ELEM_S_BEND_RIGHT_COVERED,
    TRACK_ELEM_LEFT_QUARTER_TURN_3_TILES_COVERED,
    TRACK_ELEM_RIGHT_QUARTER_TURN_3_TILES_COVERED,
    TRACK_ELEM_LEFT_HALF_BANKED_HELIX_UP_SMALL,
    TRACK_ELEM_RIGHT_HALF_BANKED_HELIX_UP_SMALL,
    TRACK_ELEM_LEFT_HALF_BANKED_HELIX_DOWN_SMALL,
    TRACK_ELEM_RIGHT_HALF_BANKED_HELIX_DOWN_SMALL = 90,
    TRACK_ELEM_LEFT_HALF_BANKED_HELIX_UP_LARGE,
    TRACK_ELEM_RIGHT_HALF_BANKED_HELIX_UP_LARGE,
    TRACK_ELEM_LEFT_HALF_BANKED_HELIX_DOWN_LARGE,
    TRACK_ELEM_RIGHT_HALF_BANKED_HELIX_DOWN_LARGE,
    TRACK_ELEM_LEFT_QUARTER_TURN_1_TILE_60_DEG_UP,
    TRACK_ELEM_RIGHT_QUARTER_TURN_1_TILE_60_DEG_UP,
    TRACK_ELEM_LEFT_QUARTER_TURN_1_TILE_60_DEG_DOWN,
    TRACK_ELEM_RIGHT_QUARTER_TURN_1_TILE_60_DEG_DOWN,
    TRACK_ELEM_BRAKES,
    TRACK_ELEM_ROTATION_CONTROL_TOGGLE = 100,
    TRACK_ELEM_BOOSTER = 100,
    TRACK_ELEM_MAZE = 101,
    TRACK_ELEM_INVERTED_90_DEG_UP_TO_FLAT_QUARTER_LOOP_ALIAS = 101, // Used by the multi-dimension coaster, as TD6 cannot handle
                                                                    // index 255.
    TRACK_ELEM_LEFT_QUARTER_BANKED_HELIX_LARGE_UP,
    TRACK_ELEM_RIGHT_QUARTER_BANKED_HELIX_LARGE_UP,
    TRACK_ELEM_LEFT_QUARTER_BANKED_HELIX_LARGE_DOWN,
    TRACK_ELEM_RIGHT_QUARTER_BANKED_HELIX_LARGE_DOWN,
    TRACK_ELEM_LEFT_QUARTER_HELIX_LARGE_UP,
    TRACK_ELEM_RIGHT_QUARTER_HELIX_LARGE_UP,
    TRACK_ELEM_LEFT_QUARTER_HELIX_LARGE_DOWN,
    TRACK_ELEM_RIGHT_QUARTER_HELIX_LARGE_DOWN,
    TRACK_ELEM_25_DEG_UP_LEFT_BANKED = 110,
    TRACK_ELEM_25_DEG_UP_RIGHT_BANKED,
    TRACK_ELEM_WATERFALL,
    TRACK_ELEM_RAPIDS,
    TRACK_ELEM_ON_RIDE_PHOTO,
    TRACK_ELEM_25_DEG_DOWN_LEFT_BANKED,
    TRACK_ELEM_25_DEG_DOWN_RIGHT_BANKED,
    TRACK_ELEM_WATER_SPLASH,
    TRACK_ELEM_FLAT_TO_60_DEG_UP_LONG_BASE,
    TRACK_ELEM_60_DEG_UP_TO_FLAT_LONG_BASE,
    TRACK_ELEM_WHIRLPOOL = 120,
    TRACK_ELEM_FLAT_TO_60_DEG_DOWN_LONG_BASE,
    TRACK_ELEM_60_DEG_UP_TO_FLAT_LONG_BASE_122,
    TRACK_ELEM_CABLE_LIFT_HILL,
    TRACK_ELEM_REVERSE_FREEFALL_SLOPE,
    TRACK_ELEM_REVERSE_FREEFALL_VERTICAL,
    TRACK_ELEM_90_DEG_UP,
    TRACK_ELEM_90_DEG_DOWN,
    TRACK_ELEM_60_DEG_UP_TO_90_DEG_UP,
    TRACK_ELEM_90_DEG_DOWN_TO_60_DEG_DOWN,
    TRACK_ELEM_90_DEG_UP_TO_60_DEG_UP = 130,
    TRACK_ELEM_60_DEG_DOWN_TO_90_DEG_DOWN,
    TRACK_ELEM_BRAKE_FOR_DROP,
    TRACK_ELEM_LEFT_EIGHTH_TO_DIAG,
    TRACK_ELEM_RIGHT_EIGHTH_TO_DIAG,
    TRACK_ELEM_LEFT_EIGHTH_TO_ORTHOGONAL,
    TRACK_ELEM_RIGHT_EIGHTH_TO_ORTHOGONAL,
    TRACK_ELEM_LEFT_EIGHTH_BANK_TO_DIAG,
    TRACK_ELEM_RIGHT_EIGHTH_BANK_TO_DIAG,
    TRACK_ELEM_LEFT_EIGHTH_BANK_TO_ORTHOGONAL,
    TRACK_ELEM_RIGHT_EIGHTH_BANK_TO_ORTHOGONAL = 140,
    TRACK_ELEM_DIAG_FLAT,
    TRACK_ELEM_DIAG_25_DEG_UP,
    TRACK_ELEM_DIAG_60_DEG_UP,
    TRACK_ELEM_DIAG_FLAT_TO_25_DEG_UP,
    TRACK_ELEM_DIAG_25_DEG_UP_TO_60_DEG_UP,
    TRACK_ELEM_DIAG_60_DEG_UP_TO_25_DEG_UP,
    TRACK_ELEM_DIAG_25_DEG_UP_TO_FLAT,
    TRACK_ELEM_DIAG_25_DEG_DOWN,
    TRACK_ELEM_DIAG_60_DEG_DOWN,
    TRACK_ELEM_DIAG_FLAT_TO_25_DEG_DOWN = 150,
    TRACK_ELEM_DIAG_25_DEG_DOWN_TO_60_DEG_DOWN,
    TRACK_ELEM_DIAG_60_DEG_DOWN_TO_25_DEG_DOWN,
    TRACK_ELEM_DIAG_25_DEG_DOWN_TO_FLAT,
    TRACK_ELEM_DIAG_FLAT_TO_60_DEG_UP,
    TRACK_ELEM_DIAG_60_DEG_UP_TO_FLAT,
    TRACK_ELEM_DIAG_FLAT_TO_60_DEG_DOWN,
    TRACK_ELEM_DIAG_60_DEG_DOWN_TO_FLAT,
    TRACK_ELEM_DIAG_FLAT_TO_LEFT_BANK,
    TRACK_ELEM_DIAG_FLAT_TO_RIGHT_BANK,
    TRACK_ELEM_DIAG_LEFT_BANK_TO_FLAT = 160,
    TRACK_ELEM_DIAG_RIGHT_BANK_TO_FLAT,
    TRACK_ELEM_DIAG_LEFT_BANK_TO_25_DEG_UP,
    TRACK_ELEM_DIAG_RIGHT_BANK_TO_25_DEG_UP,
    TRACK_ELEM_DIAG_25_DEG_UP_TO_LEFT_BANK,
    TRACK_ELEM_DIAG_25_DEG_UP_TO_RIGHT_BANK,
    TRACK_ELEM_DIAG_LEFT_BANK_TO_25_DEG_DOWN,
    TRACK_ELEM_DIAG_RIGHT_BANK_TO_25_DEG_DOWN,
    TRACK_ELEM_DIAG_25_DEG_DOWN_TO_LEFT_BANK,
    TRACK_ELEM_DIAG_25_DEG_DOWN_TO_RIGHT_BANK,
    TRACK_ELEM_DIAG_LEFT_BANK = 170,
    TRACK_ELEM_DIAG_RIGHT_BANK,
    TRACK_ELEM_LOG_FLUME_REVERSER,
    TRACK_ELEM_SPINNING_TUNNEL,
    TRACK_ELEM_LEFT_BARREL_ROLL_UP_TO_DOWN,
    TRACK_ELEM_RIGHT_BARREL_ROLL_UP_TO_DOWN,
    TRACK_ELEM_LEFT_BARREL_ROLL_DOWN_TO_UP,
    TRACK_ELEM_RIGHT_BARREL_ROLL_DOWN_TO_UP,
    TRACK_ELEM_LEFT_BANK_TO_LEFT_QUARTER_TURN_3_TILES_25_DEG_UP,
    TRACK_ELEM_RIGHT_BANK_TO_RIGHT_QUARTER_TURN_3_TILES_25_DEG_UP,
    TRACK_ELEM_LEFT_QUARTER_TURN_3_TILES_25_DEG_DOWN_TO_LEFT_BANK = 180,
    TRACK_ELEM_RIGHT_QUARTER_TURN_3_TILES_25_DEG_DOWN_TO_RIGHT_BANK,
    TRACK_ELEM_POWERED_LIFT,
    TRACK_ELEM_LEFT_LARGE_HALF_LOOP_UP,
    TRACK_ELEM_RIGHT_LARGE_HALF_LOOP_UP,
    TRACK_ELEM_RIGHT_LARGE_HALF_LOOP_DOWN,
    TRACK_ELEM_LEFT_LARGE_HALF_LOOP_DOWN,
    TRACK_ELEM_LEFT_FLYER_TWIST_UP,
    TRACK_ELEM_RIGHT_FLYER_TWIST_UP,
    TRACK_ELEM_LEFT_FLYER_TWIST_DOWN,
    TRACK_ELEM_RIGHT_FLYER_TWIST_DOWN = 190,
    TRACK_ELEM_FLYER_HALF_LOOP_UP,
    TRACK_ELEM_FLYER_HALF_LOOP_DOWN,
    TRACK_ELEM_LEFT_FLYER_CORKSCREW_UP,
    TRACK_ELEM_RIGHT_FLYER_CORKSCREW_UP,
    TRACK_ELEM_LEFT_FLYER_CORKSCREW_DOWN,
    TRACK_ELEM_RIGHT_FLYER_CORKSCREW_DOWN,
    TRACK_ELEM_HEARTLINE_TRANSFER_UP,
    TRACK_ELEM_HEARTLINE_TRANSFER_DOWN,
    TRACK_ELEM_LEFT_HEARTLINE_ROLL,
    TRACK_ELEM_RIGHT_HEARTLINE_ROLL = 200,
    TRACK_ELEM_MINI_GOLF_HOLE_A,
    TRACK_ELEM_MINI_GOLF_HOLE_B,
    TRACK_ELEM_MINI_GOLF_HOLE_C,
    TRACK_ELEM_MINI_GOLF_HOLE_D,
    TRACK_ELEM_MINI_GOLF_HOLE_E,
    TRACK_ELEM_MULTIDIM_INVERTED_FLAT_TO_90_DEG_QUARTER_LOOP_DOWN,
    TRACK_ELEM_90_DEG_TO_INVERTED_FLAT_QUARTER_LOOP_UP,
    TRACK_ELEM_INVERTED_FLAT_TO_90_DEG_QUARTER_LOOP_DOWN,
    TRACK_ELEM_LEFT_CURVED_LIFT_HILL,
    TRACK_ELEM_RIGHT_CURVED_LIFT_HILL = 210,
    TRACK_ELEM_LEFT_REVERSER,
    TRACK_ELEM_RIGHT_REVERSER,
    TRACK_ELEM_AIR_THRUST_TOP_CAP,
    TRACK_ELEM_AIR_THRUST_VERTICAL_DOWN,
    TRACK_ELEM_AIR_THRUST_VERTICAL_DOWN_TO_LEVEL,
    TRACK_ELEM_BLOCK_BRAKES,
    TRACK_ELEM_LEFT_BANKED_QUARTER_TURN_3_TILE_25_DEG_UP,
    TRACK_ELEM_RIGHT_BANKED_QUARTER_TURN_3_TILE_25_DEG_UP,
    TRACK_ELEM_LEFT_BANKED_QUARTER_TURN_3_TILE_25_DEG_DOWN,
    TRACK_ELEM_RIGHT_BANKED_QUARTER_TURN_3_TILE_25_DEG_DOWN,
    TRACK_ELEM_LEFT_BANKED_QUARTER_TURN_5_TILE_25_DEG_UP,
    TRACK_ELEM_RIGHT_BANKED_QUARTER_TURN_5_TILE_25_DEG_UP,
    TRACK_ELEM_LEFT_BANKED_QUARTER_TURN_5_TILE_25_DEG_DOWN,
    TRACK_ELEM_RIGHT_BANKED_QUARTER_TURN_5_TILE_25_DEG_DOWN,
    TRACK_ELEM_25_DEG_UP_TO_LEFT_BANKED_25_DEG_UP,
    TRACK_ELEM_25_DEG_UP_TO_RIGHT_BANKED_25_DEG_UP,
    TRACK_ELEM_LEFT_BANKED_25_DEG_UP_TO_25_DEG_UP,
    TRACK_ELEM_RIGHT_BANKED_25_DEG_UP_TO_25_DEG_UP,
    TRACK_ELEM_25_DEG_DOWN_TO_LEFT_BANKED_25_DEG_DOWN,
    TRACK_ELEM_25_DEG_DOWN_TO_RIGHT_BANKED_25_DEG_DOWN,
    TRACK_ELEM_LEFT_BANKED_25_DEG_DOWN_TO_25_DEG_DOWN,
    TRACK_ELEM_RIGHT_BANKED_25_DEG_DOWN_TO_25_DEG_DOWN,
    TRACK_ELEM_LEFT_BANKED_FLAT_TO_LEFT_BANKED_25_DEG_UP,
    TRACK_ELEM_RIGHT_BANKED_FLAT_TO_RIGHT_BANKED_25_DEG_UP,
    TRACK_ELEM_LEFT_BANKED_25_DEG_UP_TO_LEFT_BANKED_FLAT,
    TRACK_ELEM_RIGHT_BANKED_25_DEG_UP_TO_RIGHT_BANKED_FLAT,
    TRACK_ELEM_LEFT_BANKED_FLAT_TO_LEFT_BANKED_25_DEG_DOWN,
    TRACK_ELEM_RIGHT_BANKED_FLAT_TO_RIGHT_BANKED_25_DEG_DOWN,
    TRACK_ELEM_LEFT_BANKED_25_DEG_DOWN_TO_LEFT_BANKED_FLAT,
    TRACK_ELEM_RIGHT_BANKED_25_DEG_DOWN_TO_RIGHT_BANKED_FLAT,
    TRACK_ELEM_FLAT_TO_LEFT_BANKED_25_DEG_UP,
    TRACK_ELEM_FLAT_TO_RIGHT_BANKED_25_DEG_UP,
    TRACK_ELEM_LEFT_BANKED_25_DEG_UP_TO_FLAT,
    TRACK_ELEM_RIGHT_BANKED_25_DEG_UP_TO_FLAT,
    TRACK_ELEM_FLAT_TO_LEFT_BANKED_25_DEG_DOWN,
    TRACK_ELEM_FLAT_TO_RIGHT_BANKED_25_DEG_DOWN,
    TRACK_ELEM_LEFT_BANKED_25_DEG_DOWN_TO_FLAT,
    TRACK_ELEM_RIGHT_BANKED_25_DEG_DOWN_TO_FLAT,
    TRACK_ELEM_LEFT_QUARTER_TURN_1_TILE_90_DEG_UP,
    TRACK_ELEM_RIGHT_QUARTER_TURN_1_TILE_90_DEG_UP,
    TRACK_ELEM_LEFT_QUARTER_TURN_1_TILE_90_DEG_DOWN,
    TRACK_ELEM_RIGHT_QUARTER_TURN_1_TILE_90_DEG_DOWN,
    TRACK_ELEM_MULTIDIM_90_DEG_UP_TO_INVERTED_FLAT_QUARTER_LOOP,
    TRACK_ELEM_MULTIDIM_FLAT_TO_90_DEG_DOWN_QUARTER_LOOP,
    TRACK_ELEM_MULTIDIM_INVERTED_90_DEG_UP_TO_FLAT_QUARTER_LOOP,

    TRACK_ELEM_COUNT,
};

enum
{
    FLAT_TRACK_ELEM_1_X_4_A = 95,
    FLAT_TRACK_ELEM_2_X_2 = 110,
    FLAT_TRACK_ELEM_4_X_4 = 111,
    FLAT_TRACK_ELEM_1_X_5 = 116,
    FLAT_TRACK_ELEM_1_X_1_A = 118,
    FLAT_TRACK_ELEM_1_X_4_B = 119,
    FLAT_TRACK_ELEM_1_X_1_B = 121,
    FLAT_TRACK_ELEM_1_X_4_C = 122,
    FLAT_TRACK_ELEM_3_X_3 = 123,
};

enum
{
    TRACK_SEQUENCE_FLAG_DIRECTION_0 = (1 << 0),
    TRACK_SEQUENCE_FLAG_DIRECTION_1 = (1 << 1),
    TRACK_SEQUENCE_FLAG_DIRECTION_2 = (1 << 2),
    TRACK_SEQUENCE_FLAG_DIRECTION_3 = (1 << 3),
    TRACK_SEQUENCE_FLAG_ORIGIN = (1 << 4),           // 0x10
    TRACK_SEQUENCE_FLAG_CONNECTS_TO_PATH = (1 << 5), // 0x20
    TRACK_SEQUENCE_FLAG_DISALLOW_DOORS = (1 << 6),   // 0x40
};

enum
{
    TRACK_ELEMENT_LOCATION_IS_UNDERGROUND = 2,
};

enum
{
    GC_SET_MAZE_TRACK_BUILD = 0,
    GC_SET_MAZE_TRACK_MOVE = 1,
    GC_SET_MAZE_TRACK_FILL = 2,
};

struct track_circuit_iterator
{
    CoordsXYE last;
    CoordsXYE current;
    int32_t currentZ;
    int32_t currentDirection;
    TileElement* first;
    bool firstIteration;
    bool looped;
};

extern const rct_trackdefinition FlatRideTrackDefinitions[256];
extern const rct_trackdefinition TrackDefinitions[256];

int32_t track_is_connected_by_shape(TileElement* a, TileElement* b);

const rct_preview_track* get_track_def_from_ride(Ride* ride, int32_t trackType);
const rct_preview_track* get_track_def_from_ride_index(ride_id_t rideIndex, int32_t trackType);
const rct_track_coordinates* get_track_coord_from_ride(Ride* ride, int32_t trackType);

void track_circuit_iterator_begin(track_circuit_iterator* it, CoordsXYE first);
bool track_circuit_iterator_previous(track_circuit_iterator* it);
bool track_circuit_iterator_next(track_circuit_iterator* it);
bool track_circuit_iterators_match(const track_circuit_iterator* firstIt, const track_circuit_iterator* secondIt);

void track_get_back(CoordsXYE* input, CoordsXYE* output);
void track_get_front(CoordsXYE* input, CoordsXYE* output);

bool track_element_is_block_start(TileElement* trackElement);
bool track_element_is_covered(int32_t trackElementType);
bool track_type_is_station(track_type_t trackType);

int32_t track_get_actual_bank(TileElement* tileElement, int32_t bank);
int32_t track_get_actual_bank_2(int32_t rideType, bool isInverted, int32_t bank);
int32_t track_get_actual_bank_3(Vehicle* vehicle, TileElement* tileElement);

bool track_add_station_element(CoordsXYZD loc, ride_id_t rideIndex, int32_t flags, bool fromTrackDesign);
bool track_remove_station_element(int32_t x, int32_t y, int32_t z, Direction direction, ride_id_t rideIndex, int32_t flags);

money32 maze_set_track(
    uint16_t x, uint16_t y, uint16_t z, uint8_t flags, bool initialPlacement, uint8_t direction, ride_id_t rideIndex,
    uint8_t mode);

bool track_element_is_booster(uint8_t rideType, uint8_t trackType);
bool track_element_has_speed_setting(uint8_t trackType);
