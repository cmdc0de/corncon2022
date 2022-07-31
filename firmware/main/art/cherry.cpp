#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (cherry.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[10 * 9 * 2 + 1];
} gimp_image = {
  10, 9, 2,
  "\000\000\000\000\000\000\000\000\000\000\000\000c\202\002j\247\374\246\374\000\000\000\000\000\000\000\000\243\222"
  "\005\324$\253F\344d\263\343\232\000\000\300\230@\321\341\331\245\273\000\071aQ\245"
  "\303\000\000\000\000\000Pl\343\002\321F\364\000\320\000h\307\374\000@\000\010\000\000\000P\017\354\333"
  "\366\000\310\341\220\000\320$\343\000\331\000\271\000\000\000\000\240\240G\332\000\311\000\220"
  "\276\377\000\320@\331@\331\000\000\000\000\000\000\000\000\000\000\341\210\000\320]\377\000\330\040"
  "\311\000\000\000\000\000\000\000\000\000\000\000\000\301\200\000\210\341\220\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000",
};

uint32_t getWidthcherry() {return gimp_image.width;}
uint32_t getHeightcherry() {return gimp_image.height;}
uint32_t getBytePerPixelcherry() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatacherry() {return &gimp_image.pixel_data[0];}
