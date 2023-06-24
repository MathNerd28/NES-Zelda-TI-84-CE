#pragma once

#include <graphx.h>

#define SIZEOF_NORMAL16 258
#define SIZEOF_RLET16 320
#define SIZEOF_COLOR sizeof(uint16_t)

#define LINK_COLOR_COUNT 3
#define LINK_PALETTE_OFFSET 1

#define BACKGROUND_COLOR_COUNT 4
#define BACKGROUND_PALETTE_OFFSET_1 4
#define BACKGROUND_PALETTE_OFFSET_2 8
#define BACKGROUND_PALETTE_OFFSET_3 12

#define MENU_COLOR_COUNT 8
#define MENU_PALETTE_OFFSET 12

#define MAX_BACKGROUND_TILE_COUNT 88
#define LINK_SPRITE_COUNT 20
#define LINK_PALETTE_COUNT 16
#define BACKGROUND_OVERWORLD_PALETTE_COUNT 3
#define BACKGROUND_OVERWORLD_TILE_COUNT MAX_BACKGROUND_TILE_COUNT

void initMenuPalette();

void initLinkSprites();

void setLinkPalette(uint8_t palette_index);
void setOverworldPalette(uint8_t palette_index);
