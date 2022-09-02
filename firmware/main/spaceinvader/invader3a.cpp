/* GIMP RGBA C-Source image dump (invader3a.c) */

#include <stdint.h>

static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[14 * 10 * 2 + 1];
} gimp_image = {
  14, 10, 2,
  "\000\000\000\000\337\377\232\326\000\000\000\000\000\000\000\000\000\000\040\000]\357\377\377\000\000\000\000\040"
  "\000\000\000\000\000\337\377}\357\040\000\000\000\000\000\040\000\337\377\377\377\000\000a\010\070\306"
  "\377\377\000\000\000\000\040\000\337\377\242\020a\010\040\000A\010\276\367\000\000\000\000\000\000\377"
  "\377\377\377\000\000A\010\276\367\377\377\276\367\377\377\377\377\276\367\377"
  "\377\276\367\040\000\000\000\377\377\377\377\377\377\377\377\337\377E)\236\367\377"
  "\377\377\377\236\367E)\276\367\377\377\377\377\377\377\377\377\377\377\377"
  "\377\337\377\337\377\377\377\377\377\377\377\377\377\276\367\377\377\377"
  "\377\377\377\377\377\000\000a\010\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\040\000\000\000\000\000a\010\276\367\337\377}\357"
  "A\010a\010\000\000\040\000\236\367\337\377\337\377\040\000\000\000\000\000a\010\276\367\276\367"
  "\040\000\000\000\000\000\000\000\000\000\040\000\337\377\276\367\040\000\000\000\000\000\276\367\337\377\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\236\367\272\326\000\000",
};

uint32_t getWidthinvader3a() {return gimp_image.width;}
uint32_t getHeightinvader3a() {return gimp_image.height;}
uint32_t getBytePerPixelinvader3a() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatainvader3a() {return &gimp_image.pixel_data[0];}