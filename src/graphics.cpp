#include "graphics.hpp"

#include "colors.hpp"
#include "defines.hpp"
#include "gfx/gfx.h"

#include <graphx.h>

const uint16_t menu_colors[] = {
    COLOR_BLACK, COLOR_DARK_GRAY, COLOR_WHITE, COLOR_RED,
    COLOR_BLUE_1, COLOR_ORANGE, COLOR_LIGHT_PINK, COLOR_LIGHT_GREEN_1};

void initMenuPalette() {
    gfx_SetPalette(&menu_colors[0], MENU_COLOR_COUNT * SIZEOF_COLOR,
                   MENU_PALETTE_OFFSET);
}

void initLinkSprites() {
    for (uint8_t i = 0; i < 20; i++) {
        zelda.link.sprites[i] = gfx_ConvertMallocRLETSprite(link_sprites_swapped_tiles[i]);
    }
}

const uint16_t link_colors[] = {
    /* 0 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 1 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 2 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 3 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 4 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 5 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 6 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 7 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 8 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 9 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 10 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 11 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 12 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 13 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 14 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
    /* 15 */ COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GREEN_2,
};

const uint16_t overworld_colors_swap[] = {
    /* 0 */ COLOR_BLACK,
    COLOR_BLUE_2,
    COLOR_LIGHT_ORANGE,
    COLOR_GREEN_2,
    /* 1 */ COLOR_BLACK,
    COLOR_BLUE_2,
    COLOR_GRAY,
    COLOR_WHITE,
};

const uint16_t overworld_colors_const[] = {
    COLOR_BLACK,
    COLOR_BLUE_2,
    COLOR_LIGHT_ORANGE,
    COLOR_BROWN,
};

// Swap color palettes

void setLinkPalette(uint8_t index) {
    gfx_SetPalette(&link_colors[index * LINK_COLOR_COUNT], LINK_COLOR_COUNT * SIZEOF_COLOR,
                   LINK_PALETTE_OFFSET);
    zelda.link.paletteIndex = index;
}

void setOverworldPalette(uint8_t index) {
    // Slot 1 is locked to brown
    gfx_SetPalette(overworld_colors_const, BACKGROUND_COLOR_COUNT * SIZEOF_COLOR,
                   BACKGROUND_PALETTE_OFFSET_1);
    // Slot 2 is dynamic
    gfx_SetPalette(&overworld_colors_swap[index * BACKGROUND_COLOR_COUNT],
                   BACKGROUND_COLOR_COUNT * SIZEOF_COLOR, BACKGROUND_PALETTE_OFFSET_2);
    zelda.background.paletteIndex = index;
    // Slot 3 is unused
}
