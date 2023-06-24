#pragma once

#include <graphx.h>
#include <time.h>

#define DEBUG true

// Button masks
#define DOWN_BUTTON (1 << 0)
#define LEFT_BUTTON (1 << 1)
#define RIGHT_BUTTON (1 << 2)
#define UP_BUTTON (1 << 3)
#define B_BUTTON (1 << 4)
#define A_BUTTON (1 << 5)
#define SELECT_BUTTON (1 << 6)
#define START_BUTTON (1 << 7)
#define DIRECTION_MASK (DOWN_BUTTON | LEFT_BUTTON | RIGHT_BUTTON | UP_BUTTON)

#if DEBUG
#define FPS_COUNT 10
#endif

enum direction_t : uint8_t {
    NONE = 0,
    DOWN = DOWN_BUTTON,
    LEFT = LEFT_BUTTON,
    RIGHT = RIGHT_BUTTON,
    UP = UP_BUTTON
};
enum collision_t : uint8_t {
    GROUND = 0b00,
    STAIRS = 0b01,
    WALL = 0b10,
    WATER = 0b11
};

struct settings_t {
    bool lock30fps;
    uint8_t waitFrames;
};

struct controller_t {
    uint8_t buttonsCurrent;
    uint8_t buttonsPrevious;
    uint8_t buttonsPressed;
    uint8_t buttonsReleased;
    direction_t dpadDirCurrent;
    direction_t dpadDirPrevious;
    bool buttonQuit;
};

struct background_t {
    uint8_t paletteIndex;
    uint8_t *tilemap = new uint8_t[22528];
    uint8_t *paletteMap;
    gfx_sprite_t *tiles[128];
    uint8_t *tileCollision;
    uint8_t screenNumX;
    uint8_t screenNumY;
    direction_t scrollDir;
    uint8_t scrollSpeed;
    uint8_t scrollOffset;
    uint8_t framesToDraw;
    uint8_t framesToDrawMinimap;
};

struct link_t {
    gfx_rletsprite_t *sprites[20];
    uint8_t paletteIndex;
    uint8_t paletteClock;
    uint8_t spriteIndex;
    uint8_t spriteClock;
    direction_t dirFacing;
    int24_t xPos;
    int24_t yPos;
    int8_t xSubPos;
    int8_t ySubPos;
    int24_t prevX;
    int24_t prevY;
    int24_t prevX2;
    int24_t prevY2;
    gfx_TempSprite(backgroundBehind1, 16, 16);
    gfx_TempSprite(backgroundBehind2, 16, 16);
};

struct inventory_t {
    uint8_t health;
    uint8_t maxHealth;
    uint8_t bombCount;
    uint8_t maxBombs;
    uint8_t rupeeCount;
    uint8_t ringIndex;
};

#if DEBUG
struct debug_t {
    clock_t inputTime;
    clock_t logicTime;
    clock_t renderTime;
    clock_t totalTime;
    clock_t frameTimestamps[FPS_COUNT + 1];
    uint8_t fps;
};
#endif

struct zelda_t {
    controller_t controller;
    background_t background;
    link_t link;
    settings_t settings;

#if DEBUG
    debug_t debug;
#endif

    ~zelda_t() {
        delete[] background.tilemap;
    }
};

extern zelda_t *zelda_game;
#define zelda (*zelda_game)
