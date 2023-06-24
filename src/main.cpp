#include "defines.hpp"
#include "input.hpp"
#include "logic.hpp"
#include "render.hpp"

#include <sys/timers.h>
#include <time.h>

zelda_t *zelda_game;

int main() {
    zelda_game = new zelda_t();

    inputInit();
    renderInit();
    logicInit();

    while (!zelda.controller.buttonQuit) {
        clock_t start = clock();

        inputPoll();
        logicProcess();
        renderScreen();

        if (zelda.settings.lock30fps || zelda.settings.waitFrames) {
            uint24_t target = start + 1050;
            while (clock() < target) {
                usleep(500);
            }
        }
    }

    inputExit();
    logicExit();
    renderExit();

    // delete zelda_game;
}
