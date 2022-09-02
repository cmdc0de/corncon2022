/* GIMP RGBA C-Source image dump (invader1.c) */

#include <stdint.h>
static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[14 * 13 * 2 + 1];
} gimp_image = {
  14, 13, 2,
  "\000\000\000\000\000\000\000\000\000\000A\010\377\377\377\377A\010A\010A\010\000\000\000\000\000\000\000\000\000\000"
  "\000\000\202\020\377\377\377\377\377\377\377\377\377\377\377\377a\010\000\000\000\000\000"
  "\000\000\000A\010A\010\337\377\377\377\377\377\377\377\377\377\377\377\377\377\236"
  "\367\040\000\040\000\000\000\000\000a\010\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\000\000\000\000a\010A\010\377\377}\357A\010\303\030"
  "\377\377\377\377\000\000\040\000\276\367\377\377a\010\040\000\202\020\377\377\377\377"
  "\337\377\000\000}\357\377\377\377\377\276\367\040\000\337\377\377\377\377\377\000"
  "\000\377\377\377\377\377\377\337\377\236\367\377\377\377\377\377\377\377\377"
  "\276\367\337\377\377\377\377\377\377\377\040\000\337\377\377\377\337\377\337"
  "\377\236\367\377\377\377\377\236\367\337\377\276\367\377\377\337\377\000\000"
  "\000\000A\010\377\377A\010\000\000a\010\377\377\377\377\040\000\000\000A\010\377\377A\010\000\000"
  "\040\000\202\020\337\377\000\000\000\000\040\000a\010\202\020\000\000\000\000\040\000\377\377\202\020"
  "A\010A\010\337\377\307\071A\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\307\071\377\377\343"
  "\030\000\000\202\020\377\377\337\377\000\000\000\000\000\000\000\000\000\000\040\000\276\367\377\377a"
  "\010\040\000\000\000\040\000A\010\276\367\337\377A\010\000\000\000\000\000\000\377\377\377\377\000\000"
  "\000\000\000\000",
};

uint32_t getWidthinvader1() {return gimp_image.width;}
uint32_t getHeightinvader1() {return gimp_image.height;}
uint32_t getBytePerPixelinvader1() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatainvader1() {return &gimp_image.pixel_data[0];}