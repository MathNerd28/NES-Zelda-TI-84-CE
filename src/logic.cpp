#include "logic.hpp"

#include "defines.hpp"
#include "gfx/gfx.h"
#include "graphics.hpp"
#include "overworld.hpp"
#include "random.hpp"
#include "render.hpp"

#include <compression.h>
#include <graphx.h>
#include <string.h>

#if DEBUG
#include <time.h>
#endif

#define LINK_SUBPIXEL_SPEED 64

static void startScreenScroll(direction_t dir) {
    zelda.background.scrollDir = dir;
    zelda.background.scrollSpeed = SCREEN_SCROLL_SPEED_MIN;
    zelda.link.xSubPos = 0;
    zelda.link.ySubPos = 0;
    switch (dir) {
        case UP:
            zelda.background.screenNumY--;
            zelda.background.scrollOffset = BACKGROUND_HEIGHT - SCREEN_SCROLL_SPEED_MIN;
            zelda.link.yPos = LINK_POS_MIN_Y + SCREEN_SCROLL_SPEED_MIN;
            break;
        case DOWN:
            zelda.background.screenNumY++;
            zelda.background.scrollOffset = BACKGROUND_HEIGHT - SCREEN_SCROLL_SPEED_MIN;
            zelda.link.yPos = LINK_POS_MAX_Y - SCREEN_SCROLL_SPEED_MIN;
            break;
        case LEFT:
            zelda.background.screenNumX--;
            zelda.background.scrollOffset = BACKGROUND_WIDTH - SCREEN_SCROLL_SPEED_MIN;
            zelda.link.xPos = LINK_POS_MIN_X + SCREEN_SCROLL_SPEED_MIN;
            break;
        case RIGHT:
            zelda.background.screenNumX++;
            zelda.background.scrollOffset = BACKGROUND_WIDTH - SCREEN_SCROLL_SPEED_MIN;
            zelda.link.xPos = LINK_POS_MAX_X - SCREEN_SCROLL_SPEED_MIN;
            break;
        default:
            // Nothing
            break;
    }
}

static void loadTilemap(uint8_t *tilemapData, uint24_t length) {
    uint8_t *bufferPtr = zelda.background.tilemap;
    for (uint24_t i = 0; i < length; i += 2) {
        uint8_t len = tilemapData[i + 1];
        memset((void *)bufferPtr, tilemapData[i], len);
        bufferPtr += len;
    }
}

static void allocateTiles() {
    for (uint8_t i = 0; i < 128; i++) {
        zelda.background.tiles[i] = gfx_MallocSprite(16, 16);
    }
}

static void loadTiles(uint8_t **tilesCompressed, uint8_t n) {
    for (uint8_t i = 0; i < n; i++) {
        zx0_Decompress(zelda.background.tiles[i], tilesCompressed[i]);
    }
}

static void normalizeLinkSubpixels() {
    while (zelda.link.xSubPos < 0) {
        zelda.link.xSubPos += SUBPIXEL_COUNT;
        zelda.link.xPos--;
    }
    while (zelda.link.xSubPos >= SUBPIXEL_COUNT) {
        zelda.link.xSubPos -= SUBPIXEL_COUNT;
        zelda.link.xPos++;
    }
    while (zelda.link.ySubPos < 0) {
        zelda.link.ySubPos += SUBPIXEL_COUNT;
        zelda.link.yPos--;
    }
    while (zelda.link.ySubPos >= SUBPIXEL_COUNT) {
        zelda.link.ySubPos -= SUBPIXEL_COUNT;
        zelda.link.yPos++;
    }
}

static void checkScreenScroll() {
    if (zelda.background.scrollOffset > zelda.background.scrollSpeed) {
        zelda.background.scrollOffset -= zelda.background.scrollSpeed;
        zelda.background.framesToDraw = 2;
        switch (zelda.background.scrollDir) {
            case UP:
                zelda.link.yPos += zelda.background.scrollSpeed;
                break;
            case DOWN:
                zelda.link.yPos -= zelda.background.scrollSpeed;
                break;
            case LEFT:
                zelda.link.xPos += zelda.background.scrollSpeed;
                break;
            case RIGHT:
                zelda.link.xPos -= zelda.background.scrollSpeed;
                break;
            default:
                // Nothing
                break;
        }
        if (zelda.background.scrollOffset <= SCREEN_SCROLL_SPEED_MAX) {
            if (zelda.background.scrollSpeed >= 2) {
                zelda.background.scrollSpeed >>= 1;
            }
        } else if (zelda.background.scrollSpeed < SCREEN_SCROLL_SPEED_MAX) {
            zelda.background.scrollSpeed <<= 1;
        }
    } else if (zelda.background.scrollOffset) {
        switch (zelda.background.scrollDir) {
            case UP:
                zelda.link.yPos = LINK_POS_MAX_Y;
                break;
            case DOWN:
                zelda.link.yPos = LINK_POS_MIN_Y;
                break;
            case LEFT:
                zelda.link.xPos = LINK_POS_MAX_X;
                break;
            case RIGHT:
                zelda.link.xPos = LINK_POS_MIN_X;
                break;
            default:
                // Nothing
                break;
        }
        zelda.background.scrollOffset = 0;
        zelda.background.scrollSpeed = 0;
        zelda.background.scrollDir = NONE;
        zelda.background.framesToDraw = 2;
        zelda.background.framesToDrawMinimap = 2;
        zelda.settings.lock30fps = false;
    } else {
        if (zelda.link.yPos < LINK_POS_MIN_Y) {
            if (zelda.background.screenNumY > SCREEN_MIN_Y) {
                startScreenScroll(UP);
            }
        } else if (zelda.link.yPos > LINK_POS_MAX_Y) {
            if (zelda.background.screenNumY < SCREEN_MAX_Y) {
                startScreenScroll(DOWN);
            }
        } else if (zelda.link.xPos < LINK_POS_MIN_X) {
            if (zelda.background.screenNumX > SCREEN_MIN_X) {
                startScreenScroll(LEFT);
            }
        } else if (zelda.link.xPos > LINK_POS_MAX_X) {
            if (zelda.background.screenNumX < SCREEN_MAX_X) {
                startScreenScroll(RIGHT);
            }
        }
    }
}

static void snapLinkY() {
    zelda.link.ySubPos = 0;
    uint8_t offset = zelda.link.yPos % LINK_GRID_SIZE;
    if (!offset) {
        return;
    } else if (offset < (LINK_GRID_SIZE / 2)) {
        zelda.link.yPos--;
    } else {
        zelda.link.yPos++;
    }
}

static void snapLinkX() {
    zelda.link.xSubPos = 0;
    uint8_t offset = zelda.link.xPos % LINK_GRID_SIZE;
    if (!offset) {
        return;
    } else if (offset < (LINK_GRID_SIZE / 2)) {
        zelda.link.xPos--;
    } else {
        zelda.link.xPos++;
    }
}

static void dontMoveLink() {
    zelda.link.prevX2 = zelda.link.prevX;
    zelda.link.prevY2 = zelda.link.prevY;
    zelda.link.prevX = zelda.link.xPos;
    zelda.link.prevY = zelda.link.yPos;
}

static void moveLink() {
    zelda.link.prevX2 = zelda.link.prevX;
    zelda.link.prevY2 = zelda.link.prevY;
    zelda.link.prevX = zelda.link.xPos;
    zelda.link.prevY = zelda.link.yPos;

    if (zelda.background.scrollDir) {
        // Screen is scrolling, no input allowed
        return;
    }

    if (zelda.controller.dpadDirCurrent == UP) {
        snapLinkX();
        zelda.link.ySubPos -= LINK_SUBPIXEL_SPEED;
        zelda.link.dirFacing = UP;
        zelda.link.spriteIndex &= 1;
        if (zelda.controller.dpadDirPrevious != UP) {
            zelda.link.spriteClock = 7;
        }
    } else if (zelda.controller.dpadDirCurrent == DOWN) {
        snapLinkX();
        zelda.link.ySubPos += LINK_SUBPIXEL_SPEED;
        zelda.link.dirFacing = DOWN;
        zelda.link.spriteIndex = 2 | (zelda.link.spriteIndex & 1);
        if (zelda.controller.dpadDirPrevious != DOWN) {
            zelda.link.spriteClock = 7;
        }
    } else if (zelda.controller.dpadDirCurrent == LEFT) {
        snapLinkY();
        zelda.link.xSubPos -= LINK_SUBPIXEL_SPEED;
        zelda.link.dirFacing = LEFT;
        zelda.link.spriteIndex = 4 | (zelda.link.spriteIndex & 1);
        if (zelda.controller.dpadDirPrevious != LEFT) {
            zelda.link.spriteClock = 7;
        }
    } else if (zelda.controller.dpadDirCurrent == RIGHT) {
        snapLinkY();
        zelda.link.xSubPos += LINK_SUBPIXEL_SPEED;
        zelda.link.dirFacing = RIGHT;
        zelda.link.spriteIndex = 6 | (zelda.link.spriteIndex & 1);
        if (zelda.controller.dpadDirPrevious != RIGHT) {
            zelda.link.spriteClock = 7;
        }
    }

    if (zelda.controller.dpadDirCurrent) {
        zelda.link.spriteClock++;
        zelda.link.spriteClock &= 7;
        if (!zelda.link.spriteClock && zelda.link.spriteIndex < 14) {
            // Change sprite if animation is a walking sprite
            zelda.link.spriteIndex ^= 1;
        }
    }

    normalizeLinkSubpixels();
}

static uint8_t getCollision(int24_t posX, int24_t posY) {
    uint8_t tile = zelda.background.tilemap[(((posY >> 4) + (zelda.background.screenNumY * 11)) << 8) + (zelda.background.screenNumX << 4) + (posX >> 4)];
    uint8_t tileCollision = zelda.background.tileCollision[tile];
    uint8_t type = tileCollision & 0x0F;
    // Upper nibble - UL, UR, BL, BR
    // Lower nibble - collision type
    if (posY & 1 << 3) {
        if (posX & 1 << 3) {
            // BR
            return (tileCollision & 1 << 4) ? type : 0;
        } else {
            // BL
            return (tileCollision & 1 << 5) ? type : 0;
        }
    } else {
        if (posX & 1 << 3) {
            // UR
            return (tileCollision & 1 << 6) ? type : 0;
        } else {
            // UL
            return (tileCollision & 1 << 7) ? type : 0;
        }
    }
}

static void checkLinkWallCollision() {
    if (zelda.background.scrollDir) {
        // Link can't move while scrolling
        return;
    }

    if (zelda.link.xPos != zelda.link.prevX) {
        bool movingRight = zelda.link.xPos > zelda.link.prevX;
        int24_t currentX = zelda.link.xPos + (movingRight ? 15 : 0);

        uint8_t c1 = getCollision(currentX, zelda.link.yPos + 8);
        uint8_t c2 = getCollision(currentX, zelda.link.yPos + 15);

        if ((c1 != 0 && c1 != 2) || (c2 != 0 && c2 != 2)) {
            if (!movingRight) {
                zelda.link.xPos += 8;
            }
            zelda.link.xPos &= 0xFFFFF8;
            zelda.link.xSubPos = 0;
        }
    }

    if (zelda.link.yPos != zelda.link.prevY) {
        bool movingDown = zelda.link.yPos > zelda.link.prevY;
        int24_t currentY = zelda.link.yPos + (movingDown ? 15 : 8);

        uint8_t c1 = getCollision(zelda.link.xPos, currentY);
        uint8_t c2 = getCollision(zelda.link.xPos + 8, currentY);
        uint8_t c3 = getCollision(zelda.link.xPos + 15, currentY);

        if ((c1 != 0 && c1 != 2) || (c2 != 0 && c2 != 2) || (c3 != 0 && c3 != 2)) {
            if (!movingDown) {
                zelda.link.yPos += 8;
            }
            zelda.link.yPos &= 0xFFFFF8;
            zelda.link.ySubPos = 0;
        }
    }
}

void logicInit() {
    srandom(4);
    allocateTiles();
    loadTilemap(overworld_tile_map_quest_1, sizeof(overworld_tile_map_quest_1));
    loadTiles(overworld_tileset_swapped_tiles_compressed, 89);
    zelda.background.tileCollision = overworld_tile_collisions_quest_1;
    zelda.background.paletteMap = overworld_palette_map_quest_1;
    zelda.background.screenNumX = 7;
    zelda.background.screenNumY = 7;
    zelda.background.scrollOffset = 0;
    zelda.background.scrollDir = NONE;
    zelda.background.framesToDraw = 2;
    zelda.background.framesToDrawMinimap = 2;

    zelda.link.xPos = 120;
    zelda.link.yPos = 80;
}

void logicExit() {}

void logicProcess() {
#if DEBUG
    clock_t start = clock();
#endif

    if (zelda.settings.waitFrames) {
        dontMoveLink();
    } else {
        moveLink();
        checkLinkWallCollision();
        checkScreenScroll();
    }

#if DEBUG
    zelda.debug.logicTime = clock() - start;
#endif
}
