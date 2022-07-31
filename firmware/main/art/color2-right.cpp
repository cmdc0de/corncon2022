#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (color2-right.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[11 * 9 * 2 + 1];
} gimp_image = {
  11, 9, 2,
  "\000\000\000\000\000\000\000\000[\007\032\007[\007\000\000\000\000\000\000\000\000\000\000\000\000\366\005\234\007:\007[\007[\007"
  "\234\007\366\005\000\000\000\000\000\000\066\006{\017:\007\336\327\335\237[\007[\017\377\337xV\000\000"
  "\000\000{\007:\007\337\357\236\367\000\000\331\006\377\377\030\306\306\001\000\000\032\007[\007:\007"
  "\336\337\276\377\252B\371\006\377\367y\326\353\":\007\331\006[\007[\017:\007\235\267"
  "\234\207[\007[\007\275\277|G\331\006\331\006[\007[\007[\007[\007[\007[\007[\007[\007[\007\331\006\371"
  "\006[\007[\007\270\006[\007[\007[\007\270\006[\007[\007\371\006P\004\274\007x\006\000\000S\005\274\007S\005\000\000"
  "x\006\274\007P\004",
};

uint32_t getWidthcolor2right() {return gimp_image.width;}
uint32_t getHeightcolor2right() {return gimp_image.height;}
uint32_t getBytePerPixelcolor2right() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatacolor2right() {return &gimp_image.pixel_data[0];}
