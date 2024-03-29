#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (eye-down.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[11 * 9 * 2 + 1];
} gimp_image = {
  11, 9, 2,
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000a\010\000\000\000\000a\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000]\357"
  "]\357\000\000A\010iJ\377\377iJ\040\000\000\000\000\000\070\306}\357}\357\070\306\000\000\377\377"
  "\034\347\377\377\000\000\000\000\000\000\060\204\242\020\242\020\060\204\000\000\064\245\000\000\064"
  "\245\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000",
};

uint32_t getWidtheyedown() {return gimp_image.width;}
uint32_t getHeighteyedown() {return gimp_image.height;}
uint32_t getBytePerPixeleyedown() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDataeyedown() {return &gimp_image.pixel_data[0];}
