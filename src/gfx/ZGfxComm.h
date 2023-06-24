#ifndef ZGfxComm_appvar_include_file
#define ZGfxComm_appvar_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define link_sprites_palette_offset 0
#define link_sprites_swapped_tile_width 16
#define link_sprites_swapped_tile_height 16
#define link_sprites_swapped ZGfxComm_appvar[0]
#define link_sprites_swapped_tiles_num 20
extern unsigned char *link_sprites_swapped_tiles_data[20];
#define link_sprites_swapped_tiles ((gfx_sprite_t**)link_sprites_swapped_tiles_data)
#define link_sprites_swapped_tile_0 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[0])
#define link_sprites_swapped_tile_1 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[1])
#define link_sprites_swapped_tile_2 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[2])
#define link_sprites_swapped_tile_3 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[3])
#define link_sprites_swapped_tile_4 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[4])
#define link_sprites_swapped_tile_5 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[5])
#define link_sprites_swapped_tile_6 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[6])
#define link_sprites_swapped_tile_7 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[7])
#define link_sprites_swapped_tile_8 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[8])
#define link_sprites_swapped_tile_9 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[9])
#define link_sprites_swapped_tile_10 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[10])
#define link_sprites_swapped_tile_11 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[11])
#define link_sprites_swapped_tile_12 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[12])
#define link_sprites_swapped_tile_13 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[13])
#define link_sprites_swapped_tile_14 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[14])
#define link_sprites_swapped_tile_15 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[15])
#define link_sprites_swapped_tile_16 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[16])
#define link_sprites_swapped_tile_17 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[17])
#define link_sprites_swapped_tile_18 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[18])
#define link_sprites_swapped_tile_19 ((gfx_sprite_t*)link_sprites_swapped_tiles_data[19])
#define ZGfxComm_entries_num 1
extern unsigned char *ZGfxComm_appvar[1];
unsigned char ZGfxComm_init(void);

#ifdef __cplusplus
}
#endif

#endif
