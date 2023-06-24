#include "render.hpp"

#include "colors.hpp"
#include "defines.hpp"
#include "gfx/font.h"
#include "gfx/gfx.h"
#include "graphics.hpp"

#include <fontlibc.h>
#include <graphx.h>
#include <sys/timers.h>
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
    uint8_t x, x_res, x_tile, y, y_tile;
    uint24_t y_next;

    x_res = x_offset / BACKGROUND_TILE_SIZE;
    y = y_offset / BACKGROUND_TILE_SIZE;
    x_offset %= BACKGROUND_TILE_SIZE;
    y_draw = BACKGROUND_Y - (y_offset % BACKGROUND_TILE_SIZE);

    // First row
    x = x_res;
    y_next = y * BACKGROUND_WIDTH;
    x_draw = BACKGROUND_X - x_offset;
    for (x_tile = 0; x_tile <= BACKGROUND_WIDTH_TILES; x_tile++) {
        gfx_Sprite(zelda.background.tiles[zelda.background.tilemap[x + y_next]],
                   x_draw, y_draw);
        x++;
        x_draw += BACKGROUND_TILE_SIZE;
    }
    y++;
    y_draw += BACKGROUND_TILE_SIZE;
    y_next += BACKGROUND_WIDTH;

    for (y_tile = 1; y_tile < BACKGROUND_HEIGHT_TILES; y_tile++) {
        x = x_res + 1;
        x_draw = (BACKGROUND_X + BACKGROUND_TILE_SIZE) - x_offset;
        for (x_tile = 1; x_tile < BACKGROUND_WIDTH_TILES; x_tile++) {
            gfx_Sprite_NoClip(
                zelda.background.tiles[zelda.background.tilemap[x + y_next]], x_draw,
                y_draw);
            x++;
            x_draw += BACKGROUND_TILE_SIZE;
        }
        gfx_Sprite(zelda.background.tiles[zelda.background.tilemap[x_res + y_next]],
                   BACKGROUND_X - x_offset, y_draw);
        gfx_Sprite(zelda.background.tiles[zelda.background.tilemap[x + y_next]],
                   x_draw, y_draw);
        y++;
        y_next += BACKGROUND_WIDTH;
        y_draw += BACKGROUND_TILE_SIZE;
    }

    // Last row
    x = x_res;
    x_draw = BACKGROUND_X - x_offset;
    for (x_tile = 0; x_tile <= BACKGROUND_WIDTH_TILES; x_tile++) {
        gfx_Sprite(zelda.background.tiles[zelda.background.tilemap[x + y_next]],
                   x_draw, y_draw);
        x++;
        x_draw += BACKGROUND_TILE_SIZE;
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
        gfx_Sprite(zelda.link.backgroundBehind2, zelda.link.prevX2+BACKGROUND_X, zelda.link.prevY2 + BACKGROUND_Y);
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
    fontlib_DrawString("Lt=");
    fontlib_DrawInt(zelda.debug.logicTime, 1);

    fontlib_SetCursorPosition(256, 16);
    fontlib_DrawString("Rt=");
    fontlib_DrawInt(zelda.debug.renderTime, 1);

    fontlib_SetCursorPosition(256, 24);
    fontlib_DrawString("Tt=");
    fontlib_DrawInt(zelda.debug.totalTime, 1);

    fontlib_SetCursorPosition(256, 32);
    fontlib_DrawString("D=");
    fontlib_DrawInt(zelda.link.dirFacing, 1);

    fontlib_SetCursorPosition(256, 40);
    fontlib_DrawString("X=");
    fontlib_DrawInt(zelda.link.xPos, 1);

    fontlib_SetCursorPosition(256, 48);
    fontlib_DrawString("Y=");
    fontlib_DrawInt(zelda.link.yPos, 1);

    fontlib_SetCursorPosition(256, 56);
    fontlib_DrawString("Xs=");
    fontlib_DrawInt(zelda.link.xSubPos, 1);

    fontlib_SetCursorPosition(256, 64);
    fontlib_DrawString("Ys=");
    fontlib_DrawInt(zelda.link.ySubPos, 1);

    fontlib_SetCursorPosition(256, 72);
    fontlib_DrawString("A=");
    fontlib_DrawInt(zelda.link.spriteIndex, 1);
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
