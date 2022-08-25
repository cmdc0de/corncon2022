/* GIMP RGBA C-Source image dump (player_shot.c) */

#include  <stdint.h>
static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[4 * 12 * 2 + 1];
} player_shot = {
  4, 12, 2,
  "\000\000\340\007\340\007\000\000\000\000\340\007\340\007\000\000\000\000\340\007\340\007\000\000\000\000\340\007\340"
  "\007\000\000\000\000\340\007\340\007\000\000\000\000\340\007\340\007\000\000\000\000\340\007\340\007\000\000\000\000\340"
  "\007\340\007\000\000\000\000\340\007\340\007\000\000\000\000\340\007\340\007\000\000\000\000\340\007\340\007\000\000"
  "\000\000\340\007\340\007\000\000",
};

uint32_t getWidthplayer_shot() {return player_shot.width;}
uint32_t getHeightplayer_shot() {return player_shot.height;}
uint32_t getBytePerPixelplayer_shot() {return player_shot.bytes_per_pixel;}
const uint8_t *getPixelDataplayer_shot() {return &player_shot.pixel_data[0];}
