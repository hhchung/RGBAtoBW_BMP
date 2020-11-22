#ifndef __BW_H__
#define __BW_H__
#include <stdint.h>

typedef unsigned char BYTE;
void rgbaToBw(uint32_t *data, int width, int height, long stride); 
void generateRGBTable();

#endif //__BW_H__
