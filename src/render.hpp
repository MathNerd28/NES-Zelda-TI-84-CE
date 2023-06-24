#pragma once

#include "defines.hpp"

#define BACKGROUND_TILE_SIZE 16
#define BACKGROUND_X 0
#define BACKGROUND_Y 64
#define BACKGROUND_WIDTH_TILES 16
#define BACKGROUND_HEIGHT_TILES 11
#define BACKGROUND_WIDTH (BACKGROUND_WIDTH_TILES * BACKGROUND_TILE_SIZE)
#define BACKGROUND_HEIGHT (BACKGROUND_HEIGHT_TILES * BACKGROUND_TILE_SIZE)

void renderInit();
void renderExit();
void renderScreen();
