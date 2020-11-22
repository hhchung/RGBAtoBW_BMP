#include "bw.h"
#define TABLE_SIZE 256
BYTE table_R[TABLE_SIZE]; 
BYTE table_G[TABLE_SIZE];
BYTE table_B[TABLE_SIZE];


void generateRGBTable(){
    int i;
    for(i = 0; i <= 0xff; i++){
        table_R[i] = (i * 77)  / 256;
        table_G[i] = (i * 150) / 256;
        table_B[i] = (i * 29)  / 256;
    }
}

void rgbaToBw(uint32_t *pixel, int width, int height, long stride){
    int row, col;
    BYTE *r, *g, *b, bw;
    BYTE  aa;
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            r = (BYTE *) pixel + 2;
            g = (BYTE *) pixel + 1;
            b = (BYTE *) pixel;
            
	    bw = table_R[*r] + table_G[*g] + table_B[*b];
	    //bw = (*r * 0.299) + (*g * 0.587) + (*b * 0.114);      
            *r = *g = *b = bw;
            pixel++;
        }
    }
}
