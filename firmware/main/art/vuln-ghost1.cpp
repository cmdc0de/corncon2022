#include "gimp_type.h"
/* GIMP RGBA C-Source image dump (vuln-ghost1.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[11 * 9 * 2 + 1];
} gimp_image = {
  11, 9, 2,
  "\000\000\001\000\000\000H\000\275\031\274\031\232\031\000\000\000\000\000\000\000\000\000\000\003\000\274\031\336!"
  "\335!\335!\335!\377!\320\010\000\000\000\000G\000\233\031\335!\236\000]\001\335!\236\000]\001"
  "\336!\064\021\000\000H\000\336!\036\001\311\355W\254\336\000\311\355W\254\275\021\273\031"
  "\000\000\337!\275\021\276\000\254\345\067\244~\000\254\345\067\244}\001\335!V\021\276\001"
  "\231\213\362\314\037\000z\203\362\314\037\000z\203\362\314\036\000U\031\334b\324\274"
  "\035:\345\365\225\264\035:\345\365\225\264\035:\254\345\026\000\276\021<\001\335!"
  "\276\000\\\001\273!\276\000~\001\273!\376\000U\031\336!k\000\362\010\337!\065\021\000\000\377"
  "!\232\031\000\000V\021W\021",
};

uint32_t getWidthvulnghost1() {return gimp_image.width;}
uint32_t getHeightvulnghost1() {return gimp_image.height;}
uint32_t getBytePerPixelvulnghost1() {return gimp_image.bytes_per_pixel;}
const uint8_t *getPixelDatavulnghost1() {return &gimp_image.pixel_data[0];}
