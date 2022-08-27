/* GIMP RGBA C-Source image dump (invader-explosion.c) */

static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[10 * 12 * 2 + 1];
} gimp_image = {
  10, 12, 2,
  "\000\000\000\000\000\000\347\071\377\377\206\061\000\000\000\000\000\000\000\000\020\204iJ\000\000\004!\020\204"
  "\303\030\202\020\357{\206\061\000\000}\357\222\224\000\000\040\000\242\020\242\020\246\061"
  "]\357,c\202\020\202\020A\010\000\000\307\071]\357]\357Y\316\242\020\323\234}\357\000"
  "\000\040\000\216s\024\245\377\377\377\377}\357\020\204\020\204\020\204A\010\343\030"
  "\232\326\236\367\377\377\377\377\377\377\377\377,c\000\000mk\034\347e)iJ\377\377"
  "\377\377\377\377\377\377,c\000\000\307\071\020\204\020\204\024\245\377\377\377\377"
  "\377\377\377\377\226\265\020\204A\010\343\030\232\326\236\367\377\377\377\377"
  "\377\377\377\377\276\367}\357mk\034\347e)iJ\377\377\377\377\377\377\377\377"
  "mk\202\020\307\071\020\204\020\204\020\204\020\204<\347}\357\020\204\020\204\020\204"
  "\000\000A\010\034\347\030\306\000\000y\316\333\336\040\000\323\234\377\377",
};

uint32_t getWidthinvaderexplosion() {return gimp_image.width;}
uint32_t getHeightinvaderexplosion() {return gimp_image.height;}
uint32_t getBytePerPixelinvaderexplosion() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatainvaderexplosion() {return &gimp_image.pixel_data[0];}
