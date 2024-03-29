#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (eye-up.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[10 * 9 * 2 + 1];
} gimp_image = {
  10, 9, 2,
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\353Z\202\020\202\020\353Z\000\000"
  "\256s\000\000\256s\000\000\000\000Y\316\272\326\272\326Y\316\000\000\377\377\030\306\377\377"
  "\000\000\000\000\000\000\276\367\276\367\000\000\000\000\256s\377\377\256s\000\000\000\000a\010\000\000\000"
  "\000a\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000",
};

uint32_t getWidtheyeup() {return gimp_image.width;}
uint32_t getHeighteyeup() {return gimp_image.height;}
uint32_t getBytePerPixeleyeup() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDataeyeup() {return &gimp_image.pixel_data[0];}
