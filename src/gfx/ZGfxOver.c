#include "ZGfxOver.h"
#include <fileioc.h>

#define ZGfxOver_HEADER_SIZE 0

unsigned char *ZGfxOver_appvar[1] =
{
    (unsigned char*)0,
};

unsigned char *overworld_tileset_swapped_tiles_compressed[89] =
{
    (unsigned char*)0,
    (unsigned char*)67,
    (unsigned char*)134,
    (unsigned char*)200,
    (unsigned char*)258,
    (unsigned char*)343,
    (unsigned char*)407,
    (unsigned char*)491,
    (unsigned char*)519,
    (unsigned char*)605,
    (unsigned char*)644,
    (unsigned char*)681,
    (unsigned char*)744,
    (unsigned char*)820,
    (unsigned char*)875,
    (unsigned char*)946,
    (unsigned char*)1016,
    (unsigned char*)1084,
    (unsigned char*)1119,
    (unsigned char*)1139,
    (unsigned char*)1219,
    (unsigned char*)1254,
    (unsigned char*)1288,
    (unsigned char*)1355,
    (unsigned char*)1420,
    (unsigned char*)1455,
    (unsigned char*)1527,
    (unsigned char*)1534,
    (unsigned char*)1592,
    (unsigned char*)1677,
    (unsigned char*)1741,
    (unsigned char*)1806,
    (unsigned char*)1841,
    (unsigned char*)1913,
    (unsigned char*)1997,
    (unsigned char*)2047,
    (unsigned char*)2067,
    (unsigned char*)2118,
    (unsigned char*)2125,
    (unsigned char*)2196,
    (unsigned char*)2266,
    (unsigned char*)2334,
    (unsigned char*)2384,
    (unsigned char*)2404,
    (unsigned char*)2455,
    (unsigned char*)2490,
    (unsigned char*)2545,
    (unsigned char*)2577,
    (unsigned char*)2638,
    (unsigned char*)2645,
    (unsigned char*)2712,
    (unsigned char*)2779,
    (unsigned char*)2845,
    (unsigned char*)2900,
    (unsigned char*)2932,
    (unsigned char*)2993,
    (unsigned char*)3013,
    (unsigned char*)3041,
    (unsigned char*)3080,
    (unsigned char*)3117,
    (unsigned char*)3149,
    (unsigned char*)3212,
    (unsigned char*)3288,
    (unsigned char*)3343,
    (unsigned char*)3393,
    (unsigned char*)3428,
    (unsigned char*)3479,
    (unsigned char*)3551,
    (unsigned char*)3636,
    (unsigned char*)3696,
    (unsigned char*)3746,
    (unsigned char*)3818,
    (unsigned char*)3903,
    (unsigned char*)3963,
    (unsigned char*)4013,
    (unsigned char*)4051,
    (unsigned char*)4071,
    (unsigned char*)4112,
    (unsigned char*)4185,
    (unsigned char*)4222,
    (unsigned char*)4229,
    (unsigned char*)4309,
    (unsigned char*)4382,
    (unsigned char*)4419,
    (unsigned char*)4426,
    (unsigned char*)4506,
    (unsigned char*)4539,
    (unsigned char*)4564,
    (unsigned char*)4605,
};

unsigned char ZGfxOver_init(void)
{
    unsigned int data, i;
    uint8_t appvar;

    appvar = ti_Open("ZGfxOver", "r");
    if (appvar == 0)
    {
        return 0;
    }

    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)ZGfxOver_appvar[0] + ZGfxOver_HEADER_SIZE;
    for (i = 0; i < 1; i++)
    {
        ZGfxOver_appvar[i] += data;
    }

    ti_Close(appvar);

    data = (unsigned int)ZGfxOver_appvar[0] - (unsigned int)overworld_tileset_swapped_tiles_compressed[0];
    for (i = 0; i < overworld_tileset_swapped_tiles_num; i++)
    {
        overworld_tileset_swapped_tiles_compressed[i] += data;
    }

    return 1;
}

