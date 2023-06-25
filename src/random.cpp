#include "random.hpp"

static uint24_t random_state = 1;

void srandom(uint24_t state) {
    random_state = state;
}

uint8_t random() {
    uint24_t x = random_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    random_state = x;
    return (uint8_t)(x & 0xFF);
}
