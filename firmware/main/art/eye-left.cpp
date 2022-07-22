#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (eye-left.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[10 * 9 * 2 + 1];
} gimp_image = {
  10, 9, 2,
  "\000\000\000\000\000\000\040\000\000\000\040\000\000\000\000\000\000\000\000\000\000\000\323\234\232\326\000\000a\010\000"
  "\000\272\326Q\214\000\000\000\000\000\000\000\000\236\367]\357\000\000\000\000\317{\377\377\353Z\000"
  "\000\000\000\347\071\337\377\333\336\000\000\000\000\323\234\377\377\252R\000\000\000\000\060\204"
  "\266\265\000\000A\010\000\000\327\275\256s\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000",
};
