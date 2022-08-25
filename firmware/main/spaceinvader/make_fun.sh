#!/bin/bash

INCLUDE_FILE="sprits.h"

echo "pragma once" > $INCLUDE_FILE

ls *.cpp | while read line
do
	VAR=`echo $line | sed -e 's/\.cpp//'`
	VAR=`echo $VAR | sed -e 's/-//'`

	echo "uint32_t getWidth$VAR();" >> $INCLUDE_FILE
	echo "uint32_t getHeight$VAR();" >> $INCLUDE_FILE
	echo "uint32_t getBytePerPixel$VAR();" >> $INCLUDE_FILE
	echo "const uint8_t *getPixelData$VAR();" >> $INCLUDE_FILE

	echo "uint32_t getWidth$VAR() {return gimp_image.width;}"  >> $line
	echo "uint32_t getHeight$VAR() {return gimp_image.height;}" >> $line
	echo "uint32_t getBytePerPixel$VAR() {return gimp_image.bytes_per_pixel;}" >> $line
	echo "const uint8_t *getPixelData$VAR() {return &gimp_image.pixel_data[0];}" >> $line
done

