#pragma once

#define LINK_POS_MIN_X -8
#define LINK_POS_MAX_X 248
#define LINK_POS_MIN_Y -8
#define LINK_POS_MAX_Y 168

#define LINK_GRID_SIZE 8
#define SUBPIXEL_COUNT 16

#define SCREEN_MIN_X 0
#define SCREEN_MAX_X 15
#define SCREEN_MIN_Y 0
#define SCREEN_MAX_Y 7

#define SCREEN_SCROLL_SPEED_MAX 8
#define SCREEN_SCROLL_SPEED_MIN 1

void logicInit();
void logicProcess();
void logicExit();
