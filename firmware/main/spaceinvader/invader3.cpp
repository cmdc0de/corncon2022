/* GIMP RGBA C-Source image dump (invader3.c) */

#include  <stdint.h>
static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[14 * 10 * 2 + 1];
} gimp_image = {
  14, 10, 2,
  "\000\000\000\000\236\367\276\367\040\000\000\000\000\000\000\000\000\000\040\000]\357\337\377\000\000\000\000\000"
  "\000\000\000\000\000\202\020\337\377\000\000\000\000\000\000\040\000\377\377a\010\040\000\000\000\000\000\000\000\000"
  "\000\000\000\337\377\377\377\337\377\276\367\276\367\236\367\337\377\236\367\040"
  "\000\000\000\000\000\000\000\000\000\236\367\276\367\337\377\337\377\377\377\377\377\276\367"
  "\276\367\276\367\236\367\040\000\000\000\040\000\236\367\377\377\276\367E)\236\367"
  "\377\377\377\377\236\367E)\337\377\377\377\377\377\000\000]\357\337\377\377\377"
  "\337\377\337\377\377\377\377\377\377\377\377\377\337\377\337\377\377\377"
  "\337\377]\357\377\377\040\000A\010\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\242\020a\010\377\377\377\377a\010A\010]\357\040\000\040\000A\010"
  "\040\000\000\000A\010]\357a\010\040\000\377\377A\010\000\000\000\000\377\377a\010\000\000\040\000A\010"
  "a\010\242\020\377\377A\010\040\000\000\000\000\000\000\000\000\000a\010\337\377\377\377\040\000\040"
  "\000\377\377\337\377\000\000\000\000\000\000\000\000",
};

uint32_t getWidthinvader3() {return gimp_image.width;}
uint32_t getHeightinvader3() {return gimp_image.height;}
uint32_t getBytePerPixelinvader3() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatainvader3() {return &gimp_image.pixel_data[0];}