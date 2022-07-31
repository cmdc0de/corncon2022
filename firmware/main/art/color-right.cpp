#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (color-right.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[11 * 9 * 2 + 1];
} gimp_image = {
  11, 9, 2,
  "\000\000\000\000\000\000\000\000\037\376\376\365\037\376\000\000\000\000\000\000\000\000\000\000\000\000\031\315_\376"
  "\037\376\037\376?\376_\376\371\314\000\000\000\000\000\000:\325_\376\377\375\177\377\377"
  "\376?\376?\376\277\377\233\335\000\000\000\000_\376\037\376\277\377\236\367\000\000\275"
  "\355\377\377\030\306\251I\000\000\376\365?\376\037\376\237\377\276\367JR\276\365"
  "\337\377y\316lb\037\376\335\355?\376?\376\377\375\037\377\337\376?\376?\376"
  "?\377\177\376\335\355\335\355?\376?\376?\376?\376?\376?\376?\376?\376?\376"
  "\335\355\336\365?\376?\376\275\355?\376?\376?\376\275\355?\376?\376\336\365"
  "\263\233\177\376|\345\000\000\227\274\237\376\227\274\000\000|\345\177\376\263\233",
};

uint32_t getWidthcolorright() {return gimp_image.width;}
uint32_t getHeightcolorright() {return gimp_image.height;}
uint32_t getBytePerPixelcolorright() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatacolorright() {return &gimp_image.pixel_data[0];}
