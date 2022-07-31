#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (color2-up.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[11 * 9 * 2 + 1];
} gimp_image = {
  11, 9, 2,
  "\000\000\000\000\000\000\000\000[\007\032\007[\007\000\000\000\000\000\000\000\000\000\000\000\000\224\005\331\006[\007[\017[\007"
  "\331\006\224\005\000\000\000\000\000\000WN\266\225\000\000\336\307:\007\336\307\000\000\266\225WN\000"
  "\000\000\000\234g\377\377\337\377\377\367:\007\377\367\337\377\377\377\234g\000\000\032"
  "\007:\007\234\247\377\357:\007[\027:\007\377\357\234\247:\007\032\007\331\006[\007:\007:\007[\017"
  "[\007[\017:\007:\007[\007\331\006\331\006[\007[\007[\007[\007[\007[\007[\007[\007[\007\331\006\331\006;\007\371"
  "\006[\007:\007x\006:\007[\007\371\006;\007\331\006:\007/\004\211\002[\007\032\007\000\000\032\007[\007\211\002/\004"
  ":\007",
};

uint32_t getWidthcolor2up() {return gimp_image.width;}
uint32_t getHeightcolor2up() {return gimp_image.height;}
uint32_t getBytePerPixelcolor2up() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatacolor2up() {return &gimp_image.pixel_data[0];}
