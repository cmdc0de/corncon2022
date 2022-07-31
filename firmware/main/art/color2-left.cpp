#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (color2-left.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[11 * 9 * 2 + 1];
} gimp_image = {
  11, 9, 2,
  "\000\000\000\000\000\000\000\000[\007\032\007[\007\000\000\000\000\000\000\000\000\000\000\000\000\366\005\234\007[\007[\007:\007"
  "\234\007\366\005\000\000\000\000\000\000xV\377\337[\017[\007\335\237\336\327:\007{\017\066\006\000\000"
  "\000\000\306\001\030\306\377\377\331\006\000\000\236\367\337\357:\007{\007\000\000:\007\353\"y\326"
  "\377\367\371\006\252B\276\377\336\337:\007[\007\032\007\331\006|G\275\277[\007[\007\234"
  "\207\235\267:\007[\017[\007\331\006\331\006[\007[\007[\007[\007[\007[\007[\007[\007[\007\331\006\331\006"
  ";\007\371\006[\007:\007x\006:\007[\007\371\006;\007\331\006:\007/\004\211\002[\007\032\007\000\000\032\007[\007\211"
  "\002/\004:\007",
};

uint32_t getWidthcolor2left() {return gimp_image.width;}
uint32_t getHeightcolor2left() {return gimp_image.height;}
uint32_t getBytePerPixelcolor2left() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatacolor2left() {return &gimp_image.pixel_data[0];}
