#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (straberry.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[10 * 9 * 2 + 1];
} gimp_image = {
  10, 9, 2,
  "\000\000\000\000C\003`\003\271\316ml\204\003@\002\000\000\000\000\000\000\000p\204\264&\255\222\177lG\040"
  "\264\065\316\212\062\000\010\000H\070\366l\333\000\320\300\321\000\320\276\377\000\320"
  "b\331\000\000@H\000\320\000\320\\\377u\355+\333\000\320l\333\000\311\000\000\000\000\000\230\232"
  "\366\000\320\064\345\352\332\340\330q\354\000@\000\000\000\000\000\000G\322\340\320\203\331"
  "\000\320\276\367\000\000\000\010\000\000\000\000\000\010\000\000+\313\327\345\340\300\000\260A\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000`\000\020\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000",
};

uint32_t getWidthstraberry() {return gimp_image.width;}
uint32_t getHeightstraberry() {return gimp_image.height;}
uint32_t getBytePerPixelstraberry() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatastraberry() {return &gimp_image.pixel_data[0];}
