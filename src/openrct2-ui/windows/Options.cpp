/*****************************************************************************
 * Copyright (c) 2014-2019 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

/**
 * To better group the options together and allow the window to be scalable with additional OpenRCT2 options,
 * the window has been changed to a tab interface similar to the options window seen in Locomotion.
 */

#include "../interface/Theme.h"

#include <algorithm>
#include <cmath>
#include <openrct2-ui/interface/Dropdown.h>
#include <openrct2-ui/interface/Viewport.h>
#include <openrct2-ui/interface/Widget.h>
#include <openrct2-ui/windows/Window.h>
#include <openrct2/Context.h>
#include <openrct2/audio/AudioMixer.h>
#include <openrct2/audio/audio.h>
#include <openrct2/config/Config.h>
#include <openrct2/core/String.hpp>
#include <openrct2/drawing/IDrawingEngine.h>
#include <openrct2/localisation/Currency.h>
#include <openrct2/localisation/Date.h>
#include <openrct2/localisation/Language.h>
#include <openrct2/localisation/Localisation.h>
#include <openrct2/localisation/LocalisationService.h>
#include <openrct2/network/network.h>
#include <openrct2/platform/Platform2.h>
#include <openrct2/platform/platform.h>
#include <openrct2/scenario/Scenario.h>
#include <openrct2/sprites.h>
#include <openrct2/title/TitleScreen.h>
#include <openrct2/title/TitleSequenceManager.h>
#include <openrct2/ui/UiContext.h>
#include <openrct2/util/Util.h>

// clang-format off
enum WINDOW_OPTIONS_PAGE {
    WINDOW_OPTIONS_PAGE_DISPLAY,
    WINDOW_OPTIONS_PAGE_RENDERING,
    WINDOW_OPTIONS_PAGE_CULTURE,
    WINDOW_OPTIONS_PAGE_AUDIO,
    WINDOW_OPTIONS_PAGE_CONTROLS_AND_INTERFACE,
    WINDOW_OPTIONS_PAGE_MISC,
    WINDOW_OPTIONS_PAGE_ADVANCED,
    WINDOW_OPTIONS_PAGE_COUNT
};

#pragma region Widgets

enum WINDOW_OPTIONS_WIDGET_IDX {
    WIDX_BACKGROUND,
    WIDX_TITLE,
    WIDX_CLOSE,
    WIDX_PAGE_BACKGROUND,
    WIDX_TAB_1,
    WIDX_TAB_2,
    WIDX_TAB_3,
    WIDX_TAB_4,
    WIDX_TAB_5,
    WIDX_TAB_6,
    WIDX_TAB_7,

    WIDX_PAGE_START,

    // Display
    WIDX_HARDWARE_GROUP = WIDX_PAGE_START,
    WIDX_FULLSCREEN,
    WIDX_FULLSCREEN_DROPDOWN,
    WIDX_RESOLUTION,
    WIDX_RESOLUTION_DROPDOWN,
    WIDX_SCALE,
    WIDX_SCALE_UP,
    WIDX_SCALE_DOWN,
    WIDX_DRAWING_ENGINE,
    WIDX_DRAWING_ENGINE_DROPDOWN,
    WIDX_SCALE_QUALITY,
    WIDX_SCALE_QUALITY_DROPDOWN,
    WIDX_STEAM_OVERLAY_PAUSE,
    WIDX_UNCAP_FPS_CHECKBOX,
    WIDX_SHOW_FPS_CHECKBOX,
    WIDX_MULTITHREADING_CHECKBOX,
    WIDX_USE_VSYNC_CHECKBOX,
    WIDX_MINIMIZE_FOCUS_LOSS,

    // Rendering
    WIDX_RENDERING_GROUP = WIDX_PAGE_START,
    WIDX_TILE_SMOOTHING_CHECKBOX,
    WIDX_GRIDLINES_CHECKBOX,
    WIDX_UPPER_CASE_BANNERS_CHECKBOX,
    WIDX_SHOW_GUEST_PURCHASES_CHECKBOX,
    WIDX_TRANSPARENT_SCREENSHOTS_CHECKBOX,
    WIDX_VIRTUAL_FLOOR_LABEL,
    WIDX_VIRTUAL_FLOOR,
    WIDX_VIRTUAL_FLOOR_DROPDOWN,
    WIDX_EFFECTS_GROUP,
    WIDX_DAY_NIGHT_CHECKBOX,
    WIDX_ENABLE_LIGHT_FX_CHECKBOX,
    WIDX_ENABLE_LIGHT_FX_FOR_VEHICLES_CHECKBOX,
    WIDX_RENDER_WEATHER_EFFECTS_CHECKBOX,
    WIDX_DISABLE_LIGHTNING_EFFECT_CHECKBOX,

    // Culture / Units
    WIDX_LANGUAGE = WIDX_PAGE_START,
    WIDX_LANGUAGE_DROPDOWN,
    WIDX_CURRENCY,
    WIDX_CURRENCY_DROPDOWN,
    WIDX_DISTANCE,
    WIDX_DISTANCE_DROPDOWN,
    WIDX_TEMPERATURE,
    WIDX_TEMPERATURE_DROPDOWN,
    WIDX_HEIGHT_LABELS,
    WIDX_HEIGHT_LABELS_DROPDOWN,
    WIDX_DATE_FORMAT,
    WIDX_DATE_FORMAT_DROPDOWN,

    // Audio
    WIDX_SOUND = WIDX_PAGE_START,
    WIDX_SOUND_DROPDOWN,
    WIDX_MASTER_SOUND_CHECKBOX,
    WIDX_SOUND_CHECKBOX,
    WIDX_MUSIC_CHECKBOX,
    WIDX_AUDIO_FOCUS_CHECKBOX,
    WIDX_TITLE_MUSIC_LABEL,
    WIDX_TITLE_MUSIC,
    WIDX_TITLE_MUSIC_DROPDOWN,
    WIDX_MASTER_VOLUME,
    WIDX_SOUND_VOLUME,
    WIDX_MUSIC_VOLUME,

    // Controls and interface
    WIDX_CONTROLS_GROUP = WIDX_PAGE_START,
    WIDX_SCREEN_EDGE_SCROLLING,
    WIDX_TRAP_CURSOR,
    WIDX_INVERT_DRAG,
    WIDX_ZOOM_TO_CURSOR,
    WIDX_HOTKEY_DROPDOWN,
    WIDX_THEMES_GROUP,
    WIDX_THEMES,
    WIDX_THEMES_DROPDOWN,
    WIDX_THEMES_BUTTON,
    WIDX_TOOLBAR_BUTTONS_GROUP,
    WIDX_TOOLBAR_SHOW_FINANCES,
    WIDX_TOOLBAR_SHOW_RESEARCH,
    WIDX_TOOLBAR_SHOW_CHEATS,
    WIDX_TOOLBAR_SHOW_NEWS,
    WIDX_TOOLBAR_SHOW_MUTE,
    WIDX_TOOLBAR_SHOW_CHAT,

    // Misc
    WIDX_TITLE_SEQUENCE_GROUP = WIDX_PAGE_START,
    WIDX_TITLE_SEQUENCE_RANDOM,
    WIDX_TITLE_SEQUENCE,
    WIDX_TITLE_SEQUENCE_DROPDOWN,
    WIDX_TITLE_SEQUENCE_BUTTON,
    WIDX_SCENARIO_GROUP,
    WIDX_SCENARIO_GROUPING,
    WIDX_SCENARIO_GROUPING_DROPDOWN,
    WIDX_SCENARIO_UNLOCKING,
    WIDX_SCENARIO_OPTIONS_GROUP,
    WIDX_ALLOW_EARLY_COMPLETION,
    WIDX_TWEAKS_GROUP,
    WIDX_REAL_NAME_CHECKBOX,
    WIDX_AUTO_STAFF_PLACEMENT,
    WIDX_AUTO_OPEN_SHOPS,
    WIDX_DEFAULT_INSPECTION_INTERVAL,
    WIDX_DEFAULT_INSPECTION_INTERVAL_DROPDOWN,

    // Advanced
    WIDX_DEBUGGING_TOOLS = WIDX_PAGE_START,
    WIDX_ALLOW_LOADING_WITH_INCORRECT_CHECKSUM,
    WIDX_SAVE_PLUGIN_DATA_CHECKBOX,
    WIDX_STAY_CONNECTED_AFTER_DESYNC,
    WIDX_ALWAYS_NATIVE_LOADSAVE,
    WIDX_AUTOSAVE,
    WIDX_AUTOSAVE_DROPDOWN,
    WIDX_AUTOSAVE_AMOUNT,
    WIDX_AUTOSAVE_AMOUNT_UP,
    WIDX_AUTOSAVE_AMOUNT_DOWN,
    WIDX_PATH_TO_RCT1_TEXT,
    WIDX_PATH_TO_RCT1_BUTTON,
    WIDX_PATH_TO_RCT1_CLEAR,
};

static constexpr const rct_string_id WINDOW_TITLE = STR_OPTIONS_TITLE;
static constexpr const int32_t WW = 310;
static constexpr const int32_t WH = 332;

#define MAIN_OPTIONS_WIDGETS \
    WINDOW_SHIM(WINDOW_TITLE, WW, WH), \
    { WWT_RESIZE,           1,  0,      WW-1,   43,     WH-1,   0xFFFFFFFF,             STR_NONE }, \
    { WWT_TAB,              1,  3,      33,     17,     43,     IMAGE_TYPE_REMAP | SPR_TAB,   STR_OPTIONS_DISPLAY_TIP }, \
    { WWT_TAB,              1,  34,     64,     17,     43,     IMAGE_TYPE_REMAP | SPR_TAB,   STR_OPTIONS_RENDERING_TIP }, \
    { WWT_TAB,              1,  65,     95,     17,     43,     IMAGE_TYPE_REMAP | SPR_TAB,   STR_OPTIONS_CULTURE_TIP }, \
    { WWT_TAB,              1,  96,     126,    17,     43,     IMAGE_TYPE_REMAP | SPR_TAB,   STR_OPTIONS_AUDIO_TIP }, \
    { WWT_TAB,              1,  127,    157,    17,     43,     IMAGE_TYPE_REMAP | SPR_TAB,   STR_OPTIONS_CONTROLS_AND_INTERFACE_TIP }, \
    { WWT_TAB,              1,  158,    188,    17,     43,     IMAGE_TYPE_REMAP | SPR_TAB,   STR_OPTIONS_MISCELLANEOUS_TIP }, \
    { WWT_TAB,              1,  189,    219,    17,     43,     IMAGE_TYPE_REMAP | SPR_TAB,   STR_OPTIONS_ADVANCED }

static rct_widget window_options_display_widgets[] = {
    MAIN_OPTIONS_WIDGETS,
    { WWT_GROUPBOX,         1,  5,      304,    53,     207,    STR_HARDWARE_GROUP,     STR_NONE },                 // Hardware group
    { WWT_DROPDOWN,         1,  155,    299,    68,     79,     STR_NONE,               STR_NONE },                 // Fullscreen
    { WWT_BUTTON,           1,  288,    298,    69,     78,     STR_DROPDOWN_GLYPH,     STR_FULLSCREEN_MODE_TIP },
    { WWT_DROPDOWN,         1,  155,    299,    83,     94,     STR_ARG_16_RESOLUTION_X_BY_Y,   STR_NONE },             // Resolution
    { WWT_BUTTON,           1,  288,    298,    84,     93,     STR_DROPDOWN_GLYPH,     STR_DISPLAY_RESOLUTION_TIP },
      SPINNER_WIDGETS      (1,  155,    299,    98,    109,     STR_NONE,               STR_WINDOW_SCALE_TIP), // Scale spinner (3 widgets)
    { WWT_DROPDOWN,         1,  155,    299,    113,    124,    STR_NONE,               STR_NONE },
    { WWT_BUTTON,           1,  288,    298,    114,    123,    STR_DROPDOWN_GLYPH,     STR_DRAWING_ENGINE_TIP },
    { WWT_DROPDOWN,         1,  155,    299,    128,    139,    STR_NONE,                       STR_NONE },                         // Scaling quality hint
    { WWT_BUTTON,           1,  288,    298,    129,    138,    STR_DROPDOWN_GLYPH,             STR_SCALE_QUALITY_TIP },
    { WWT_CHECKBOX,         1,  25,     290,    144,    155,    STR_STEAM_OVERLAY_PAUSE,        STR_STEAM_OVERLAY_PAUSE_TIP },      // Pause on steam overlay
    { WWT_CHECKBOX,         1,  11,     153,    161,    172,    STR_UNCAP_FPS,          STR_UNCAP_FPS_TIP },        // Uncap fps
    { WWT_CHECKBOX,         1,  155,    290,    161,    172,    STR_SHOW_FPS,           STR_SHOW_FPS_TIP },         // Show fps
    { WWT_CHECKBOX,         1,  155,    290,    176,    187,    STR_MULTITHREADING,           STR_MULTITHREADING_TIP },         // Multithreading
    { WWT_CHECKBOX,         1,  11,     153,    176,    187,    STR_USE_VSYNC,          STR_USE_VSYNC_TIP },        // Use vsync
    { WWT_CHECKBOX,         1,  11,     290,    191,    202,    STR_MINIMISE_FULLSCREEN_ON_FOCUS_LOSS,  STR_MINIMISE_FULLSCREEN_ON_FOCUS_LOSS_TIP },    // Minimise fullscreen focus loss
    { WIDGETS_END },
};

static rct_widget window_options_rendering_widgets[] = {
    MAIN_OPTIONS_WIDGETS,
#define FRAME_RENDERING_START 53
    { WWT_GROUPBOX,         1,  5,      304,    FRAME_RENDERING_START + 0,      FRAME_RENDERING_START + 107,    STR_RENDERING_GROUP,            STR_NONE },                             // Rendering group
    { WWT_CHECKBOX,         1,  10,     290,    FRAME_RENDERING_START + 15,     FRAME_RENDERING_START + 26,     STR_TILE_SMOOTHING,             STR_TILE_SMOOTHING_TIP },               // Landscape smoothing
    { WWT_CHECKBOX,         1,  10,     290,    FRAME_RENDERING_START + 30,     FRAME_RENDERING_START + 41,     STR_GRIDLINES,                  STR_GRIDLINES_TIP },                    // Gridlines
    { WWT_CHECKBOX,         1,  10,     290,    FRAME_RENDERING_START + 45,     FRAME_RENDERING_START + 56,     STR_UPPERCASE_BANNERS,          STR_UPPERCASE_BANNERS_TIP },            // Uppercase banners
    { WWT_CHECKBOX,         1,  10,     290,    FRAME_RENDERING_START + 60,     FRAME_RENDERING_START + 71,     STR_SHOW_GUEST_PURCHASES,       STR_SHOW_GUEST_PURCHASES_TIP },         // Guest purchases
    { WWT_CHECKBOX,         1,  10,     290,    FRAME_RENDERING_START + 75,     FRAME_RENDERING_START + 86,     STR_TRANSPARENT_SCREENSHOT,     STR_TRANSPARENT_SCREENSHOT_TIP },       // Transparent screenshot
    { WWT_LABEL,            1,  10,     290,    FRAME_RENDERING_START + 90,     FRAME_RENDERING_START + 101,    STR_VIRTUAL_FLOOR_STYLE,        STR_NONE },                             // Virtual floor
    { WWT_DROPDOWN,         1,  155,    299,    FRAME_RENDERING_START + 90,     FRAME_RENDERING_START + 101,    STR_NONE,                       STR_VIRTUAL_FLOOR_STYLE_TIP },          // Virtual floor dropdown
    { WWT_BUTTON,           1,  288,    298,    FRAME_RENDERING_START + 91,     FRAME_RENDERING_START + 100,    STR_DROPDOWN_GLYPH,             STR_VIRTUAL_FLOOR_STYLE_TIP },          // Virtual floor dropdown
#undef FRAME_RENDERING_START
#define FRAME_EFFECTS_START 163
    { WWT_GROUPBOX,         1,  5,      304,    FRAME_EFFECTS_START + 0,        FRAME_EFFECTS_START + 93,       STR_EFFECTS_GROUP,              STR_NONE },                             // Rendering group
    { WWT_CHECKBOX,         1,  10,     290,    FRAME_EFFECTS_START + 15,       FRAME_EFFECTS_START + 26,       STR_CYCLE_DAY_NIGHT,            STR_CYCLE_DAY_NIGHT_TIP },              // Cycle day-night
    { WWT_CHECKBOX,         1,  25,     290,    FRAME_EFFECTS_START + 30,       FRAME_EFFECTS_START + 41,       STR_ENABLE_LIGHTING_EFFECTS,    STR_ENABLE_LIGHTING_EFFECTS_TIP },      // Enable light fx
    { WWT_CHECKBOX,         1,  40,     290,    FRAME_EFFECTS_START + 45,       FRAME_EFFECTS_START + 56,       STR_ENABLE_LIGHTING_VEHICLES,   STR_ENABLE_LIGHTING_VEHICLES_TIP },     // Enable light fx for vehicles
    { WWT_CHECKBOX,         1,  10,     290,    FRAME_EFFECTS_START + 60,       FRAME_EFFECTS_START + 71,       STR_RENDER_WEATHER_EFFECTS,     STR_RENDER_WEATHER_EFFECTS_TIP },       // Render weather effects
    { WWT_CHECKBOX,         1,  25,     290,    FRAME_EFFECTS_START + 75,       FRAME_EFFECTS_START + 86,       STR_DISABLE_LIGHTNING_EFFECT,   STR_DISABLE_LIGHTNING_EFFECT_TIP },     // Disable lightning effect
#undef FRAME_EFFECTS_START
    { WIDGETS_END },
};

static rct_widget window_options_culture_widgets[] = {
    MAIN_OPTIONS_WIDGETS,
    { WWT_DROPDOWN,         1,  155,    299,    53,     64,     STR_STRING,             STR_NONE }, // language
    { WWT_BUTTON,           1,  288,    298,    54,     63,     STR_DROPDOWN_GLYPH,     STR_LANGUAGE_TIP },
    { WWT_DROPDOWN,         1,  155,    299,    68,     79,     STR_NONE,               STR_NONE }, // Currency
    { WWT_BUTTON,           1,  288,    298,    69,     78,     STR_DROPDOWN_GLYPH,     STR_CURRENCY_TIP },
    { WWT_DROPDOWN,         1,  155,    299,    83,     94,     STR_NONE,               STR_NONE }, // Distance and speed
    { WWT_BUTTON,           1,  288,    298,    84,     93,     STR_DROPDOWN_GLYPH,     STR_DISTANCE_AND_SPEED_TIP },
    { WWT_DROPDOWN,         1,  155,    299,    98,     110,    STR_NONE,               STR_NONE }, // Temperature
    { WWT_BUTTON,           1,  288,    298,    99,     108,    STR_DROPDOWN_GLYPH,     STR_TEMPERATURE_FORMAT_TIP },
    { WWT_DROPDOWN,         1,  155,    299,    113,    124,    STR_NONE,               STR_NONE }, // Height labels
    { WWT_BUTTON,           1,  288,    298,    114,    123,    STR_DROPDOWN_GLYPH,     STR_HEIGHT_LABELS_UNITS_TIP },
    { WWT_DROPDOWN,         1,  155,    299,    128,    139,    STR_NONE,               STR_NONE }, // Date format
    { WWT_BUTTON,           1,  288,    298,    129,    138,    STR_DROPDOWN_GLYPH,     STR_DATE_FORMAT_TIP },
    { WIDGETS_END },
};

static rct_widget window_options_audio_widgets[] = {
    MAIN_OPTIONS_WIDGETS,
    { WWT_DROPDOWN,         1,  10,     299,    53,     64,     STR_NONE,               STR_NONE },                     // Audio device
    { WWT_BUTTON,           1,  288,    298,    54,     63,     STR_DROPDOWN_GLYPH,     STR_AUDIO_DEVICE_TIP },
    { WWT_CHECKBOX,         1,  10,     229,    69,     80,     STR_MASTER_VOLUME,      STR_MASTER_VOLUME_TIP },        // Enable / disable master sound
    { WWT_CHECKBOX,         1,  10,     229,    84,     95,     STR_SOUND_EFFECTS,      STR_SOUND_EFFECTS_TIP },        // Enable / disable sound effects
    { WWT_CHECKBOX,         1,  10,     229,    99,     110,    STR_RIDE_MUSIC,         STR_RIDE_MUSIC_TIP },           // Enable / disable ride music
    { WWT_CHECKBOX,         1,  10,     299,    113,    125,    STR_AUDIO_FOCUS,        STR_AUDIO_FOCUS_TIP },          // Enable / disable audio disabled on focus lost
    { WWT_LABEL,            1,  10,     154,    128,    140,    STR_OPTIONS_MUSIC_LABEL,STR_NONE },                     // Title music label
    { WWT_DROPDOWN,         1,  155,    299,    127,    139,    STR_NONE,               STR_NONE },                     // Title music
    { WWT_BUTTON,           1,  288,    298,    128,    138,    STR_DROPDOWN_GLYPH,     STR_TITLE_MUSIC_TIP },
    { WWT_SCROLL,           1,  155,    299,    68,     80,     SCROLL_HORIZONTAL,      STR_NONE },                     // Master volume
    { WWT_SCROLL,           1,  155,    299,    83,     95,     SCROLL_HORIZONTAL,      STR_NONE },                     // Sound effect volume
    { WWT_SCROLL,           1,  155,    299,    98,     110,    SCROLL_HORIZONTAL,      STR_NONE },                     // Music volume
    { WIDGETS_END },
};

static rct_widget window_options_controls_and_interface_widgets[] = {
    MAIN_OPTIONS_WIDGETS,
#define CONTROLS_GROUP_START 53
    { WWT_GROUPBOX,         1,  5,      304,    CONTROLS_GROUP_START + 0,    CONTROLS_GROUP_START + 91,   STR_CONTROLS_GROUP,                     STR_NONE },                                 // Controls group
    { WWT_CHECKBOX,         2,  10,     299,    CONTROLS_GROUP_START + 13,   CONTROLS_GROUP_START + 26,   STR_SCREEN_EDGE_SCROLLING,              STR_SCREEN_EDGE_SCROLLING_TIP },            // Edge scrolling
    { WWT_CHECKBOX,         2,  10,     299,    CONTROLS_GROUP_START + 30,   CONTROLS_GROUP_START + 41,   STR_TRAP_MOUSE,                         STR_TRAP_MOUSE_TIP },                       // Trap mouse
    { WWT_CHECKBOX,         2,  10,     299,    CONTROLS_GROUP_START + 45,   CONTROLS_GROUP_START + 56,   STR_INVERT_RIGHT_MOUSE_DRAG,            STR_INVERT_RIGHT_MOUSE_DRAG_TIP },          // Invert right mouse dragging
    { WWT_CHECKBOX,         2,  10,     299,    CONTROLS_GROUP_START + 60,   CONTROLS_GROUP_START + 71,   STR_ZOOM_TO_CURSOR,                     STR_ZOOM_TO_CURSOR_TIP },                   // Zoom to cursor
    { WWT_BUTTON,           1,  155,    299,    CONTROLS_GROUP_START + 75,   CONTROLS_GROUP_START + 87,   STR_HOTKEY,                             STR_HOTKEY_TIP },                           // Set hotkeys buttons
#undef CONTROLS_GROUP_START
#define THEMES_GROUP_START 148
    { WWT_GROUPBOX,         1,  5,      304,    THEMES_GROUP_START + 0,      THEMES_GROUP_START + 47,     STR_THEMES_GROUP,                       STR_NONE },                                 // Toolbar buttons group
    { WWT_DROPDOWN,         1,  155,    299,    THEMES_GROUP_START + 14,     THEMES_GROUP_START + 25,     STR_STRING,                             STR_NONE },                                 // Themes
    { WWT_BUTTON,           1,  288,    298,    THEMES_GROUP_START + 15,     THEMES_GROUP_START + 24,     STR_DROPDOWN_GLYPH,                     STR_CURRENT_THEME_TIP },
    { WWT_BUTTON,           1,  155,    299,    THEMES_GROUP_START + 30,     THEMES_GROUP_START + 42,     STR_EDIT_THEMES_BUTTON,                 STR_EDIT_THEMES_BUTTON_TIP },               // Themes button
#undef THEMES_GROUP_START
#define TOOLBAR_GROUP_START 200
    { WWT_GROUPBOX,         1,  5,      304,    TOOLBAR_GROUP_START + 0,     TOOLBAR_GROUP_START + 75,    STR_TOOLBAR_BUTTONS_GROUP,              STR_NONE },                                 // Toolbar buttons group
    { WWT_CHECKBOX,         2,  24,     145,    TOOLBAR_GROUP_START + 31,    TOOLBAR_GROUP_START + 42,    STR_FINANCES_BUTTON_ON_TOOLBAR,         STR_FINANCES_BUTTON_ON_TOOLBAR_TIP },       // Finances
    { WWT_CHECKBOX,         2,  24,     145,    TOOLBAR_GROUP_START + 46,    TOOLBAR_GROUP_START + 57,    STR_RESEARCH_BUTTON_ON_TOOLBAR,         STR_RESEARCH_BUTTON_ON_TOOLBAR_TIP },       // Research
    { WWT_CHECKBOX,         2,  155,    299,    TOOLBAR_GROUP_START + 31,    TOOLBAR_GROUP_START + 42,    STR_CHEATS_BUTTON_ON_TOOLBAR,           STR_CHEATS_BUTTON_ON_TOOLBAR_TIP },         // Cheats
    { WWT_CHECKBOX,         2,  155,    299,    TOOLBAR_GROUP_START + 46,    TOOLBAR_GROUP_START + 57,    STR_SHOW_RECENT_MESSAGES_ON_TOOLBAR,    STR_SHOW_RECENT_MESSAGES_ON_TOOLBAR_TIP },  // Recent messages
    { WWT_CHECKBOX,         2,  24,     185,    TOOLBAR_GROUP_START + 61,    TOOLBAR_GROUP_START + 72,    STR_MUTE_BUTTON_ON_TOOLBAR,             STR_MUTE_BUTTON_ON_TOOLBAR_TIP },           // Mute
    { WWT_CHECKBOX,         2,  155,    299,    TOOLBAR_GROUP_START + 61,    TOOLBAR_GROUP_START + 72,    STR_CHAT_BUTTON_ON_TOOLBAR,             STR_CHAT_BUTTON_ON_TOOLBAR_TIP },           // Chat
    { WIDGETS_END },
#undef TOOLBAR_GROUP_START
};

static rct_widget window_options_misc_widgets[] = {
    MAIN_OPTIONS_WIDGETS,
#define TITLE_SEQUENCE_START 53
    { WWT_GROUPBOX,         1,  5,      304,    TITLE_SEQUENCE_START + 0,       TITLE_SEQUENCE_START + 64,      STR_OPTIONS_TITLE_SEQUENCE,       STR_NONE },
    { WWT_CHECKBOX,         2,  10,    299,    TITLE_SEQUENCE_START + 15,      TITLE_SEQUENCE_START + 30,  STR_OPTIONS_RANDOM_TITLE_SEQUENCE, STR_NONE},              //Random Title Sequence
    { WWT_DROPDOWN,         1,  135,    299,    TITLE_SEQUENCE_START + 32,      TITLE_SEQUENCE_START + 43,      STR_STRING,                       STR_NONE },                             // Title sequence dropdown
    { WWT_BUTTON,           1,  288,    298,    TITLE_SEQUENCE_START + 33,      TITLE_SEQUENCE_START + 44,      STR_DROPDOWN_GLYPH,               STR_TITLE_SEQUENCE_TIP },               // Title sequence dropdown button
    { WWT_BUTTON,           1,  135,    299,    TITLE_SEQUENCE_START + 48,      TITLE_SEQUENCE_START + 60,      STR_EDIT_TITLE_SEQUENCES_BUTTON,  STR_EDIT_TITLE_SEQUENCES_BUTTON_TIP },  // Edit title sequences button
#undef TITLE_SEQUENCE_START
#define SCENARIO_START 122
    { WWT_GROUPBOX,         1,  5,      304,    SCENARIO_START + 0,             SCENARIO_START + 50,            STR_OPTIONS_SCENARIO_SELECTION,   STR_NONE },
    { WWT_DROPDOWN,         1,  175,    299,    SCENARIO_START + 15,            SCENARIO_START + 26,            STR_NONE,                         STR_NONE },                             // Scenario select mode
    { WWT_BUTTON,           1,  288,    298,    SCENARIO_START + 16,            SCENARIO_START + 25,            STR_DROPDOWN_GLYPH,               STR_SCENARIO_GROUPING_TIP },
    { WWT_CHECKBOX,         2,  25,     299,    SCENARIO_START + 30,            SCENARIO_START + 45,            STR_OPTIONS_SCENARIO_UNLOCKING,   STR_SCENARIO_UNLOCKING_TIP },           // Unlocking of scenarios
#undef SCENARIO_START
#define SCENARIO_OPTIONS_START 177
    { WWT_GROUPBOX,         1,  5,      304,    SCENARIO_OPTIONS_START + 0,     SCENARIO_OPTIONS_START + 34,    STR_SCENARIO_OPTIONS,             STR_NONE },
    { WWT_CHECKBOX,         2,  10,     299,    SCENARIO_OPTIONS_START + 15,    SCENARIO_OPTIONS_START + 29,    STR_ALLOW_EARLY_COMPLETION,       STR_EARLY_COMPLETION_TIP },             // Allow early scenario completion
#undef SCENARIO_OPTIONS_START
#define TWEAKS_START 216
    { WWT_GROUPBOX,         1,  5,      304,    TWEAKS_START + 0,               TWEAKS_START + 80,              STR_OPTIONS_TWEAKS,               STR_NONE },
    { WWT_CHECKBOX,         2,  10,     299,    TWEAKS_START + 15,              TWEAKS_START + 29,              STR_REAL_NAME,                    STR_REAL_NAME_TIP },                    // Show 'real' names of guests
    { WWT_CHECKBOX,         2,  10,     299,    TWEAKS_START + 30,              TWEAKS_START + 44,              STR_AUTO_STAFF_PLACEMENT,         STR_AUTO_STAFF_PLACEMENT_TIP },         // Auto staff placement
    { WWT_CHECKBOX,         2,  10,     299,    TWEAKS_START + 45,              TWEAKS_START + 59,              STR_AUTO_OPEN_SHOPS,              STR_AUTO_OPEN_SHOPS_TIP },              // Automatically open shops & stalls
    { WWT_DROPDOWN,         1,  175,    299,    TWEAKS_START + 61,              TWEAKS_START + 72,              STR_NONE,                         STR_NONE },                             // Default inspection time dropdown
    { WWT_BUTTON,           1,  288,    298,    TWEAKS_START + 62,              TWEAKS_START + 71,              STR_DROPDOWN_GLYPH,               STR_DEFAULT_INSPECTION_INTERVAL_TIP },  // Default inspection time dropdown button
#undef TWEAKS_START
    { WIDGETS_END },
};

static rct_widget window_options_advanced_widgets[] = {
    MAIN_OPTIONS_WIDGETS,
    { WWT_CHECKBOX,         2,  10,     299,    54,      65,    STR_ENABLE_DEBUGGING_TOOLS,                 STR_ENABLE_DEBUGGING_TOOLS_TIP },                   // Enable debugging tools
    { WWT_CHECKBOX,         2,  10,     299,    69,      80,    STR_ALLOW_LOADING_WITH_INCORRECT_CHECKSUM,  STR_ALLOW_LOADING_WITH_INCORRECT_CHECKSUM_TIP },    // Allow loading with incorrect checksum
    { WWT_CHECKBOX,         2,  10,     299,    84,      95,    STR_SAVE_PLUGIN_DATA,                       STR_SAVE_PLUGIN_DATA_TIP },                         // Export plug-in objects with saved games
    { WWT_CHECKBOX,         2,  10,     299,    99,      110,   STR_STAY_CONNECTED_AFTER_DESYNC,            STR_STAY_CONNECTED_AFTER_DESYNC_TIP },              // Do not disconnect after the client desynchronises with the server
    { WWT_CHECKBOX,         1,  10,     299,    114,     125,   STR_ALWAYS_NATIVE_LOADSAVE,                 STR_ALWAYS_NATIVE_LOADSAVE_TIP },                   // Use native load/save window
    { WWT_DROPDOWN,         1,  165,    299,    130,     142,   STR_NONE,                                   STR_NONE },                                         // Autosave dropdown
    { WWT_BUTTON,           1,  288,    298,    131,     141,   STR_DROPDOWN_GLYPH,                         STR_AUTOSAVE_FREQUENCY_TIP },                       // Autosave dropdown button
    SPINNER_WIDGETS        (1,  165,    299,    150,     161,   STR_NONE,                                   STR_AUTOSAVE_AMOUNT_TIP ),                          // Autosave amount spinner
    { WWT_LABEL,            1,  23,     298,    169,     180,   STR_PATH_TO_RCT1,                           STR_PATH_TO_RCT1_TIP },                             // RCT 1 path text
    { WWT_BUTTON,           1,  24,     289,    184,     197,   STR_NONE,                                   STR_STRING_TOOLTIP },                               // RCT 1 path button
    { WWT_BUTTON,           1,  289,    299,    184,     197,   STR_CLOSE_X,                                STR_PATH_TO_RCT1_CLEAR_TIP },                       // RCT 1 path clear button
    { WIDGETS_END },
};

static rct_widget *window_options_page_widgets[] = {
    window_options_display_widgets,
    window_options_rendering_widgets,
    window_options_culture_widgets,
    window_options_audio_widgets,
    window_options_controls_and_interface_widgets,
    window_options_misc_widgets,
    window_options_advanced_widgets,
};

#pragma endregion

static constexpr const rct_string_id window_options_autosave_names[6] = {
    STR_SAVE_EVERY_MINUTE,
    STR_SAVE_EVERY_5MINUTES,
    STR_SAVE_EVERY_15MINUTES,
    STR_SAVE_EVERY_30MINUTES,
    STR_SAVE_EVERY_HOUR,
    STR_SAVE_NEVER,
};

static constexpr const rct_string_id window_options_title_music_names[] = {
    STR_OPTIONS_MUSIC_VALUE_NONE ,
    STR_ROLLERCOASTER_TYCOON_1_DROPDOWN ,
    STR_ROLLERCOASTER_TYCOON_2_DROPDOWN ,
    STR_OPTIONS_MUSIC_VALUE_RANDOM,
};

static constexpr const rct_string_id window_options_scale_quality_names[] = {
    STR_SCALING_QUALITY_LINEAR,
    STR_SCALING_QUALITY_SMOOTH_NN
};

static constexpr const rct_string_id window_options_fullscreen_mode_names[] = {
    STR_OPTIONS_DISPLAY_WINDOWED,
    STR_OPTIONS_DISPLAY_FULLSCREEN,
    STR_OPTIONS_DISPLAY_FULLSCREEN_BORDERLESS,
};

const int32_t window_options_tab_animation_divisor[] =
{
    4, // WINDOW_OPTIONS_PAGE_DISPLAY,
    1, // WINDOW_OPTIONS_PAGE_RENDERING,
    8, // WINDOW_OPTIONS_PAGE_CULTURE,
    2, // WINDOW_OPTIONS_PAGE_AUDIO,
    2, // WINDOW_OPTIONS_PAGE_CONTROLS_AND_INTERFACE,
    4, // WINDOW_OPTIONS_PAGE_MISC,
    2, // WINDOW_OPTIONS_PAGE_ADVANCED,
};
const int32_t window_options_tab_animation_frames[] =
{
     8, // WINDOW_OPTIONS_PAGE_DISPLAY,
     1, // WINDOW_OPTIONS_PAGE_RENDERING,
     8, // WINDOW_OPTIONS_PAGE_CULTURE,
    16, // WINDOW_OPTIONS_PAGE_AUDIO,
     4, // WINDOW_OPTIONS_PAGE_CONTROLS_AND_INTERFACE,
    16, // WINDOW_OPTIONS_PAGE_MISC,
    16, // WINDOW_OPTIONS_PAGE_ADVANCED,
};

static void window_options_set_page(rct_window *w, int32_t page);
static void window_options_set_pressed_tab(rct_window *w);
static void window_options_draw_tab_image(rct_drawpixelinfo *dpi, rct_window *w, int32_t page, int32_t spriteIndex);
static void window_options_draw_tab_images(rct_drawpixelinfo *dpi, rct_window *w);
static void window_options_show_dropdown(rct_window *w, rct_widget *widget, int32_t num_items);
static void window_options_update_height_markers();

#pragma region Events

static void window_options_mouseup(rct_window *w, rct_widgetindex widgetIndex);
static void window_options_mousedown(rct_window *w, rct_widgetindex widgetIndex, rct_widget* widget);
static void window_options_dropdown(rct_window *w, rct_widgetindex widgetIndex, int32_t dropdownIndex);
static void window_options_update(rct_window *w);
static void window_options_invalidate(rct_window *w);
static void window_options_paint(rct_window *w, rct_drawpixelinfo *dpi);
static void window_options_scrollgetsize(rct_window *w, int32_t scrollIndex, int32_t *width, int32_t *height);
static void window_options_tooltip(rct_window *w, rct_widgetindex widgetIndex, rct_string_id *stringid);

static rct_window_event_list window_options_events = {
    nullptr,
    window_options_mouseup,
    nullptr,
    window_options_mousedown,
    window_options_dropdown,
    nullptr,
    window_options_update,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    window_options_scrollgetsize,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    window_options_tooltip,
    nullptr,
    nullptr,
    window_options_invalidate,
    window_options_paint,
    nullptr
};

#pragma endregion

#pragma region Enabled Widgets

#define MAIN_OPTIONS_ENABLED_WIDGETS \
    (1 << WIDX_CLOSE) | \
    (1 << WIDX_TAB_1) | \
    (1 << WIDX_TAB_2) | \
    (1 << WIDX_TAB_3) | \
    (1 << WIDX_TAB_4) | \
    (1 << WIDX_TAB_5) | \
    (1 << WIDX_TAB_6) | \
    (1 << WIDX_TAB_7)

static uint64_t window_options_page_enabled_widgets[] = {
    MAIN_OPTIONS_ENABLED_WIDGETS |
    (1 << WIDX_RESOLUTION) |
    (1 << WIDX_RESOLUTION_DROPDOWN) |
    (1 << WIDX_FULLSCREEN) |
    (1 << WIDX_FULLSCREEN_DROPDOWN) |
    (1 << WIDX_DRAWING_ENGINE) |
    (1 << WIDX_DRAWING_ENGINE_DROPDOWN) |
    (1 << WIDX_UNCAP_FPS_CHECKBOX) |
    (1 << WIDX_USE_VSYNC_CHECKBOX) |
    (1 << WIDX_SHOW_FPS_CHECKBOX) |
    (1 << WIDX_MULTITHREADING_CHECKBOX) |
    (1 << WIDX_MINIMIZE_FOCUS_LOSS) |
    (1 << WIDX_STEAM_OVERLAY_PAUSE) |
    (1 << WIDX_SCALE) |
    (1 << WIDX_SCALE_UP) |
    (1 << WIDX_SCALE_DOWN) |
    (1 << WIDX_SCALE_QUALITY) |
    (1 << WIDX_SCALE_QUALITY_DROPDOWN),

    MAIN_OPTIONS_ENABLED_WIDGETS |
    (1 << WIDX_TILE_SMOOTHING_CHECKBOX) |
    (1 << WIDX_GRIDLINES_CHECKBOX) |
    (1 << WIDX_UPPER_CASE_BANNERS_CHECKBOX) |
    (1 << WIDX_SHOW_GUEST_PURCHASES_CHECKBOX) |
    (1 << WIDX_TRANSPARENT_SCREENSHOTS_CHECKBOX) |
    (1 << WIDX_VIRTUAL_FLOOR) |
    (1 << WIDX_VIRTUAL_FLOOR_DROPDOWN) |
    (1 << WIDX_DAY_NIGHT_CHECKBOX) |
    (1 << WIDX_ENABLE_LIGHT_FX_CHECKBOX) |
    (1 << WIDX_ENABLE_LIGHT_FX_FOR_VEHICLES_CHECKBOX) |
    (1 << WIDX_RENDER_WEATHER_EFFECTS_CHECKBOX) |
    (1 << WIDX_DISABLE_LIGHTNING_EFFECT_CHECKBOX),

    MAIN_OPTIONS_ENABLED_WIDGETS |
    (1 << WIDX_LANGUAGE) |
    (1 << WIDX_LANGUAGE_DROPDOWN) |
    (1 << WIDX_CURRENCY) |
    (1 << WIDX_CURRENCY_DROPDOWN) |
    (1 << WIDX_DISTANCE) |
    (1 << WIDX_DISTANCE_DROPDOWN) |
    (1 << WIDX_TEMPERATURE) |
    (1 << WIDX_TEMPERATURE_DROPDOWN) |
    (1 << WIDX_HEIGHT_LABELS) |
    (1 << WIDX_HEIGHT_LABELS_DROPDOWN) |
    (1 << WIDX_DATE_FORMAT) |
    (1 << WIDX_DATE_FORMAT_DROPDOWN),

    MAIN_OPTIONS_ENABLED_WIDGETS |
    (1 << WIDX_SOUND) |
    (1 << WIDX_SOUND_DROPDOWN) |
    (1 << WIDX_MASTER_SOUND_CHECKBOX) |
    (1 << WIDX_SOUND_CHECKBOX) |
    (1 << WIDX_MUSIC_CHECKBOX) |
    (1 << WIDX_AUDIO_FOCUS_CHECKBOX) |
    (1 << WIDX_TITLE_MUSIC) |
    (1 << WIDX_TITLE_MUSIC_DROPDOWN),

    MAIN_OPTIONS_ENABLED_WIDGETS |
    (1 << WIDX_SCREEN_EDGE_SCROLLING) |
    (1 << WIDX_TRAP_CURSOR) |
    (1 << WIDX_INVERT_DRAG) |
    (1 << WIDX_ZOOM_TO_CURSOR) |
    (1 << WIDX_HOTKEY_DROPDOWN) |
    (1 << WIDX_TOOLBAR_SHOW_FINANCES) |
    (1 << WIDX_TOOLBAR_SHOW_RESEARCH) |
    (1 << WIDX_TOOLBAR_SHOW_CHEATS) |
    (1 << WIDX_TOOLBAR_SHOW_NEWS) |
    (1 << WIDX_THEMES) |
    (1 << WIDX_THEMES_DROPDOWN) |
    (1 << WIDX_THEMES_BUTTON) |
    (1 << WIDX_TOOLBAR_SHOW_MUTE) |
    (1 << WIDX_TOOLBAR_SHOW_CHAT),

    MAIN_OPTIONS_ENABLED_WIDGETS |
    (1 << WIDX_REAL_NAME_CHECKBOX) |
    (1 << WIDX_AUTO_STAFF_PLACEMENT) |
    (1 << WIDX_TITLE_SEQUENCE) |
    (1 << WIDX_TITLE_SEQUENCE_DROPDOWN) |
    (1 << WIDX_TITLE_SEQUENCE_BUTTON) |
    (1 << WIDX_TITLE_SEQUENCE_RANDOM) |
    (1 << WIDX_SCENARIO_GROUPING) |
    (1 << WIDX_SCENARIO_GROUPING_DROPDOWN) |
    (1 << WIDX_SCENARIO_UNLOCKING) |
    (1 << WIDX_ALLOW_EARLY_COMPLETION) |
    (1 << WIDX_AUTO_OPEN_SHOPS) |
    (1 << WIDX_DEFAULT_INSPECTION_INTERVAL) |
    (1 << WIDX_DEFAULT_INSPECTION_INTERVAL_DROPDOWN),

    MAIN_OPTIONS_ENABLED_WIDGETS |
    (1 << WIDX_DEBUGGING_TOOLS) |
    (1 << WIDX_ALLOW_LOADING_WITH_INCORRECT_CHECKSUM) |
    (1 << WIDX_SAVE_PLUGIN_DATA_CHECKBOX) |
    (1 << WIDX_STAY_CONNECTED_AFTER_DESYNC) |
    (1 << WIDX_ALWAYS_NATIVE_LOADSAVE) |
    (1 << WIDX_AUTOSAVE) |
    (1 << WIDX_AUTOSAVE_DROPDOWN) |
    (1 << WIDX_AUTOSAVE_AMOUNT) |
    (1 << WIDX_AUTOSAVE_AMOUNT_UP) |
    (1 << WIDX_AUTOSAVE_AMOUNT_DOWN) |
    (1 << WIDX_PATH_TO_RCT1_TEXT) |
    (1 << WIDX_PATH_TO_RCT1_BUTTON) |
    (1 << WIDX_PATH_TO_RCT1_CLEAR),
};
// clang-format on

#pragma endregion

/**
 *
 *  rct2: 0x006BAC5B
 */
rct_window* window_options_open()
{
    rct_window* w;

    // Check if window is already open
    w = window_bring_to_front_by_class(WC_OPTIONS);
    if (w != nullptr)
        return w;

    w = window_create_centred(WW, WH, &window_options_events, WC_OPTIONS, 0);
    w->widgets = window_options_display_widgets;
    w->enabled_widgets = window_options_page_enabled_widgets[WINDOW_OPTIONS_PAGE_DISPLAY];
    w->page = WINDOW_OPTIONS_PAGE_DISPLAY;
    w->frame_no = 0;
    window_init_scroll_widgets(w);

    return w;
}

/**
 *
 *  rct2: 0x006BAFCA
 */
static void window_options_mouseup(rct_window* w, rct_widgetindex widgetIndex)
{
    switch (widgetIndex)
    {
        case WIDX_CLOSE:
            window_close(w);
            return;
        case WIDX_TAB_1:
        case WIDX_TAB_2:
        case WIDX_TAB_3:
        case WIDX_TAB_4:
        case WIDX_TAB_5:
        case WIDX_TAB_6:
        case WIDX_TAB_7:
            window_options_set_page(w, widgetIndex - WIDX_TAB_1);
            break;
    }

    switch (w->page)
    {
        case WINDOW_OPTIONS_PAGE_DISPLAY:
            switch (widgetIndex)
            {
                case WIDX_UNCAP_FPS_CHECKBOX:
                    gConfigGeneral.uncap_fps ^= 1;
                    drawing_engine_set_vsync(gConfigGeneral.use_vsync);
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_USE_VSYNC_CHECKBOX:
                    gConfigGeneral.use_vsync ^= 1;
                    drawing_engine_set_vsync(gConfigGeneral.use_vsync);
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_SHOW_FPS_CHECKBOX:
                    gConfigGeneral.show_fps ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_MULTITHREADING_CHECKBOX:
                    gConfigGeneral.multithreading ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_MINIMIZE_FOCUS_LOSS:
                    gConfigGeneral.minimize_fullscreen_focus_loss ^= 1;
                    platform_refresh_video(false);
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_STEAM_OVERLAY_PAUSE:
                    gConfigGeneral.steam_overlay_pause ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_RENDERING:
            switch (widgetIndex)
            {
                case WIDX_TILE_SMOOTHING_CHECKBOX:
                    gConfigGeneral.landscape_smoothing ^= 1;
                    config_save_default();
                    gfx_invalidate_screen();
                    break;
                case WIDX_GRIDLINES_CHECKBOX:
                    gConfigGeneral.always_show_gridlines ^= 1;
                    config_save_default();
                    gfx_invalidate_screen();
                    if ((w = window_get_main()) != nullptr)
                    {
                        if (gConfigGeneral.always_show_gridlines)
                            w->viewport->flags |= VIEWPORT_FLAG_GRIDLINES;
                        else
                            w->viewport->flags &= ~VIEWPORT_FLAG_GRIDLINES;
                    }
                    break;
                case WIDX_DAY_NIGHT_CHECKBOX:
                    gConfigGeneral.day_night_cycle ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_ENABLE_LIGHT_FX_CHECKBOX:
                    gConfigGeneral.enable_light_fx ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_ENABLE_LIGHT_FX_FOR_VEHICLES_CHECKBOX:
                    gConfigGeneral.enable_light_fx_for_vehicles ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_UPPER_CASE_BANNERS_CHECKBOX:
                    gConfigGeneral.upper_case_banners ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_DISABLE_LIGHTNING_EFFECT_CHECKBOX:
                    gConfigGeneral.disable_lightning_effect ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_RENDER_WEATHER_EFFECTS_CHECKBOX:
                    gConfigGeneral.render_weather_effects ^= 1;
                    gConfigGeneral.render_weather_gloom = gConfigGeneral.render_weather_effects;
                    config_save_default();
                    w->Invalidate();
                    gfx_invalidate_screen();
                    break;
                case WIDX_SHOW_GUEST_PURCHASES_CHECKBOX:
                    gConfigGeneral.show_guest_purchases ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_TRANSPARENT_SCREENSHOTS_CHECKBOX:
                    gConfigGeneral.transparent_screenshot ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_CULTURE:
            break;

        case WINDOW_OPTIONS_PAGE_AUDIO:
            switch (widgetIndex)
            {
                case WIDX_SOUND_CHECKBOX:
                    gConfigSound.sound_enabled = !gConfigSound.sound_enabled;
                    config_save_default();
                    w->Invalidate();
                    break;

                case WIDX_MASTER_SOUND_CHECKBOX:
                    gConfigSound.master_sound_enabled = !gConfigSound.master_sound_enabled;
                    if (!gConfigSound.master_sound_enabled)
                        audio_pause_sounds();
                    else
                        audio_unpause_sounds();
                    window_invalidate_by_class(WC_TOP_TOOLBAR);
                    config_save_default();
                    w->Invalidate();
                    break;

                case WIDX_MUSIC_CHECKBOX:
                    gConfigSound.ride_music_enabled = !gConfigSound.ride_music_enabled;
                    if (!gConfigSound.ride_music_enabled)
                    {
                        audio_stop_ride_music();
                    }
                    config_save_default();
                    w->Invalidate();
                    break;

                case WIDX_AUDIO_FOCUS_CHECKBOX:
                    gConfigSound.audio_focus = !gConfigSound.audio_focus;
                    config_save_default();
                    w->Invalidate();
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_CONTROLS_AND_INTERFACE:
            switch (widgetIndex)
            {
                case WIDX_HOTKEY_DROPDOWN:
                    context_open_window(WC_KEYBOARD_SHORTCUT_LIST);
                    break;
                case WIDX_SCREEN_EDGE_SCROLLING:
                    gConfigGeneral.edge_scrolling ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_TRAP_CURSOR:
                    gConfigGeneral.trap_cursor ^= 1;
                    config_save_default();
                    context_set_cursor_trap(gConfigGeneral.trap_cursor);
                    w->Invalidate();
                    break;
                case WIDX_ZOOM_TO_CURSOR:
                    gConfigGeneral.zoom_to_cursor ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_TOOLBAR_SHOW_FINANCES:
                    gConfigInterface.toolbar_show_finances ^= 1;
                    config_save_default();
                    w->Invalidate();
                    window_invalidate_by_class(WC_TOP_TOOLBAR);
                    break;
                case WIDX_TOOLBAR_SHOW_RESEARCH:
                    gConfigInterface.toolbar_show_research ^= 1;
                    config_save_default();
                    w->Invalidate();
                    window_invalidate_by_class(WC_TOP_TOOLBAR);
                    break;
                case WIDX_TOOLBAR_SHOW_CHEATS:
                    gConfigInterface.toolbar_show_cheats ^= 1;
                    config_save_default();
                    w->Invalidate();
                    window_invalidate_by_class(WC_TOP_TOOLBAR);
                    break;
                case WIDX_TOOLBAR_SHOW_NEWS:
                    gConfigInterface.toolbar_show_news ^= 1;
                    config_save_default();
                    w->Invalidate();
                    window_invalidate_by_class(WC_TOP_TOOLBAR);
                    break;
                case WIDX_TOOLBAR_SHOW_MUTE:
                    gConfigInterface.toolbar_show_mute ^= 1;
                    config_save_default();
                    w->Invalidate();
                    window_invalidate_by_class(WC_TOP_TOOLBAR);
                    break;
                case WIDX_TOOLBAR_SHOW_CHAT:
                    gConfigInterface.toolbar_show_chat ^= 1;
                    config_save_default();
                    w->Invalidate();
                    window_invalidate_by_class(WC_TOP_TOOLBAR);
                    break;
                case WIDX_INVERT_DRAG:
                    gConfigGeneral.invert_viewport_drag ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_THEMES_BUTTON:
                    context_open_window(WC_THEMES);
                    w->Invalidate();
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_MISC:
            switch (widgetIndex)
            {
                case WIDX_REAL_NAME_CHECKBOX:
                    gConfigGeneral.show_real_names_of_guests ^= 1;
                    config_save_default();
                    w->Invalidate();
                    peep_update_names(gConfigGeneral.show_real_names_of_guests);
                    break;
                case WIDX_AUTO_STAFF_PLACEMENT:
                    gConfigGeneral.auto_staff_placement ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_TITLE_SEQUENCE_BUTTON:
                    window_title_editor_open(0);
                    break;
                case WIDX_SCENARIO_UNLOCKING:
                    gConfigGeneral.scenario_unlocking_enabled ^= 1;
                    config_save_default();
                    window_close_by_class(WC_SCENARIO_SELECT);
                    break;
                case WIDX_TITLE_SEQUENCE_RANDOM:
                    gConfigInterface.random_title_sequence ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_AUTO_OPEN_SHOPS:
                    gConfigGeneral.auto_open_shops = !gConfigGeneral.auto_open_shops;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_ALLOW_EARLY_COMPLETION:
                    gConfigGeneral.allow_early_completion ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_ADVANCED:
            switch (widgetIndex)
            {
                case WIDX_DEBUGGING_TOOLS:
                    gConfigGeneral.debugging_tools ^= 1;
                    config_save_default();
                    gfx_invalidate_screen();
                    break;
                case WIDX_ALLOW_LOADING_WITH_INCORRECT_CHECKSUM:
                    gConfigGeneral.allow_loading_with_incorrect_checksum = !gConfigGeneral
                                                                                .allow_loading_with_incorrect_checksum;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_SAVE_PLUGIN_DATA_CHECKBOX:
                    gConfigGeneral.save_plugin_data ^= 1;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_STAY_CONNECTED_AFTER_DESYNC:
                    gConfigNetwork.stay_connected = !gConfigNetwork.stay_connected;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_ALWAYS_NATIVE_LOADSAVE:
                    gConfigGeneral.use_native_browse_dialog = !gConfigGeneral.use_native_browse_dialog;
                    config_save_default();
                    w->Invalidate();
                    break;
                case WIDX_PATH_TO_RCT1_BUTTON:
                {
                    utf8string rct1path = platform_open_directory_browser(language_get_string(STR_PATH_TO_RCT1_BROWSER));
                    if (rct1path)
                    {
                        // Check if this directory actually contains RCT1
                        if (Csg1datPresentAtLocation(rct1path))
                        {
                            if (Csg1idatPresentAtLocation(rct1path))
                            {
                                if (CsgAtLocationIsUsable(rct1path))
                                {
                                    SafeFree(gConfigGeneral.rct1_path);
                                    gConfigGeneral.rct1_path = rct1path;
                                    gConfigInterface.scenarioselect_last_tab = 0;
                                    config_save_default();
                                    context_show_error(STR_RESTART_REQUIRED, STR_NONE);
                                }
                                else
                                {
                                    SafeFree(rct1path);
                                    context_show_error(STR_PATH_TO_RCT1_IS_WRONG_VERSION, STR_NONE);
                                }
                            }
                            else
                            {
                                SafeFree(rct1path);
                                context_show_error(STR_PATH_TO_RCT1_DOES_NOT_CONTAIN_CSG1I_DAT, STR_NONE);
                            }
                        }
                        else
                        {
                            SafeFree(rct1path);
                            context_show_error(STR_PATH_TO_RCT1_WRONG_ERROR, STR_NONE);
                        }
                    }
                    w->Invalidate();
                    break;
                }
                case WIDX_PATH_TO_RCT1_CLEAR:
                    if (!str_is_null_or_empty(gConfigGeneral.rct1_path))
                    {
                        SafeFree(gConfigGeneral.rct1_path);
                        config_save_default();
                    }
                    w->Invalidate();
                    break;
            }
            break;
    }
}

/**
 *
 *  rct2: 0x006BB01B
 */
static void window_options_mousedown(rct_window* w, rct_widgetindex widgetIndex, rct_widget* widget)
{
    uint32_t num_items;

    widget = &w->widgets[widgetIndex - 1];

    switch (w->page)
    {
        case WINDOW_OPTIONS_PAGE_DISPLAY:
            switch (widgetIndex)
            {
                case WIDX_RESOLUTION_DROPDOWN:
                {
                    const auto& resolutions = OpenRCT2::GetContext()->GetUiContext()->GetFullscreenResolutions();

                    int32_t selectedResolution = -1;
                    for (size_t i = 0; i < resolutions.size(); i++)
                    {
                        const Resolution& resolution = resolutions[i];

                        gDropdownItemsFormat[i] = STR_DROPDOWN_MENU_LABEL;

                        uint16_t* args = reinterpret_cast<uint16_t*>(&gDropdownItemsArgs[i]);
                        args[0] = STR_RESOLUTION_X_BY_Y;
                        args[1] = resolution.Width;
                        args[2] = resolution.Height;

                        if (resolution.Width == gConfigGeneral.fullscreen_width
                            && resolution.Height == gConfigGeneral.fullscreen_height)
                        {
                            selectedResolution = static_cast<int32_t>(i);
                        }
                    }

                    window_options_show_dropdown(w, widget, static_cast<int32_t>(resolutions.size()));

                    if (selectedResolution != -1 && selectedResolution < 32)
                    {
                        dropdown_set_checked(selectedResolution, true);
                    }
                }

                break;
                case WIDX_FULLSCREEN_DROPDOWN:
                    gDropdownItemsFormat[0] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsFormat[1] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsFormat[2] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsArgs[0] = STR_OPTIONS_DISPLAY_WINDOWED;
                    gDropdownItemsArgs[1] = STR_OPTIONS_DISPLAY_FULLSCREEN;
                    gDropdownItemsArgs[2] = STR_OPTIONS_DISPLAY_FULLSCREEN_BORDERLESS;

                    window_options_show_dropdown(w, widget, 3);

                    dropdown_set_checked(gConfigGeneral.fullscreen_mode, true);
                    break;
                case WIDX_DRAWING_ENGINE_DROPDOWN:
                {
                    int32_t numItems = 3;
#ifdef DISABLE_OPENGL
                    numItems = 2;
#endif

                    for (int32_t i = 0; i < numItems; i++)
                    {
                        gDropdownItemsFormat[i] = STR_DROPDOWN_MENU_LABEL;
                        gDropdownItemsArgs[i] = DrawingEngineStringIds[i];
                    }
                    window_options_show_dropdown(w, widget, numItems);
                    dropdown_set_checked(gConfigGeneral.drawing_engine, true);
                    break;
                }
                case WIDX_SCALE_UP:
                    gConfigGeneral.window_scale += 0.25f;
                    config_save_default();
                    gfx_invalidate_screen();
                    context_trigger_resize();
                    context_update_cursor_scale();
                    break;
                case WIDX_SCALE_DOWN:
                    gConfigGeneral.window_scale -= 0.25f;
                    gConfigGeneral.window_scale = std::max(0.5f, gConfigGeneral.window_scale);
                    config_save_default();
                    gfx_invalidate_screen();
                    context_trigger_resize();
                    context_update_cursor_scale();
                    break;
                case WIDX_SCALE_QUALITY_DROPDOWN:
                    gDropdownItemsFormat[0] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsFormat[1] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsArgs[0] = STR_SCALING_QUALITY_LINEAR;
                    gDropdownItemsArgs[1] = STR_SCALING_QUALITY_SMOOTH_NN;

                    window_options_show_dropdown(w, widget, 2);

                    // Note: offset by one to compensate for lack of NN option.
                    dropdown_set_checked(gConfigGeneral.scale_quality - 1, true);
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_RENDERING:
            switch (widgetIndex)
            {
                case WIDX_VIRTUAL_FLOOR_DROPDOWN:
                    gDropdownItemsFormat[0] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsFormat[1] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsFormat[2] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsArgs[0] = STR_VIRTUAL_FLOOR_STYLE_DISABLED;
                    gDropdownItemsArgs[1] = STR_VIRTUAL_FLOOR_STYLE_TRANSPARENT;
                    gDropdownItemsArgs[2] = STR_VIRTUAL_FLOOR_STYLE_GLASSY;

                    window_options_show_dropdown(w, widget, 3);

                    dropdown_set_checked(gConfigGeneral.virtual_floor_style, true);
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_CULTURE:
            switch (widgetIndex)
            {
                case WIDX_HEIGHT_LABELS_DROPDOWN:
                    gDropdownItemsFormat[0] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsFormat[1] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsArgs[0] = STR_HEIGHT_IN_UNITS;
                    gDropdownItemsArgs[1] = STR_REAL_VALUES;

                    window_options_show_dropdown(w, widget, 2);

                    dropdown_set_checked(gConfigGeneral.show_height_as_units ? 0 : 1, true);
                    break;
                case WIDX_CURRENCY_DROPDOWN:
                {
                    num_items = CURRENCY_END + 1;                      // All the currencies plus the separator
                    size_t num_ordinary_currencies = CURRENCY_END - 1; // All the currencies except custom currency

                    for (size_t i = 0; i < num_ordinary_currencies; i++)
                    {
                        gDropdownItemsFormat[i] = STR_DROPDOWN_MENU_LABEL;
                        gDropdownItemsArgs[i] = CurrencyDescriptors[i].stringId;
                    }

                    gDropdownItemsFormat[num_ordinary_currencies] = DROPDOWN_SEPARATOR;

                    gDropdownItemsFormat[num_ordinary_currencies + 1] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsArgs[num_ordinary_currencies + 1] = CurrencyDescriptors[CURRENCY_CUSTOM].stringId;

                    window_options_show_dropdown(w, widget, num_items);

                    if (gConfigGeneral.currency_format == CURRENCY_CUSTOM)
                    {
                        dropdown_set_checked(gConfigGeneral.currency_format + 1, true);
                    }
                    else
                    {
                        dropdown_set_checked(gConfigGeneral.currency_format, true);
                    }
                    break;
                }
                case WIDX_DISTANCE_DROPDOWN:
                    gDropdownItemsFormat[0] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsFormat[1] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsFormat[2] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsArgs[0] = STR_IMPERIAL;
                    gDropdownItemsArgs[1] = STR_METRIC;
                    gDropdownItemsArgs[2] = STR_SI;

                    window_options_show_dropdown(w, widget, 3);

                    dropdown_set_checked(gConfigGeneral.measurement_format, true);
                    break;
                case WIDX_TEMPERATURE_DROPDOWN:
                    gDropdownItemsFormat[0] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsFormat[1] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsArgs[0] = STR_CELSIUS;
                    gDropdownItemsArgs[1] = STR_FAHRENHEIT;

                    window_options_show_dropdown(w, widget, 2);

                    dropdown_set_checked(gConfigGeneral.temperature_format, true);
                    break;
                case WIDX_LANGUAGE_DROPDOWN:
                    for (size_t i = 1; i < LANGUAGE_COUNT; i++)
                    {
                        gDropdownItemsFormat[i - 1] = STR_OPTIONS_DROPDOWN_ITEM;
                        gDropdownItemsArgs[i - 1] = reinterpret_cast<uintptr_t>(LanguagesDescriptors[i].native_name);
                    }
                    window_options_show_dropdown(w, widget, LANGUAGE_COUNT - 1);
                    dropdown_set_checked(LocalisationService_GetCurrentLanguage() - 1, true);
                    break;
                case WIDX_DATE_FORMAT_DROPDOWN:
                    for (size_t i = 0; i < 4; i++)
                    {
                        gDropdownItemsFormat[i] = STR_DROPDOWN_MENU_LABEL;
                        gDropdownItemsArgs[i] = DateFormatStringIds[i];
                    }
                    window_options_show_dropdown(w, widget, 4);
                    dropdown_set_checked(gConfigGeneral.date_format, true);
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_AUDIO:
            switch (widgetIndex)
            {
                case WIDX_SOUND_DROPDOWN:
                    audio_populate_devices();

                    // populate the list with the sound devices
                    for (size_t i = 0; static_cast<int32_t>(i) < gAudioDeviceCount; i++)
                    {
                        gDropdownItemsFormat[i] = STR_OPTIONS_DROPDOWN_ITEM;
                        gDropdownItemsArgs[i] = reinterpret_cast<uintptr_t>(gAudioDevices[i].name);
                    }

                    window_options_show_dropdown(w, widget, gAudioDeviceCount);

                    dropdown_set_checked(gAudioCurrentDevice, true);
                    break;
                case WIDX_TITLE_MUSIC_DROPDOWN:
                    num_items = 4;

                    for (size_t i = 0; i < num_items; i++)
                    {
                        gDropdownItemsFormat[i] = STR_DROPDOWN_MENU_LABEL;
                        gDropdownItemsArgs[i] = window_options_title_music_names[i];
                    }

                    window_options_show_dropdown(w, widget, num_items);

                    dropdown_set_checked(gConfigSound.title_music, true);
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_CONTROLS_AND_INTERFACE:
            switch (widgetIndex)
            {
                case WIDX_THEMES_DROPDOWN:
                    num_items = static_cast<uint32_t>(theme_manager_get_num_available_themes());

                    for (size_t i = 0; i < num_items; i++)
                    {
                        gDropdownItemsFormat[i] = STR_OPTIONS_DROPDOWN_ITEM;
                        gDropdownItemsArgs[i] = reinterpret_cast<uintptr_t>(theme_manager_get_available_theme_name(i));
                    }

                    window_dropdown_show_text_custom_width(
                        { w->windowPos.x + widget->left, w->windowPos.y + widget->top }, widget->bottom - widget->top + 1,
                        w->colours[1], 0, DROPDOWN_FLAG_STAY_OPEN, num_items, widget->right - widget->left - 3);

                    dropdown_set_checked(static_cast<int32_t>(theme_manager_get_active_available_theme_index()), true);
                    widget_invalidate(w, WIDX_THEMES_DROPDOWN);
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_MISC:
            switch (widgetIndex)
            {
                case WIDX_TITLE_SEQUENCE_DROPDOWN:
                    num_items = static_cast<int32_t>(title_sequence_manager_get_count());
                    for (size_t i = 0; i < num_items; i++)
                    {
                        gDropdownItemsFormat[i] = STR_OPTIONS_DROPDOWN_ITEM;
                        gDropdownItemsArgs[i] = reinterpret_cast<uintptr_t>(title_sequence_manager_get_name(i));
                    }

                    window_dropdown_show_text(
                        { w->windowPos.x + widget->left, w->windowPos.y + widget->top }, widget->bottom - widget->top + 1,
                        w->colours[1], DROPDOWN_FLAG_STAY_OPEN, num_items);

                    dropdown_set_checked(static_cast<int32_t>(title_get_current_sequence()), true);
                    break;
                case WIDX_SCENARIO_GROUPING_DROPDOWN:
                    num_items = 2;

                    gDropdownItemsFormat[0] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsArgs[0] = STR_OPTIONS_SCENARIO_DIFFICULTY;
                    gDropdownItemsFormat[1] = STR_DROPDOWN_MENU_LABEL;
                    gDropdownItemsArgs[1] = STR_OPTIONS_SCENARIO_ORIGIN;

                    window_dropdown_show_text_custom_width(
                        { w->windowPos.x + widget->left, w->windowPos.y + widget->top }, widget->bottom - widget->top + 1,
                        w->colours[1], 0, DROPDOWN_FLAG_STAY_OPEN, num_items, widget->right - widget->left - 3);

                    dropdown_set_checked(gConfigGeneral.scenario_select_mode, true);
                    break;
                case WIDX_DEFAULT_INSPECTION_INTERVAL_DROPDOWN:
                    for (size_t i = 0; i < 7; i++)
                    {
                        gDropdownItemsFormat[i] = STR_DROPDOWN_MENU_LABEL;
                        gDropdownItemsArgs[i] = RideInspectionIntervalNames[i];
                    }

                    window_options_show_dropdown(w, widget, 7);
                    dropdown_set_checked(gConfigGeneral.default_inspection_interval, true);
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_ADVANCED:
            switch (widgetIndex)
            {
                case WIDX_AUTOSAVE_DROPDOWN:
                    for (size_t i = AUTOSAVE_EVERY_MINUTE; i <= AUTOSAVE_NEVER; i++)
                    {
                        gDropdownItemsFormat[i] = STR_DROPDOWN_MENU_LABEL;
                        gDropdownItemsArgs[i] = window_options_autosave_names[i];
                    }

                    window_options_show_dropdown(w, widget, AUTOSAVE_NEVER + 1);
                    dropdown_set_checked(gConfigGeneral.autosave_frequency, true);
                    break;
                case WIDX_AUTOSAVE_AMOUNT_UP:
                    gConfigGeneral.autosave_amount += 1;
                    config_save_default();
                    widget_invalidate(w, WIDX_AUTOSAVE);
                    widget_invalidate(w, WIDX_AUTOSAVE_DROPDOWN);
                    widget_invalidate(w, WIDX_AUTOSAVE_AMOUNT);
                    break;
                case WIDX_AUTOSAVE_AMOUNT_DOWN:
                    if (gConfigGeneral.autosave_amount > 1)
                    {
                        gConfigGeneral.autosave_amount -= 1;
                        config_save_default();
                        widget_invalidate(w, WIDX_AUTOSAVE);
                        widget_invalidate(w, WIDX_AUTOSAVE_DROPDOWN);
                        widget_invalidate(w, WIDX_AUTOSAVE_AMOUNT);
                    }
            }
            break;
    }
}

/**
 *
 *  rct2: 0x006BB076
 */
static void window_options_dropdown(rct_window* w, rct_widgetindex widgetIndex, int32_t dropdownIndex)
{
    if (dropdownIndex == -1)
        return;

    switch (w->page)
    {
        case WINDOW_OPTIONS_PAGE_DISPLAY:
            switch (widgetIndex)
            {
                case WIDX_RESOLUTION_DROPDOWN:
                {
                    const auto& resolutions = OpenRCT2::GetContext()->GetUiContext()->GetFullscreenResolutions();

                    const Resolution& resolution = resolutions[dropdownIndex];
                    if (resolution.Width != gConfigGeneral.fullscreen_width
                        || resolution.Height != gConfigGeneral.fullscreen_height)
                    {
                        gConfigGeneral.fullscreen_width = resolution.Width;
                        gConfigGeneral.fullscreen_height = resolution.Height;

                        if (gConfigGeneral.fullscreen_mode == static_cast<int32_t>(OpenRCT2::Ui::FULLSCREEN_MODE::FULLSCREEN))
                            context_set_fullscreen_mode(static_cast<int32_t>(OpenRCT2::Ui::FULLSCREEN_MODE::FULLSCREEN));

                        config_save_default();
                        gfx_invalidate_screen();
                    }
                }
                break;
                case WIDX_FULLSCREEN_DROPDOWN:
                    if (dropdownIndex != gConfigGeneral.fullscreen_mode)
                    {
                        context_set_fullscreen_mode(dropdownIndex);

                        gConfigGeneral.fullscreen_mode = static_cast<uint8_t>(dropdownIndex);
                        config_save_default();
                        gfx_invalidate_screen();
                    }
                    break;
                case WIDX_DRAWING_ENGINE_DROPDOWN:
                    if (dropdownIndex != gConfigGeneral.drawing_engine)
                    {
                        int32_t srcEngine = drawing_engine_get_type();
                        int32_t dstEngine = dropdownIndex;

                        gConfigGeneral.drawing_engine = static_cast<uint8_t>(dstEngine);
                        bool recreate_window = drawing_engine_requires_new_window(srcEngine, dstEngine);
                        platform_refresh_video(recreate_window);
                        config_save_default();
                        w->Invalidate();
                    }
                    break;
                case WIDX_SCALE_QUALITY_DROPDOWN:
                    // Note: offset by one to compensate for lack of NN option.
                    if ((dropdownIndex + 1) != gConfigGeneral.scale_quality)
                    {
                        gConfigGeneral.scale_quality = static_cast<uint8_t>(dropdownIndex) + 1;
                        config_save_default();
                        gfx_invalidate_screen();
                        context_trigger_resize();
                    }
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_RENDERING:
            switch (widgetIndex)
            {
                case WIDX_VIRTUAL_FLOOR_DROPDOWN:
                    gConfigGeneral.virtual_floor_style = dropdownIndex;
                    config_save_default();
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_CULTURE:
            switch (widgetIndex)
            {
                case WIDX_HEIGHT_LABELS_DROPDOWN:
                    // reset flag and set it to 1 if height as units is selected
                    gConfigGeneral.show_height_as_units = 0;

                    if (dropdownIndex == 0)
                    {
                        gConfigGeneral.show_height_as_units = 1;
                    }
                    config_save_default();
                    window_options_update_height_markers();
                    break;
                case WIDX_CURRENCY_DROPDOWN:
                    if (dropdownIndex == CURRENCY_CUSTOM + 1)
                    { // Add 1 because the separator occupies a position
                        gConfigGeneral.currency_format = static_cast<int8_t>(dropdownIndex) - 1;
                        context_open_window(WC_CUSTOM_CURRENCY_CONFIG);
                    }
                    else
                    {
                        gConfigGeneral.currency_format = static_cast<int8_t>(dropdownIndex);
                    }
                    config_save_default();
                    gfx_invalidate_screen();
                    break;
                case WIDX_DISTANCE_DROPDOWN:
                    gConfigGeneral.measurement_format = static_cast<int8_t>(dropdownIndex);
                    config_save_default();
                    window_options_update_height_markers();
                    break;
                case WIDX_TEMPERATURE_DROPDOWN:
                    if (dropdownIndex != gConfigGeneral.temperature_format)
                    {
                        gConfigGeneral.temperature_format = static_cast<int8_t>(dropdownIndex);
                        config_save_default();
                        gfx_invalidate_screen();
                    }
                    break;
                case WIDX_LANGUAGE_DROPDOWN:
                {
                    auto fallbackLanguage = LocalisationService_GetCurrentLanguage();
                    if (dropdownIndex != LocalisationService_GetCurrentLanguage() - 1)
                    {
                        if (!language_open(dropdownIndex + 1))
                        {
                            // Failed to open language file, try to recover by falling
                            // back to previously used language
                            if (language_open(fallbackLanguage))
                            {
                                // It worked, so we can say it with error message in-game
                                context_show_error(STR_LANGUAGE_LOAD_FAILED, STR_NONE);
                            }
                            // report error to console regardless
                            log_error("Failed to open language file.");
                            dropdownIndex = fallbackLanguage - 1;
                        }
                        else
                        {
                            gConfigGeneral.language = dropdownIndex + 1;
                            config_save_default();
                            gfx_invalidate_screen();
                        }
                    }
                }
                break;
                case WIDX_DATE_FORMAT_DROPDOWN:
                    if (dropdownIndex != gConfigGeneral.date_format)
                    {
                        gConfigGeneral.date_format = static_cast<uint8_t>(dropdownIndex);
                        config_save_default();
                        gfx_invalidate_screen();
                    }
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_AUDIO:
            switch (widgetIndex)
            {
                case WIDX_SOUND_DROPDOWN:
                    audio_init_ride_sounds(dropdownIndex);
                    if (dropdownIndex < gAudioDeviceCount)
                    {
                        if (dropdownIndex == 0)
                        {
                            Mixer_Init(nullptr);
                            gConfigSound.device = nullptr;
                        }
                        else
                        {
                            char* devicename = gAudioDevices[dropdownIndex].name;
                            Mixer_Init(devicename);
                            SafeFree(gConfigSound.device);
                            gConfigSound.device = strndup(devicename, AUDIO_DEVICE_NAME_SIZE);
                        }
                        config_save_default();
                        audio_start_title_music();
                    }
                    w->Invalidate();
                    break;
                case WIDX_TITLE_MUSIC_DROPDOWN:
                    if ((dropdownIndex == 1 || dropdownIndex == 3)
                        && !platform_file_exists(context_get_path_legacy(PATH_ID_CSS50)))
                    {
                        context_show_error(STR_OPTIONS_MUSIC_ERR_CSS50_NOT_FOUND, STR_OPTIONS_MUSIC_ERR_CSS50_NOT_FOUND_HINT);
                    }
                    else
                    {
                        gConfigSound.title_music = static_cast<int8_t>(dropdownIndex);
                        config_save_default();
                        w->Invalidate();
                    }

                    audio_stop_title_music();
                    if (dropdownIndex != 0)
                        audio_start_title_music();
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_CONTROLS_AND_INTERFACE:
            switch (widgetIndex)
            {
                case WIDX_THEMES_DROPDOWN:
                    if (dropdownIndex != -1)
                    {
                        theme_manager_set_active_available_theme(dropdownIndex);
                    }
                    config_save_default();
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_MISC:
            switch (widgetIndex)
            {
                case WIDX_TITLE_SEQUENCE_DROPDOWN:
                    if (dropdownIndex != static_cast<int32_t>(title_get_current_sequence()))
                    {
                        title_sequence_change_preset(static_cast<size_t>(dropdownIndex));
                        config_save_default();
                        w->Invalidate();
                    }
                    break;
                case WIDX_DEFAULT_INSPECTION_INTERVAL_DROPDOWN:
                    if (dropdownIndex != gConfigGeneral.default_inspection_interval)
                    {
                        gConfigGeneral.default_inspection_interval = static_cast<uint8_t>(dropdownIndex);
                        config_save_default();
                        w->Invalidate();
                    }
                    break;
                case WIDX_SCENARIO_GROUPING_DROPDOWN:
                    if (dropdownIndex != gConfigGeneral.scenario_select_mode)
                    {
                        gConfigGeneral.scenario_select_mode = dropdownIndex;
                        gConfigInterface.scenarioselect_last_tab = 0;
                        config_save_default();
                        w->Invalidate();
                        window_close_by_class(WC_SCENARIO_SELECT);
                    }
                    break;
            }
            break;

        case WINDOW_OPTIONS_PAGE_ADVANCED:
            switch (widgetIndex)
            {
                case WIDX_AUTOSAVE_DROPDOWN:
                    if (dropdownIndex != gConfigGeneral.autosave_frequency)
                    {
                        gConfigGeneral.autosave_frequency = static_cast<uint8_t>(dropdownIndex);
                        config_save_default();
                        w->Invalidate();
                    }
                    break;
            }
            break;
    }
}

static void initialize_scroll_position(rct_window* w, rct_widgetindex widget_index, int32_t scroll_id, uint8_t volume)
{
    rct_widget* widget = &window_options_audio_widgets[widget_index];
    rct_scroll* scroll = &w->scrolls[scroll_id];

    int widget_size = scroll->h_right - (widget->right - widget->left - 1);
    scroll->h_left = ceil(volume / 100.0f * widget_size);

    widget_scroll_update_thumbs(w, widget_index);
}

/**
 *
 *  rct2: 0x006BAD48
 */
static void window_options_invalidate(rct_window* w)
{
    rct_widget* widget;

    if (window_options_page_widgets[w->page] != w->widgets)
    {
        w->widgets = window_options_page_widgets[w->page];
        window_init_scroll_widgets(w);
    }
    window_options_set_pressed_tab(w);

    w->disabled_widgets = 0;

    switch (w->page)
    {
        case WINDOW_OPTIONS_PAGE_DISPLAY:
        {
            // Resolution dropdown caption.
            auto ft = Formatter::Common();
            ft.Increment(16);
            ft.Add<uint16_t>(static_cast<uint16_t>(gConfigGeneral.fullscreen_width));
            ft.Add<uint16_t>(static_cast<uint16_t>(gConfigGeneral.fullscreen_height));

            // Disable resolution dropdown on "Windowed" and "Fullscreen (desktop)"
            if (gConfigGeneral.fullscreen_mode != static_cast<int32_t>(OpenRCT2::Ui::FULLSCREEN_MODE::FULLSCREEN))
            {
                w->disabled_widgets |= (1 << WIDX_RESOLUTION_DROPDOWN);
                w->disabled_widgets |= (1 << WIDX_RESOLUTION);
            }
            else
            {
                w->disabled_widgets &= ~(1 << WIDX_RESOLUTION_DROPDOWN);
                w->disabled_widgets &= ~(1 << WIDX_RESOLUTION);
            }

            // Disable Steam Overlay checkbox when using software rendering.
            if (gConfigGeneral.drawing_engine == DRAWING_ENGINE_SOFTWARE)
            {
                w->disabled_widgets |= (1 << WIDX_STEAM_OVERLAY_PAUSE);
            }
            else
            {
                w->disabled_widgets &= ~(1 << WIDX_STEAM_OVERLAY_PAUSE);
            }

            // Disable scaling quality dropdown when using software rendering or when using an integer scalar.
            // In the latter case, nearest neighbour rendering will be used to scale.
            if (gConfigGeneral.drawing_engine == DRAWING_ENGINE_SOFTWARE
                || gConfigGeneral.window_scale == std::floor(gConfigGeneral.window_scale))
            {
                w->disabled_widgets |= (1 << WIDX_SCALE_QUALITY);
                w->disabled_widgets |= (1 << WIDX_SCALE_QUALITY_DROPDOWN);
            }
            else
            {
                w->disabled_widgets &= ~(1 << WIDX_SCALE_QUALITY);
                w->disabled_widgets &= ~(1 << WIDX_SCALE_QUALITY_DROPDOWN);
            }

            // Disable changing VSync for Software engine, as we can't control its use of VSync
            if (gConfigGeneral.drawing_engine == DRAWING_ENGINE_SOFTWARE)
            {
                w->disabled_widgets |= (1 << WIDX_USE_VSYNC_CHECKBOX);
            }
            else
            {
                w->disabled_widgets &= ~(1 << WIDX_USE_VSYNC_CHECKBOX);
            }

            widget_set_checkbox_value(w, WIDX_UNCAP_FPS_CHECKBOX, gConfigGeneral.uncap_fps);
            widget_set_checkbox_value(w, WIDX_USE_VSYNC_CHECKBOX, gConfigGeneral.use_vsync);
            widget_set_checkbox_value(w, WIDX_SHOW_FPS_CHECKBOX, gConfigGeneral.show_fps);
            widget_set_checkbox_value(w, WIDX_MULTITHREADING_CHECKBOX, gConfigGeneral.multithreading);
            widget_set_checkbox_value(w, WIDX_MINIMIZE_FOCUS_LOSS, gConfigGeneral.minimize_fullscreen_focus_loss);
            widget_set_checkbox_value(w, WIDX_STEAM_OVERLAY_PAUSE, gConfigGeneral.steam_overlay_pause);

            // Dropdown captions for straightforward strings.
            window_options_display_widgets[WIDX_FULLSCREEN].text = window_options_fullscreen_mode_names[gConfigGeneral
                                                                                                            .fullscreen_mode];
            window_options_display_widgets[WIDX_DRAWING_ENGINE].text = DrawingEngineStringIds[gConfigGeneral.drawing_engine];
            window_options_display_widgets[WIDX_SCALE_QUALITY].text = window_options_scale_quality_names
                [gConfigGeneral.scale_quality - 1];

            break;
        }

        case WINDOW_OPTIONS_PAGE_RENDERING:
        {
            widget_set_checkbox_value(w, WIDX_TILE_SMOOTHING_CHECKBOX, gConfigGeneral.landscape_smoothing);
            widget_set_checkbox_value(w, WIDX_GRIDLINES_CHECKBOX, gConfigGeneral.always_show_gridlines);
            widget_set_checkbox_value(w, WIDX_DAY_NIGHT_CHECKBOX, gConfigGeneral.day_night_cycle);
            widget_set_checkbox_value(w, WIDX_SHOW_GUEST_PURCHASES_CHECKBOX, gConfigGeneral.show_guest_purchases);
            widget_set_checkbox_value(w, WIDX_TRANSPARENT_SCREENSHOTS_CHECKBOX, gConfigGeneral.transparent_screenshot);
            widget_set_checkbox_value(w, WIDX_UPPER_CASE_BANNERS_CHECKBOX, gConfigGeneral.upper_case_banners);

            rct_string_id VirtualFloorStyleStrings[] = { STR_VIRTUAL_FLOOR_STYLE_DISABLED, STR_VIRTUAL_FLOOR_STYLE_TRANSPARENT,
                                                         STR_VIRTUAL_FLOOR_STYLE_GLASSY };

            window_options_rendering_widgets[WIDX_VIRTUAL_FLOOR].text = VirtualFloorStyleStrings[gConfigGeneral
                                                                                                     .virtual_floor_style];

            widget_set_checkbox_value(w, WIDX_ENABLE_LIGHT_FX_CHECKBOX, gConfigGeneral.enable_light_fx);
            if (gConfigGeneral.day_night_cycle
                && gConfigGeneral.drawing_engine == DRAWING_ENGINE_SOFTWARE_WITH_HARDWARE_DISPLAY)
            {
                w->disabled_widgets &= ~(1 << WIDX_ENABLE_LIGHT_FX_CHECKBOX);
            }
            else
            {
                w->disabled_widgets |= (1 << WIDX_ENABLE_LIGHT_FX_CHECKBOX);
            }

            widget_set_checkbox_value(
                w, WIDX_ENABLE_LIGHT_FX_FOR_VEHICLES_CHECKBOX, gConfigGeneral.enable_light_fx_for_vehicles);
            if (gConfigGeneral.day_night_cycle && gConfigGeneral.drawing_engine == DRAWING_ENGINE_SOFTWARE_WITH_HARDWARE_DISPLAY
                && gConfigGeneral.enable_light_fx)
            {
                w->disabled_widgets &= ~(1 << WIDX_ENABLE_LIGHT_FX_FOR_VEHICLES_CHECKBOX);
            }
            else
            {
                w->disabled_widgets |= (1 << WIDX_ENABLE_LIGHT_FX_FOR_VEHICLES_CHECKBOX);
            }

            widget_set_checkbox_value(
                w, WIDX_RENDER_WEATHER_EFFECTS_CHECKBOX,
                gConfigGeneral.render_weather_effects || gConfigGeneral.render_weather_gloom);
            widget_set_checkbox_value(w, WIDX_DISABLE_LIGHTNING_EFFECT_CHECKBOX, gConfigGeneral.disable_lightning_effect);
            if (!gConfigGeneral.render_weather_effects && !gConfigGeneral.render_weather_gloom)
            {
                widget_set_checkbox_value(w, WIDX_DISABLE_LIGHTNING_EFFECT_CHECKBOX, true);
                w->enabled_widgets &= ~(1 << WIDX_DISABLE_LIGHTNING_EFFECT_CHECKBOX);
                w->disabled_widgets |= (1 << WIDX_DISABLE_LIGHTNING_EFFECT_CHECKBOX);
            }
            else
            {
                w->enabled_widgets |= (1 << WIDX_DISABLE_LIGHTNING_EFFECT_CHECKBOX);
                w->disabled_widgets &= ~(1 << WIDX_DISABLE_LIGHTNING_EFFECT_CHECKBOX);
            }

            break;
        }

        case WINDOW_OPTIONS_PAGE_CULTURE:
        {
            // Language
            auto ft = Formatter::Common();
            ft.Add<char*>(LanguagesDescriptors[LocalisationService_GetCurrentLanguage()].native_name);

            // Currency: pounds, dollars, etc. (10 total)
            window_options_culture_widgets[WIDX_CURRENCY].text = CurrencyDescriptors[gConfigGeneral.currency_format].stringId;

            // Distance: metric / imperial / si
            {
                rct_string_id stringId;
                switch (gConfigGeneral.measurement_format)
                {
                    default:
                    case MEASUREMENT_FORMAT_IMPERIAL:
                        stringId = STR_IMPERIAL;
                        break;
                    case MEASUREMENT_FORMAT_METRIC:
                        stringId = STR_METRIC;
                        break;
                    case MEASUREMENT_FORMAT_SI:
                        stringId = STR_SI;
                        break;
                }
                window_options_culture_widgets[WIDX_DISTANCE].text = stringId;
            }

            // Date format
            window_options_culture_widgets[WIDX_DATE_FORMAT].text = DateFormatStringIds[gConfigGeneral.date_format];

            // Temperature: celsius/fahrenheit
            window_options_culture_widgets[WIDX_TEMPERATURE].text = gConfigGeneral.temperature_format == TEMPERATURE_FORMAT_F
                ? STR_FAHRENHEIT
                : STR_CELSIUS;

            // Height: units/real values
            window_options_culture_widgets[WIDX_HEIGHT_LABELS].text = gConfigGeneral.show_height_as_units ? STR_HEIGHT_IN_UNITS
                                                                                                          : STR_REAL_VALUES;

            break;
        }
        case WINDOW_OPTIONS_PAGE_AUDIO:
        {
            // Sound device
            rct_string_id audioDeviceStringId = STR_OPTIONS_SOUND_VALUE_DEFAULT;
            const char* audioDeviceName = nullptr;
            if (gAudioCurrentDevice == -1)
            {
                audioDeviceStringId = STR_SOUND_NONE;
            }
            else
            {
                audioDeviceStringId = STR_STRING;
#ifndef __linux__
                if (gAudioCurrentDevice == 0)
                {
                    audioDeviceStringId = STR_OPTIONS_SOUND_VALUE_DEFAULT;
                }
#endif // __linux__
                if (audioDeviceStringId == STR_STRING)
                {
                    audioDeviceName = gAudioDevices[gAudioCurrentDevice].name;
                }
            }

            window_options_audio_widgets[WIDX_SOUND].text = audioDeviceStringId;
            auto ft = Formatter::Common();
            ft.Add<char*>(audioDeviceName);

            window_options_audio_widgets[WIDX_TITLE_MUSIC].text = window_options_title_music_names[gConfigSound.title_music];

            widget_set_checkbox_value(w, WIDX_SOUND_CHECKBOX, gConfigSound.sound_enabled);
            widget_set_checkbox_value(w, WIDX_MASTER_SOUND_CHECKBOX, gConfigSound.master_sound_enabled);
            widget_set_checkbox_value(w, WIDX_MUSIC_CHECKBOX, gConfigSound.ride_music_enabled);
            widget_set_checkbox_value(w, WIDX_AUDIO_FOCUS_CHECKBOX, gConfigSound.audio_focus);
            widget_set_enabled(w, WIDX_SOUND_CHECKBOX, gConfigSound.master_sound_enabled);
            widget_set_enabled(w, WIDX_MUSIC_CHECKBOX, gConfigSound.master_sound_enabled);

            // Initialize only on first frame, otherwise the scrollbars wont be able to be modified
            if (w->frame_no == 0)
            {
                initialize_scroll_position(w, WIDX_MASTER_VOLUME, 0, gConfigSound.master_volume);
                initialize_scroll_position(w, WIDX_SOUND_VOLUME, 1, gConfigSound.sound_volume);
                initialize_scroll_position(w, WIDX_MUSIC_VOLUME, 2, gConfigSound.ride_music_volume);
            }

            break;
        }

        case WINDOW_OPTIONS_PAGE_CONTROLS_AND_INTERFACE:
        {
            widget_set_checkbox_value(w, WIDX_SCREEN_EDGE_SCROLLING, gConfigGeneral.edge_scrolling);
            widget_set_checkbox_value(w, WIDX_TRAP_CURSOR, gConfigGeneral.trap_cursor);
            widget_set_checkbox_value(w, WIDX_INVERT_DRAG, gConfigGeneral.invert_viewport_drag);
            widget_set_checkbox_value(w, WIDX_ZOOM_TO_CURSOR, gConfigGeneral.zoom_to_cursor);
            widget_set_checkbox_value(w, WIDX_TOOLBAR_SHOW_FINANCES, gConfigInterface.toolbar_show_finances);
            widget_set_checkbox_value(w, WIDX_TOOLBAR_SHOW_RESEARCH, gConfigInterface.toolbar_show_research);
            widget_set_checkbox_value(w, WIDX_TOOLBAR_SHOW_CHEATS, gConfigInterface.toolbar_show_cheats);
            widget_set_checkbox_value(w, WIDX_TOOLBAR_SHOW_NEWS, gConfigInterface.toolbar_show_news);
            widget_set_checkbox_value(w, WIDX_TOOLBAR_SHOW_MUTE, gConfigInterface.toolbar_show_mute);
            widget_set_checkbox_value(w, WIDX_TOOLBAR_SHOW_CHAT, gConfigInterface.toolbar_show_chat);

            size_t activeAvailableThemeIndex = theme_manager_get_active_available_theme_index();
            const utf8* activeThemeName = theme_manager_get_available_theme_name(activeAvailableThemeIndex);
            auto ft = Formatter::Common();
            ft.Add<uintptr_t>(reinterpret_cast<uintptr_t>(activeThemeName));

            break;
        }

        case WINDOW_OPTIONS_PAGE_MISC:
        {
            const utf8* name = title_sequence_manager_get_name(title_get_config_sequence());
            auto ft = Formatter::Common();
            ft.Add<uintptr_t>(reinterpret_cast<uintptr_t>(name));

            // The real name setting of clients is fixed to that of the server
            // and the server cannot change the setting during gameplay to prevent desyncs
            if (network_get_mode() != NETWORK_MODE_NONE)
            {
                w->disabled_widgets |= (1ULL << WIDX_REAL_NAME_CHECKBOX);
                window_options_misc_widgets[WIDX_REAL_NAME_CHECKBOX].tooltip = STR_OPTION_DISABLED_DURING_NETWORK_PLAY;
            }

            widget_set_checkbox_value(w, WIDX_REAL_NAME_CHECKBOX, gConfigGeneral.show_real_names_of_guests);
            widget_set_checkbox_value(w, WIDX_AUTO_STAFF_PLACEMENT, gConfigGeneral.auto_staff_placement);
            widget_set_checkbox_value(w, WIDX_AUTO_OPEN_SHOPS, gConfigGeneral.auto_open_shops);
            widget_set_checkbox_value(w, WIDX_TITLE_SEQUENCE_RANDOM, gConfigInterface.random_title_sequence);
            widget_set_checkbox_value(w, WIDX_ALLOW_EARLY_COMPLETION, gConfigGeneral.allow_early_completion);

            // Disable title sequence dropdown if set to random
            if (gConfigInterface.random_title_sequence)
            {
                w->disabled_widgets |= (1 << WIDX_TITLE_SEQUENCE_DROPDOWN);
                w->disabled_widgets |= (1 << WIDX_TITLE_SEQUENCE);
            }
            else
            {
                w->disabled_widgets &= ~(1 << WIDX_TITLE_SEQUENCE_DROPDOWN);
                w->disabled_widgets &= ~(1 << WIDX_TITLE_SEQUENCE);
            }

            if (gConfigGeneral.scenario_select_mode == SCENARIO_SELECT_MODE_DIFFICULTY)
                window_options_misc_widgets[WIDX_SCENARIO_GROUPING].text = STR_OPTIONS_SCENARIO_DIFFICULTY;
            else
                window_options_misc_widgets[WIDX_SCENARIO_GROUPING].text = STR_OPTIONS_SCENARIO_ORIGIN;

            widget_set_checkbox_value(w, WIDX_SCENARIO_UNLOCKING, gConfigGeneral.scenario_unlocking_enabled);

            if (gConfigGeneral.scenario_select_mode == SCENARIO_SELECT_MODE_ORIGIN)
            {
                w->disabled_widgets &= ~(1ULL << WIDX_SCENARIO_UNLOCKING);
            }
            else
            {
                w->disabled_widgets |= (1ULL << WIDX_SCENARIO_UNLOCKING);
            }

            window_options_misc_widgets[WIDX_DEFAULT_INSPECTION_INTERVAL].text = RideInspectionIntervalNames
                [gConfigGeneral.default_inspection_interval];

            break;
        }

        case WINDOW_OPTIONS_PAGE_ADVANCED:
            widget_set_checkbox_value(w, WIDX_DEBUGGING_TOOLS, gConfigGeneral.debugging_tools);
            widget_set_checkbox_value(
                w, WIDX_ALLOW_LOADING_WITH_INCORRECT_CHECKSUM, gConfigGeneral.allow_loading_with_incorrect_checksum);
            widget_set_checkbox_value(w, WIDX_SAVE_PLUGIN_DATA_CHECKBOX, gConfigGeneral.save_plugin_data);
            widget_set_checkbox_value(w, WIDX_STAY_CONNECTED_AFTER_DESYNC, gConfigNetwork.stay_connected);
            widget_set_checkbox_value(w, WIDX_ALWAYS_NATIVE_LOADSAVE, gConfigGeneral.use_native_browse_dialog);
            break;
    }

    // Automatically adjust window height to fit widgets
    int32_t y = 0;
    for (widget = &w->widgets[WIDX_PAGE_START]; widget->type != WWT_LAST; widget++)
    {
        y = std::max<int32_t>(y, widget->bottom);
    }
    w->height = y + 6;
    w->widgets[WIDX_BACKGROUND].bottom = w->height - 1;
    w->widgets[WIDX_PAGE_BACKGROUND].bottom = w->height - 1;
}

static uint8_t get_scroll_percentage(rct_widget* widget, rct_scroll* scroll)
{
    uint8_t width = widget->right - widget->left - 1;
    return static_cast<float>(scroll->h_left) / (scroll->h_right - width) * 100;
}

static void window_options_update(rct_window* w)
{
    // Tab animation
    w->frame_no++;
    widget_invalidate(w, WIDX_TAB_1 + w->page);

    if (w->page == WINDOW_OPTIONS_PAGE_AUDIO)
    {
        rct_widget* widget;

        widget = &window_options_audio_widgets[WIDX_MASTER_VOLUME];
        uint8_t master_volume = get_scroll_percentage(widget, &w->scrolls[0]);
        if (master_volume != gConfigSound.master_volume)
        {
            gConfigSound.master_volume = master_volume;
            config_save_default();
            widget_invalidate(w, WIDX_MASTER_VOLUME);
        }

        widget = &window_options_audio_widgets[WIDX_SOUND_VOLUME];
        uint8_t sound_volume = get_scroll_percentage(widget, &w->scrolls[1]);
        if (sound_volume != gConfigSound.sound_volume)
        {
            gConfigSound.sound_volume = sound_volume;
            config_save_default();
            widget_invalidate(w, WIDX_SOUND_VOLUME);
        }

        widget = &window_options_audio_widgets[WIDX_MUSIC_VOLUME];
        uint8_t ride_music_volume = get_scroll_percentage(widget, &w->scrolls[2]);
        if (ride_music_volume != gConfigSound.ride_music_volume)
        {
            gConfigSound.ride_music_volume = ride_music_volume;
            config_save_default();
            widget_invalidate(w, WIDX_MUSIC_VOLUME);
        }
    }
}

/**
 *
 *  rct2: 0x006BAEB4
 */
static void window_options_paint(rct_window* w, rct_drawpixelinfo* dpi)
{
    window_draw_widgets(w, dpi);
    window_options_draw_tab_images(dpi, w);

    switch (w->page)
    {
        case WINDOW_OPTIONS_PAGE_DISPLAY:
        {
            gfx_draw_string_left(
                dpi, STR_FULLSCREEN_MODE, w, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 10, window_options_display_widgets[WIDX_FULLSCREEN].top + 1 });

            // Disable resolution dropdown on "Windowed" and "Fullscreen (desktop)"
            int32_t colour = w->colours[1];
            if (gConfigGeneral.fullscreen_mode != static_cast<int32_t>(OpenRCT2::Ui::FULLSCREEN_MODE::FULLSCREEN))
            {
                colour |= COLOUR_FLAG_INSET;
            }
            gfx_draw_string_left(
                dpi, STR_DISPLAY_RESOLUTION, w, colour,
                w->windowPos + ScreenCoordsXY{ 10 + 15, window_options_display_widgets[WIDX_RESOLUTION].top + 1 });

            gfx_draw_string_left(
                dpi, STR_UI_SCALING_DESC, w, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 10, window_options_display_widgets[WIDX_SCALE].top + 1 });
            gfx_draw_string_left(
                dpi, STR_DRAWING_ENGINE, w, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 10, window_options_display_widgets[WIDX_DRAWING_ENGINE].top + 1 });

            int32_t scale = static_cast<int32_t>(gConfigGeneral.window_scale * 100);
            gfx_draw_string_left(
                dpi, STR_WINDOW_OBJECTIVE_VALUE_RATING, &scale, w->colours[1],
                w->windowPos + ScreenCoordsXY{ w->widgets[WIDX_SCALE].left + 1, w->widgets[WIDX_SCALE].top + 1 });

            colour = w->colours[1];
            if (gConfigGeneral.drawing_engine == DRAWING_ENGINE_SOFTWARE
                || gConfigGeneral.window_scale == std::floor(gConfigGeneral.window_scale))
            {
                colour |= COLOUR_FLAG_INSET;
            }
            gfx_draw_string_left(
                dpi, STR_SCALING_QUALITY, w, colour,
                w->windowPos + ScreenCoordsXY{ 25, window_options_display_widgets[WIDX_SCALE_QUALITY].top + 1 });
            break;
        }

        case WINDOW_OPTIONS_PAGE_CULTURE:
            gfx_draw_string_left(
                dpi, STR_OPTIONS_LANGUAGE, w, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 10, window_options_culture_widgets[WIDX_LANGUAGE].top + 1 });
            gfx_draw_string_left(
                dpi, STR_CURRENCY, w, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 10, window_options_culture_widgets[WIDX_CURRENCY].top + 1 });
            gfx_draw_string_left(
                dpi, STR_DISTANCE_AND_SPEED, w, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 10, window_options_culture_widgets[WIDX_DISTANCE].top + 1 });
            gfx_draw_string_left(
                dpi, STR_TEMPERATURE, w, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 10, window_options_culture_widgets[WIDX_TEMPERATURE].top + 1 });
            gfx_draw_string_left(
                dpi, STR_HEIGHT_LABELS, w, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 10, window_options_culture_widgets[WIDX_HEIGHT_LABELS].top + 1 });
            gfx_draw_string_left(
                dpi, STR_DATE_FORMAT, w, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 10, window_options_culture_widgets[WIDX_DATE_FORMAT].top + 1 });
            break;

        case WINDOW_OPTIONS_PAGE_CONTROLS_AND_INTERFACE:
        {
            gfx_draw_string_left(
                dpi, STR_SHOW_TOOLBAR_BUTTONS_FOR, w, w->colours[1],
                w->windowPos
                    + ScreenCoordsXY{ 10, window_options_controls_and_interface_widgets[WIDX_TOOLBAR_BUTTONS_GROUP].top + 15 });
            gfx_draw_string_left(
                dpi, STR_THEMES_LABEL_CURRENT_THEME, nullptr, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 10, window_options_controls_and_interface_widgets[WIDX_THEMES].top + 1 });
            break;
        }

        case WINDOW_OPTIONS_PAGE_MISC:
        {
            gfx_draw_string_left(
                dpi, STR_TITLE_SEQUENCE, w, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 10, window_options_misc_widgets[WIDX_TITLE_SEQUENCE].top + 1 });
            gfx_draw_string_left(
                dpi, STR_OPTIONS_SCENARIO_GROUPING, nullptr, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 10, window_options_misc_widgets[WIDX_SCENARIO_GROUPING].top + 1 });
            gfx_draw_string_left(
                dpi, STR_DEFAULT_INSPECTION_INTERVAL, w, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 10, window_options_misc_widgets[WIDX_DEFAULT_INSPECTION_INTERVAL].top + 1 });
            break;
        }

        case WINDOW_OPTIONS_PAGE_ADVANCED:
        {
            gfx_draw_string_left(
                dpi, STR_OPTIONS_AUTOSAVE_FREQUENCY_LABEL, w, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 24, window_options_advanced_widgets[WIDX_AUTOSAVE].top + 1 });
            gfx_draw_string_left(
                dpi, window_options_autosave_names[gConfigGeneral.autosave_frequency], nullptr, w->colours[1],
                w->windowPos
                    + ScreenCoordsXY{ window_options_advanced_widgets[WIDX_AUTOSAVE].left + 1,
                                      window_options_advanced_widgets[WIDX_AUTOSAVE].top });
            gfx_draw_string_left(
                dpi, STR_AUTOSAVE_AMOUNT, w, w->colours[1],
                w->windowPos + ScreenCoordsXY{ 24, window_options_advanced_widgets[WIDX_AUTOSAVE_AMOUNT].top + 1 });
            int32_t autosavesToKeep = static_cast<int32_t>(gConfigGeneral.autosave_amount);
            gfx_draw_string_left(
                dpi, STR_WINDOW_OBJECTIVE_VALUE_GUEST_COUNT, &autosavesToKeep, w->colours[1],
                w->windowPos
                    + ScreenCoordsXY{ w->widgets[WIDX_AUTOSAVE_AMOUNT].left + 1, w->widgets[WIDX_AUTOSAVE_AMOUNT].top + 1 });

            auto ft = Formatter::Common();
            ft.Add<uintptr_t>(Platform::StrDecompToPrecomp(gConfigGeneral.rct1_path));

            rct_widget pathWidget = window_options_advanced_widgets[WIDX_PATH_TO_RCT1_BUTTON];

            // Apply vertical alignment if appropriate.
            int32_t widgetHeight = pathWidget.bottom - pathWidget.top;
            int32_t lineHeight = font_get_line_height(gCurrentFontSpriteBase);
            uint32_t padding = widgetHeight > lineHeight ? (widgetHeight - lineHeight) / 2 : 0;
            ScreenCoordsXY screenCoords = { w->windowPos.x + pathWidget.left + 1,
                                            w->windowPos.y + pathWidget.top + static_cast<int32_t>(padding) };
            gfx_draw_string_left_clipped(dpi, STR_STRING, gCommonFormatArgs, w->colours[1], screenCoords, 277);
            break;
        }
    }
}

// helper function, all dropdown boxes have similar properties
static void window_options_show_dropdown(rct_window* w, rct_widget* widget, int32_t num_items)
{
    window_dropdown_show_text_custom_width(
        { w->windowPos.x + widget->left, w->windowPos.y + widget->top }, widget->bottom - widget->top + 1, w->colours[1], 0,
        DROPDOWN_FLAG_STAY_OPEN, num_items, widget->right - widget->left - 3);
}

static void window_options_update_height_markers()
{
    config_save_default();
    gfx_invalidate_screen();
}

static void window_options_scrollgetsize(rct_window* w, int32_t scrollIndex, int32_t* width, int32_t* height)
{
    if (w->page == WINDOW_OPTIONS_PAGE_AUDIO)
    {
        *width = 500;
    }
}

static void window_options_tooltip(rct_window* w, rct_widgetindex widgetIndex, rct_string_id* stringid)
{
    if (w->page == WINDOW_OPTIONS_PAGE_ADVANCED && widgetIndex == WIDX_PATH_TO_RCT1_BUTTON)
    {
        if (str_is_null_or_empty(gConfigGeneral.rct1_path))
        {
            // No tooltip if the path is empty
            *stringid = STR_NONE;
        }
        else
        {
            auto ft = Formatter::Common();
            ft.Add<uintptr_t>(reinterpret_cast<uintptr_t>(gConfigGeneral.rct1_path));
        }
    }
}

#pragma region Common

static void window_options_set_page(rct_window* w, int32_t page)
{
    w->page = page;
    w->frame_no = 0;
    w->enabled_widgets = window_options_page_enabled_widgets[page];
    w->pressed_widgets = 0;
    w->widgets = window_options_page_widgets[page];

    w->Invalidate();
    window_event_resize_call(w);
    window_event_invalidate_call(w);
    window_init_scroll_widgets(w);
    w->Invalidate();
}

static void window_options_set_pressed_tab(rct_window* w)
{
    int32_t i;
    for (i = 0; i < WINDOW_OPTIONS_PAGE_COUNT; i++)
        w->pressed_widgets &= ~(1 << (WIDX_TAB_1 + i));
    w->pressed_widgets |= 1LL << (WIDX_TAB_1 + w->page);
}

static void window_options_draw_tab_image(rct_drawpixelinfo* dpi, rct_window* w, int32_t page, int32_t spriteIndex)
{
    rct_widgetindex widgetIndex = WIDX_TAB_1 + page;
    rct_widget* widget = &w->widgets[widgetIndex];

    auto screenCoords = w->windowPos + ScreenCoordsXY{ widget->left, widget->top };

    if (!(w->disabled_widgets & (1LL << widgetIndex)))
    {
        if (w->page == page)
        {
            int32_t frame = w->frame_no / window_options_tab_animation_divisor[w->page];
            spriteIndex += (frame % window_options_tab_animation_frames[w->page]);
        }

        // Draw normal, enabled sprite.
        gfx_draw_sprite(dpi, spriteIndex, screenCoords.x, screenCoords.y, 0);
    }
    else
    {
        // Get the window background colour
        uint8_t window_colour = NOT_TRANSLUCENT(w->colours[widget->colour]);

        // Draw greyed out (light border bottom right shadow)
        gfx_draw_sprite_solid(dpi, spriteIndex, screenCoords + ScreenCoordsXY{ 1, 1 }, ColourMapA[window_colour].lighter);

        // Draw greyed out (dark)
        gfx_draw_sprite_solid(dpi, spriteIndex, screenCoords, ColourMapA[window_colour].mid_light);
    }
}

static void window_options_draw_tab_images(rct_drawpixelinfo* dpi, rct_window* w)
{
    window_options_draw_tab_image(dpi, w, WINDOW_OPTIONS_PAGE_DISPLAY, SPR_TAB_PAINT_0);
    window_options_draw_tab_image(dpi, w, WINDOW_OPTIONS_PAGE_RENDERING, SPR_G2_TAB_TREE);
    window_options_draw_tab_image(dpi, w, WINDOW_OPTIONS_PAGE_CULTURE, SPR_TAB_TIMER_0);
    window_options_draw_tab_image(dpi, w, WINDOW_OPTIONS_PAGE_AUDIO, SPR_TAB_MUSIC_0);
    window_options_draw_tab_image(dpi, w, WINDOW_OPTIONS_PAGE_CONTROLS_AND_INTERFACE, SPR_TAB_GEARS_0);
    window_options_draw_tab_image(dpi, w, WINDOW_OPTIONS_PAGE_MISC, SPR_TAB_RIDE_0);
    window_options_draw_tab_image(dpi, w, WINDOW_OPTIONS_PAGE_ADVANCED, SPR_TAB_WRENCH_0);
}

#pragma endregion
