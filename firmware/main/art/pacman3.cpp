#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (pacman3.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[10 * 9 * 2 + 1];
} gimp_image = {
  10, 9, 2,
  "\000\000\000\000\000\000F\335\207\345\207\345\347\365\000\000\000\000\000\000\000\000#\203(\376\347\365"
  "\347\365\347\355@\061\040\010\000\000\000\000\000\000(\376\347\365\347\365\347\365\202j"
  "\000\000\000\000\000\000\000\000F\335\347\365\347\365'\376\000)\000\000\000\000\000\000\000\000\000\000\207\345"
  "\347\365\007\366\303r\000\000\000\000\000\000\000\000\000\000\000\000\304\233\347\365\347\365\007\376"
  "\004\244\000\000\000\000\000\000\000\000\000\000\000\000'\376\347\365\347\365\347\365\344\243\000\000\000"
  "\000\000\000\000\000\040\010@\061\207\345\347\365\347\365'\376C\213\000\000\000\000\000\000\000\000\000"
  "\000\000\000\344\233%\254$\254e\264\000\000\000\000\000\000",
};

uint32_t getWidthpacman3() {return gimp_image.width;}
uint32_t getHeightpacman3() {return gimp_image.height;}
uint32_t getBytePerPixelpacman3() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatapacman3() {return &gimp_image.pixel_data[0];}
