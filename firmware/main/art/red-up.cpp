#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (red-up.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[11 * 9 * 2 + 1];
} gimp_image = {
  11, 9, 2,
  "\000\000\000\000\000\000\000\000\040\321\040\321\040\321\000\000\000\000\000\000\000\000\000\000\000\000\040\240\040\311"
  "`\330@\331`\330\040\311\040\240\000\000\000\000\000\000\211\272\222\254\000\000\030\366\000\320"
  "\030\366\000\000\222\254\211\272\000\000\000\000L\343\377\377\276\367\276\377\000\320\276"
  "\377\276\367\377\377L\343\000\000\040\321@\320T\355\236\377\000\320\202\331\000\320"
  "\236\377T\355@\320\040\321\040\311@\331\000\320\000\320A\331@\331A\331\000\320\000"
  "\320@\331\040\311\040\311@\331@\331@\331@\331@\331@\331@\331@\331@\331\040\311"
  "\040\311@\331\040\311@\331@\321\000\301@\321@\331\040\311@\331\040\311\040\321\240"
  "\200@P@\331\040\321\000\000\040\321@\331@P\240\200\040\321",
};

uint32_t getWidthredup() {return gimp_image.width;}
uint32_t getHeightredup() {return gimp_image.height;}
uint32_t getBytePerPixelredup() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDataredup() {return &gimp_image.pixel_data[0];}
