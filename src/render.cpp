#include "render.hpp"

#include "colors.hpp"
#include "defines.hpp"
#include "gfx/font.h"
#include "gfx/gfx.h"
#include "graphics.hpp"

#include <fontlibc.h>
#include <graphx.h>
#include <time.h>

static void renderBlackBuffers() {
    gfx_FillScreen(12);
    gfx_SwapDraw();
    gfx_FillScreen(12);
}

void renderInit() {
    if (!ZGfxComm_init() || !ZGfxOver_init()) {
        exit(1);
    }

    gfx_Begin();
    gfx_SetDrawScreen();
    gfx_SetTransparentColor(0);
    renderBlackBuffers();

    fontlib_SetFont(zelda_font, (fontlib_load_options_t)0);
    fontlib_SetColors(14, 12); // White on black
    fontlib_SetTransparency(true);

    initMenuPalette();
    initLinkSprites();
    setLinkPalette(0);
    setOverworldPalette(0);
}

void renderExit() {
    gfx_End();
}

static void renderTilemap(uint24_t x_offset, uint24_t y_offset) {
    setOverworldPalette(
        zelda.background.paletteMap[16 * zelda.background.screenNumY +
                                    zelda.background.screenNumX]);

    int24_t x_draw, y_draw;
    uint8_t x, x_reset, x_tile, y, y_tile;
    uint24_t y_next;

    x_reset = x_offset / BACKGROUND_TILE_SIZE;
    y = y_offset / BACKGROUND_TILE_SIZE;
    x_offset %= BACKGROUND_TILE_SIZE;
    y_offset %= BACKGROUND_TILE_SIZE;
    y_next = y * BACKGROUND_WIDTH;

    // If aligned to 16x16 grid, skip clipping
    // Always aligned to at least x or y
    bool noClipX = x_offset == 0;
    bool noClipY = y_offset == 0;
    if (noClipX && noClipY) {
        y_draw = BACKGROUND_Y;
        for (y_tile = 0; y_tile < BACKGROUND_HEIGHT_TILES; y_tile++) {
            x = x_reset;
            x_draw = BACKGROUND_X;
            for (x_tile = 0; x_tile < BACKGROUND_WIDTH_TILES; x_tile++) {
                gfx_Sprite_NoClip(zelda.background.tiles[zelda.background.tilemap[x + y_next]], x_draw, y_draw);
                x++;
                x_draw += BACKGROUND_TILE_SIZE;
            }
            y++;
            y_next += BACKGROUND_WIDTH;
            y_draw += BACKGROUND_TILE_SIZE;
        }
        return;
    } else if (noClipX) {
        // First row (clipped)
        y_draw = BACKGROUND_Y - y_offset;
        x_draw = BACKGROUND_X;
        x = x_reset;
        for (x_tile = 0; x_tile < BACKGROUND_WIDTH_TILES; x_tile++) {
            gfx_Sprite(zelda.background.tiles[zelda.background.tilemap[x + y_next]],
                       x_draw, y_draw);
            x++;
            x_draw += BACKGROUND_TILE_SIZE;
        }
        y++;
        y_draw += BACKGROUND_TILE_SIZE;
        y_next += BACKGROUND_WIDTH;
        // Middle rows (unclipped)
        for (y_tile = 0; y_tile < BACKGROUND_HEIGHT_TILES - 1; y_tile++) {
            x_draw = BACKGROUND_X;
            x = x_reset;
            for (x_tile = 0; x_tile < BACKGROUND_WIDTH_TILES; x_tile++) {
                gfx_Sprite_NoClip(zelda.background.tiles[zelda.background.tilemap[x + y_next]], x_draw, y_draw);
                x++;
                x_draw += BACKGROUND_TILE_SIZE;
            }
            y++;
            y_next += BACKGROUND_WIDTH;
            y_draw += BACKGROUND_TILE_SIZE;
        }
        // Last row (clipped)
        x_draw = BACKGROUND_X - x_offset;
        x = x_reset;
        for (x_tile = 0; x_tile < BACKGROUND_WIDTH_TILES; x_tile++) {
            gfx_Sprite(zelda.background.tiles[zelda.background.tilemap[x + y_next]],
                       x_draw, y_draw);
            x++;
            x_draw += BACKGROUND_TILE_SIZE;
        }
        return;
    } else if (noClipY) {
        y_draw = BACKGROUND_Y;
        for (y_tile = 0; y_tile < BACKGROUND_HEIGHT_TILES; y_tile++) {
            x = x_reset + 1;
            x_draw = (BACKGROUND_X + BACKGROUND_TILE_SIZE) - x_offset;
            for (x_tile = 0; x_tile <= BACKGROUND_WIDTH_TILES - 2; x_tile++) {
                gfx_Sprite_NoClip(
                    zelda.background.tiles[zelda.background.tilemap[x + y_next]], x_draw,
                    y_draw);
                x++;
                x_draw += BACKGROUND_TILE_SIZE;
            }
            gfx_Sprite(zelda.background.tiles[zelda.background.tilemap[x_reset + y_next]],
                       BACKGROUND_X - x_offset, y_draw);
            gfx_Sprite(zelda.background.tiles[zelda.background.tilemap[x + y_next]],
                       x_draw, y_draw);
            y++;
            y_next += BACKGROUND_WIDTH;
            y_draw += BACKGROUND_TILE_SIZE;
        }
        return;
    } else {
        // should never happen, just don't render anything
    }
}

static void renderBackground() {
    if (!zelda.background.framesToDraw) {
        return;
    }
    zelda.background.framesToDraw--;

    uint24_t scrollX = (uint24_t)zelda.background.screenNumX * 256;
    uint24_t scrollY = (uint24_t)zelda.background.screenNumY * 176;
    switch (zelda.background.scrollDir) {
        case UP:
            scrollY += zelda.background.scrollOffset;
            break;
        case DOWN:
            scrollY -= zelda.background.scrollOffset;
            break;
        case LEFT:
            scrollX += zelda.background.scrollOffset;
            break;
        case RIGHT:
            scrollX -= zelda.background.scrollOffset;
            break;
        case NONE:
            break;
    }

    renderTilemap(scrollX, scrollY);
}

static void renderMap() {
    if (!zelda.background.framesToDrawMinimap) {
        return;
    }
    zelda.background.framesToDrawMinimap--;

    gfx_SetColor(13);
    gfx_FillRectangle_NoClip(16, 24, 64, 32);
    gfx_SetColor(19);
    gfx_FillRectangle_NoClip(zelda.background.screenNumX * 4 + 16,
                             zelda.background.screenNumY * 4 + 24, 4, 4);
}

static void undrawLink() {
    // If background is going to be drawn over it, don't bother
    if (!zelda.background.framesToDraw) {
        gfx_Sprite(zelda.link.backgroundBehind2, zelda.link.prevX2 + BACKGROUND_X, zelda.link.prevY2 + BACKGROUND_Y);
    }
}

static void copyBackgroundLink() {
    if (zelda.background.framesToDraw >= 2) {
        // Skip if drawing background in two frames
        return;
    }
    // Swap buffers
    gfx_sprite_t *temp = zelda.link.backgroundBehind1;
    zelda.link.backgroundBehind1 = zelda.link.backgroundBehind2;
    zelda.link.backgroundBehind2 = temp;

    // Copy screen to current buffer (2 frames out)
    gfx_GetSprite(zelda.link.backgroundBehind1, zelda.link.xPos + BACKGROUND_X, zelda.link.yPos + BACKGROUND_Y);
}

static void renderLink() {
    gfx_RLETSprite(zelda.link.sprites[zelda.link.spriteIndex], zelda.link.xPos + BACKGROUND_X,
                   zelda.link.yPos + BACKGROUND_Y);
}

#if DEBUG
static void renderDebug() {
    zelda.debug.totalTime =
        zelda.debug.inputTime + zelda.debug.logicTime + zelda.debug.renderTime;
    for (uint8_t i = 0; i < FPS_COUNT; i++) {
        zelda.debug.frameTimestamps[i] = zelda.debug.frameTimestamps[i + 1];
    }
    zelda.debug.frameTimestamps[FPS_COUNT] = clock();
    zelda.debug.fps =
        (CLOCKS_PER_SEC * FPS_COUNT) / (zelda.debug.frameTimestamps[FPS_COUNT] -
                                        zelda.debug.frameTimestamps[0] - 1);

    gfx_SetColor(12);
    gfx_FillRectangle_NoClip(256, 0, 64, 240);

    fontlib_SetCursorPosition(256, 0);
    fontlib_DrawString("FPS=");
    fontlib_DrawInt(zelda.debug.fps, 1);

    fontlib_SetCursorPosition(256, 8);
    fontlib_DrawString("Tt=");
    fontlib_DrawInt(zelda.debug.totalTime, 1);
}
#endif

void renderScreen() {
#if DEBUG
    clock_t start = clock();
#endif

    if (zelda.settings.waitFrames) {
        zelda.debug.renderTime = 0;
        renderDebug();
        gfx_SwapDraw();
        gfx_Wait();
        zelda.settings.waitFrames--;
        return;
    }

    gfx_SetClipRegion(BACKGROUND_X, BACKGROUND_Y, BACKGROUND_X + BACKGROUND_WIDTH,
                      BACKGROUND_Y + BACKGROUND_HEIGHT);
    undrawLink();
    renderBackground();
    copyBackgroundLink();
    renderLink();
    gfx_SetClipRegion(0, 0, GFX_LCD_WIDTH, GFX_LCD_HEIGHT);

    renderMap();

#if DEBUG
    zelda.debug.renderTime = clock() - start;
    renderDebug();
#endif

    gfx_SwapDraw();
    gfx_Wait();
}
