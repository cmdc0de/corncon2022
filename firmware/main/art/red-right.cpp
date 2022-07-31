#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (red-right.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[11 * 9 * 2 + 1];
} gimp_image = {
  11, 9, 2,
  "\000\000\000\000\000\000\000\000\040\321\040\321\040\321\000\000\000\000\000\000\000\000\000\000\000\000\340\250@\341"
  "\040\320\340\330@\331@\331\000\250\000\000\000\000\000\000\340\260a\331\000\320y\366\363\354"
  "\040\331A\331\272\376\251\272\000\000\000\000@\331\000\320]\377\276\367\000\000\000\310\377"
  "\377\070\306\000@\000\000\040\321@\331\000\320\373\366\276\367\307A\000\310\276\377y"
  "\316\003I@\321\040\311@\331A\331\000\320\326\355q\354\040\331@\331\030\366\210\342"
  "\040\311\040\311@\331@\331@\331\000\331\040\331@\331\040\331\000\331\040\331\040\311"
  "\040\311@\331\040\311@\331@\321\000\301@\321@\331\040\311@\331\040\311\040\321\240"
  "\200@P@\331\040\321\000\000\040\321@\331@P\240\200\040\321",
};

uint32_t getWidthredright() {return gimp_image.width;}
uint32_t getHeightredright() {return gimp_image.height;}
uint32_t getBytePerPixelredright() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDataredright() {return &gimp_image.pixel_data[0];}
