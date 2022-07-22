#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (color3-right.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[11 * 9 * 2 + 1];
} gimp_image = {
  11, 9, 2,
  "\000\000\000\000\000\000\000\000\307\374\246\364\307\374\000\000\000\000\000\000\000\000\000\000\000\000\345\323"
  "\347\374\244\374\245\374\307\374\347\374\300\323\000\000\000\000\000\000\005\324\347\374"
  "\040\374\032\377T\376\306\374\307\374|\377\253\344\000\000\000\000\347\374`\374\235"
  "\377\236\367\000\000\040\354\377\377\030\306\300\070\000\000\246\364\307\374\200\374"
  ";\377\277\367\211Z@\364\276\377\232\316%j\247\374f\354\307\374\307\374`\374"
  "v\376\321\375\306\374\307\374\227\376\012\375f\354f\354\307\374\307\374\307"
  "\374\306\374\306\374\307\374\307\374\306\374\307\374f\354\206\364\307\374"
  "\307\374F\344\307\374\307\374\307\374F\344\307\374\307\374\206\364\243\222"
  "\007\375&\334\000\000d\273\007\375d\273\000\000&\334\007\375\243\222",
};
