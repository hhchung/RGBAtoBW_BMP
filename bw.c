#include "bw.h"
#define TABLE_SIZE 256
#include <stdint.h>

uint32_t table_R[TABLE_SIZE]; 
uint32_t table_G[TABLE_SIZE];
uint32_t table_B[TABLE_SIZE];


void generateRGBTable(){
  
    for(unsigned int i = 0; i <= 0xff; i++){
        table_R[i] = i * 77;
        table_G[i] = i * 150;
        table_B[i] = i * 29;
    }
}

void rgbaToBw(uint32_t *pixel, int width, int height, long stride){
    int row, col;
    BYTE *r, *g, *b, bw;
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            r = (BYTE *) pixel   ;
            g = (BYTE *) pixel+1 ;
            b = (BYTE *) pixel+2 ;
	        bw = (table_R[*r] + table_G[*g] + table_B[*b])>>8;
            *r = *g = *b = bw;
            pixel++;
        }
    }
}
