#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (color2-down.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[11 * 9 * 2 + 1];
} gimp_image = {
  11, 9, 2,
  "\000\000\000\000\000\000\000\000[\007\032\007[\007\000\000\000\000\000\000\000\000\000\000\000\000\366\005\234\007[\007[\007[\007"
  "\234\007\366\005\000\000\000\000\000\000\066\006{\007:\007[\017[\007[\017:\007{\007\066\006\000\000\000\000{\007\235"
  "\267\377\377:\007[\017:\007\377\377\235\267{\007\000\000\032\007[g\277\377<\347\377\377"
  ":\007\377\377<\347\277\377[g\032\007\331\006[GT}\000\000\275\247:\007\275\247\000\000T}[G"
  "\331\006\331\006[\007:\007\032\007[\007[\007[\007\032\007:\007[\007\331\006\331\006;\007\371\006[\007:\007x\006"
  ":\007[\007\371\006;\007\331\006:\007/\004\211\002[\007\032\007\000\000\032\007[\007\211\002/\004:\007",
};

uint32_t getWidthcolor2down() {return gimp_image.width;}
uint32_t getHeightcolor2down() {return gimp_image.height;}
uint32_t getBytePerPixelcolor2down() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatacolor2down() {return &gimp_image.pixel_data[0];}
