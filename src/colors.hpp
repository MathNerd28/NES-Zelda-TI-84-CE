#pragma once

#include <graphx.h>
#include <sys/lcd.h>

// The color that will be used in the transparent index (not actually used)
#define COLOR_TRANSPARENT gfx_RGBTo1555(0xFF, 0x00, 0x80)

// Defining the colors that the original NES used, may vary by source
// Will need to tweak later to calibrate for calculator's screen
// Colors sourced from http://www.romdetectives.com/Wiki/index.php?title=NES_Palette
// Duplicate blacks omited

#define COLOR_GRAY gfx_RGBTo1555(0x7C, 0x7C, 0x7C)
#define COLOR_DARKER_BLUE gfx_RGBTo1555(0x00, 0x00, 0xFC)
#define COLOR_DARK_BLUE gfx_RGBTo1555(0x00, 0x00, 0xBC)
#define COLOR_DARK_PURPLE gfx_RGBTo1555(0x44, 0x28, 0xBC)
#define COLOR_DARK_MAGENTA gfx_RGBTo1555(0x94, 0x00, 0x84)
#define COLOR_DARK_PINK gfx_RGBTo1555(0xA8, 0x00, 0x20)
#define COLOR_DARK_RED gfx_RGBTo1555(0xA8, 0x10, 0x00)
#define COLOR_DARK_BROWN gfx_RGBTo1555(0x88, 0x14, 0x00)
#define COLOR_DARKER_YELLOW_GREEN gfx_RGBTo1555(0x50, 0x30, 0x00)
#define COLOR_DARK_GREEN_1 gfx_RGBTo1555(0x00, 0x78, 0x00)
#define COLOR_DARK_GREEN_2 gfx_RGBTo1555(0x00, 0x68, 0x00)
#define COLOR_DARK_GREEN_3 gfx_RGBTo1555(0x00, 0x58, 0x00)
#define COLOR_DARK_TURQUOISE gfx_RGBTo1555(0x00, 0x40, 0x58)
#define COLOR_BLACK gfx_RGBTo1555(0x00, 0x00, 0x00)

#define COLOR_LIGHT_GRAY gfx_RGBTo1555(0xBC, 0xBC, 0xBC)
#define COLOR_BLUE_1 gfx_RGBTo1555(0x00, 0x78, 0xF8)
#define COLOR_BLUE_2 gfx_RGBTo1555(0x00, 0x58, 0xF8)
#define COLOR_PURPLE gfx_RGBTo1555(0x68, 0x44, 0xFC)
#define COLOR_MAGENTA gfx_RGBTo1555(0xD8, 0x00, 0xCC)
#define COLOR_PINK gfx_RGBTo1555(0xE4, 0x00, 0x58)
#define COLOR_RED gfx_RGBTo1555(0xF8, 0x38, 0x00)
#define COLOR_BROWN gfx_RGBTo1555(0xE4, 0x5C, 0x10)
#define COLOR_DARK_YELLOW_GREEN gfx_RGBTo1555(0xAC, 0x7C, 0x00)
#define COLOR_GREEN_1 gfx_RGBTo1555(0x00, 0xB8, 0x00)
#define COLOR_GREEN_2 gfx_RGBTo1555(0x00, 0xA8, 0x00)
#define COLOR_GREEN_3 gfx_RGBTo1555(0x00, 0xA8, 0x44)
#define COLOR_TURQUOISE gfx_RGBTo1555(0x00, 0x88, 0x88)

#define COLOR_DARK_WHITE gfx_RGBTo1555(0xF8, 0xF8, 0xF8)
#define COLOR_LIGHT_BLUE gfx_RGBTo1555(0x3C, 0xBC, 0xFC)
#define COLOR_LIGHT_INDIGO gfx_RGBTo1555(0x68, 0x88, 0xFC)
#define COLOR_LIGHT_PURPLE gfx_RGBTo1555(0x98, 0x78, 0xF8)
#define COLOR_LIGHT_MAGENTA gfx_RGBTo1555(0xF8, 0x78, 0xF8)
#define COLOR_LIGHT_PINK gfx_RGBTo1555(0xF8, 0x58, 0x98)
#define COLOR_LIGHT_RED gfx_RGBTo1555(0xF8, 0x78, 0x58)
#define COLOR_ORANGE gfx_RGBTo1555(0xFC, 0xA0, 0x44)
#define COLOR_YELLOW_GREEN gfx_RGBTo1555(0xF8, 0xB8, 0x00)
#define COLOR_LIGHT_GREEN_1 gfx_RGBTo1555(0xB8, 0xF8, 0x18)
#define COLOR_LIGHT_GREEN_2 gfx_RGBTo1555(0x58, 0xD8, 0x54)
#define COLOR_LIGHT_GREEN_BLUE gfx_RGBTo1555(0x58, 0xF8, 0x98)
#define COLOR_LIGHT_TURQUOISE gfx_RGBTo1555(0x00, 0xE8, 0xD8)
#define COLOR_DARK_GRAY gfx_RGBTo1555(0x78, 0x78, 0x78)

#define COLOR_WHITE gfx_RGBTo1555(0xFC, 0xFC, 0xFC)
#define COLOR_LIGHTER_BLUE gfx_RGBTo1555(0xA4, 0xE4, 0xFC)
#define COLOR_LIGHTER_INDIGO gfx_RGBTo1555(0xB8, 0xB8, 0xF8)
#define COLOR_LIGHTER_PURPLE gfx_RGBTo1555(0xD8, 0xB8, 0xF8)
#define COLOR_LIGHTER_MAGENTA gfx_RGBTo1555(0xF8, 0xB8, 0xF8)
#define COLOR_LIGHTER_PINK gfx_RGBTo1555(0xF8, 0xA4, 0xC0)
#define COLOR_LIGHTER_RED gfx_RGBTo1555(0xF0, 0xD0, 0xB0)
#define COLOR_LIGHT_ORANGE gfx_RGBTo1555(0xFC, 0xE0, 0xA8)
#define COLOR_LIGHT_YELLOW_GREEN gfx_RGBTo1555(0xF8, 0xD8, 0x78)
#define COLOR_LIGHTER_GREEN_1 gfx_RGBTo1555(0xD8, 0xF8, 0x78)
#define COLOR_LIGHTER_GREEN_2 gfx_RGBTo1555(0xB8, 0xF8, 0xB8)
#define COLOR_LIGHTER_GREEN_BLUE gfx_RGBTo1555(0xB8, 0xF8, 0xD8)
#define COLOR_LIGHTER_TURQUOISE gfx_RGBTo1555(0x00, 0xFC, 0xFC)
#define COLOR_LIGHTER_GRAY gfx_RGBTo1555(0xF8, 0xD8, 0xF8)
