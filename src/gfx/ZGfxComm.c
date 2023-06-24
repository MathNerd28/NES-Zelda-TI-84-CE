#include "ZGfxComm.h"
#include <fileioc.h>

#define ZGfxComm_HEADER_SIZE 0

unsigned char *ZGfxComm_appvar[1] =
{
    (unsigned char*)0,
};

unsigned char *link_sprites_swapped_tiles_data[20] =
{
    (unsigned char*)0,
    (unsigned char*)258,
    (unsigned char*)516,
    (unsigned char*)774,
    (unsigned char*)1032,
    (unsigned char*)1290,
    (unsigned char*)1548,
    (unsigned char*)1806,
    (unsigned char*)2064,
    (unsigned char*)2322,
    (unsigned char*)2580,
    (unsigned char*)2838,
    (unsigned char*)3096,
    (unsigned char*)3354,
    (unsigned char*)3612,
    (unsigned char*)3870,
    (unsigned char*)4128,
    (unsigned char*)4386,
    (unsigned char*)4644,
    (unsigned char*)4902,
};

unsigned char ZGfxComm_init(void)
{
    unsigned int data, i;
    uint8_t appvar;

    appvar = ti_Open("ZGfxComm", "r");
    if (appvar == 0)
    {
        return 0;
    }

    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)ZGfxComm_appvar[0] + ZGfxComm_HEADER_SIZE;
    for (i = 0; i < 1; i++)
    {
        ZGfxComm_appvar[i] += data;
    }

    ti_Close(appvar);

    data = (unsigned int)ZGfxComm_appvar[0] - (unsigned int)link_sprites_swapped_tiles_data[0];
    for (i = 0; i < link_sprites_swapped_tiles_num; i++)
    {
        link_sprites_swapped_tiles_data[i] += data;
    }

    return 1;
}

