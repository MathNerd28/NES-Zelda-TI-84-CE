#include "input.hpp"

#include "defines.hpp"

#include <keypadc.h>

#if DEBUG
#include <time.h>
#endif

// These can be combined since the other 4 bits are unused:
// #define POLL_UP_BUTTON (kb_Data[7] & kb_Up)
// #define POLL_DOWN_BUTTON (kb_Data[7] & kb_Down)
// #define POLL_LEFT_BUTTON (kb_Data[7] & kb_Left)
// #define POLL_RIGHT_BUTTON (kb_Data[7] & kb_Right)
#define POLL_DIRECTION_BUTTONS kb_Data[7]
#define POLL_A_BUTTON (kb_Data[1] & kb_2nd)
#define POLL_B_BUTTON (kb_Data[2] & kb_Alpha)
#define POLL_SELECT_BUTTON (kb_Data[1] & kb_Mode)
#define POLL_START_BUTTON (kb_Data[3] & kb_GraphVar)
#define POLL_QUIT_BUTTON (kb_Data[6] & kb_Clear)

void inputInit() { kb_SetMode(MODE_3_CONTINUOUS); }

void inputExit() {}

void inputPoll() {
#if DEBUG
    clock_t start = clock();
#endif
    // Update all buttons
    zelda.controller.buttonsPrevious = zelda.controller.buttonsCurrent;
    zelda.controller.dpadDirPrevious = zelda.controller.dpadDirCurrent;

    uint8_t buttonsNew = 0;
    buttonsNew |= POLL_DIRECTION_BUTTONS;
    buttonsNew |= (POLL_B_BUTTON >> 3);
    buttonsNew |= POLL_A_BUTTON;
    buttonsNew |= POLL_SELECT_BUTTON;
    buttonsNew |= POLL_START_BUTTON;
    zelda.controller.buttonsCurrent = buttonsNew;

    zelda.controller.buttonQuit = POLL_QUIT_BUTTON;

    uint8_t buttonDelta = zelda.controller.buttonsPrevious ^ buttonsNew;
    zelda.controller.buttonsPressed = buttonDelta & buttonsNew;
    zelda.controller.buttonsReleased = buttonDelta & zelda.controller.buttonsPrevious;

    uint8_t newDpad = zelda.controller.buttonsPressed & DIRECTION_MASK;
    if (newDpad) {
        if (newDpad & UP_BUTTON) {
            zelda.controller.dpadDirCurrent = UP;
        } else if (newDpad & DOWN_BUTTON) {
            zelda.controller.dpadDirCurrent = DOWN;
        } else if (newDpad & LEFT_BUTTON) {
            zelda.controller.dpadDirCurrent = LEFT;
        } else if (newDpad & RIGHT_BUTTON) {
            zelda.controller.dpadDirCurrent = RIGHT;
        }
    } else if (zelda.controller.buttonsReleased & zelda.controller.dpadDirCurrent) {
        if (buttonsNew & UP_BUTTON) {
            zelda.controller.dpadDirCurrent = UP;
        } else if (buttonsNew & DOWN_BUTTON) {
            zelda.controller.dpadDirCurrent = DOWN;
        } else if (buttonsNew & LEFT_BUTTON) {
            zelda.controller.dpadDirCurrent = LEFT;
        } else if (buttonsNew & RIGHT_BUTTON) {
            zelda.controller.dpadDirCurrent = RIGHT;
        } else {
            zelda.controller.dpadDirCurrent = NONE;
        }
    }

#if DEBUG
    zelda.debug.inputTime = clock() - start;
#endif
}
