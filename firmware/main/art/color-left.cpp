#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (color-left.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[11 * 9 * 2 + 1];
} gimp_image = {
  11, 9, 2,
  "\000\000\000\000\000\000\000\000\037\376\376\365\037\376\000\000\000\000\000\000\000\000\000\000\000\000\371\314_\376"
  "?\376\037\376\037\376_\376\031\315\000\000\000\000\000\000\233\335\277\377?\376?\376\377"
  "\376\177\377\377\375_\376:\325\000\000\000\000\251I\030\306\377\377\275\355\000\000\236"
  "\367\277\377\037\376_\376\000\000\037\376lby\316\337\377\276\365JR\276\367\237"
  "\377\037\376?\376\376\365\335\355\177\376?\377?\376?\376\337\376\037\377\377"
  "\375?\376?\376\335\355\335\355?\376?\376?\376?\376?\376?\376?\376?\376?\376"
  "\335\355\336\365?\376?\376\275\355?\376?\376?\376\275\355?\376?\376\336\365"
  "\263\233\177\376|\345\000\000\227\274\237\376\227\274\000\000|\345\177\376\263\233",
};

uint32_t getWidthcolorleft() {return gimp_image.width;}
uint32_t getHeightcolorleft() {return gimp_image.height;}
uint32_t getBytePerPixelcolorleft() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatacolorleft() {return &gimp_image.pixel_data[0];}
