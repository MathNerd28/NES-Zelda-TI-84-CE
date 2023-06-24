#include "random.hpp"

static uint24_t random_state = 1;

void srandom(uint24_t state) {
    random_state = state;
}

uint24_t random() {
    uint24_t x = random_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return random_state = x;
}
