/* GIMP RGBA C-Source image dump (invader1a.c) */

#include <stdint.h>
static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[14 * 13 * 2 + 1];
} gimp_image = {
  14, 13, 2,
  "\000\000\000\000\000\000a\010\040\000\303\030\377\377\377\377$!\202\020\040\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\202\020\377\377\377\377\377\377\377\377\377\377\377\377\202\020"
  "\000\000\000\000\000\000\000\000a\010\040\000\337\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\337\377\202\020\040\000\000\000\000\000A\010\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\242\020\000\000A\010\236\367\377"
  "\377\276\367A\010\202\020\377\377\377\377\303\030a\010\337\377\377\377\377\377"
  "\343\030\377\377\377\377\377\377\276\367\040\000<\347\377\377\377\377\337\377"
  "\040\000\276\367\377\377\377\377\377\377\377\377\377\377\377\377\337\377\236"
  "\367\377\377\377\377\377\377\377\377\236\367\337\377\377\377\377\377\377"
  "\377\202\020\000\000\040\000\040\000\377\377\337\377\276\367\276\367\276\367\377\377"
  "\303\030a\010\040\000\303\030\000\000\000\000\000\000\242\020\377\377\246\061<\347\337\377e)"
  "}\357a\010\000\000\040\000\242\020\000\000\000\000$!\377\377\206\061\206\061]\357]\357E)\206"
  "\061\236\367\303\030\040\000\000\000\000\000\343\030\377\377\242\020\303\030\337\377\377"
  "\377\377\377}\357$!\040\000\377\377\004!\000\000\303\030\377\377A\010\307\071\236\367"
  "\004!a\010\040\000\004!\276\367\202\020\202\020\236\367\004!\377\377\070\306\000\000<\347"
  "\377\377a\010\000\000\000\000a\010\377\377\276\367\000\000\333\336\377\377",
};

uint32_t getWidthinvader1a() {return gimp_image.width;}
uint32_t getHeightinvader1a() {return gimp_image.height;}
uint32_t getBytePerPixelinvader1a() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatainvader1a() {return &gimp_image.pixel_data[0];}
