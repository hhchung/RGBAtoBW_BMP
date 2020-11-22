#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bmp.h"
#include "bw.h"
#define FILE_NAME_SZ 256

#if NEON_ASM
extern void rgbaToBw_neon(int32_t *data, int width, int height, long stride);
#endif

void showHelp(void){
    puts("rgbatobw input.bmp output.bmp");
}

int main(int argc, char *argv[]) {
    
    char openfile[FILE_NAME_SZ];
    char savefile[FILE_NAME_SZ];
    BMP *bmp = (BMP*) malloc(sizeof(BMP));
    clock_t start = 0;
    clock_t end = 0;
    int loop = 50;
    int i;

    if (argc != 3){
       showHelp();
       return -1;
    } else {
        strncpy(openfile, argv[1], FILE_NAME_SZ-1); 
        strncpy(savefile, argv[2], FILE_NAME_SZ-1);
        openfile[FILE_NAME_SZ-1] = '\0';
        savefile[FILE_NAME_SZ-1] = '\0';
        printf("Input Path: %s\n", openfile);
        printf("Output Path: %s\n", savefile);
    }

    /* Load the image and print the infomation */
    bmpLoad(bmp, openfile);
    long stride = bmp->width * 4;
    // bmpPrint(bmp);

    /* RGBA to BW */
    printf("RGBA to BW is in progress....\n");
    generateRGBTable();
    start = clock();
    for(i = 0; i < loop; i++){

#if NEON_ASM
        rgbaToBw_neon(bmp->data, bmp->width, bmp->height, stride);
#else
        rgbaToBw(bmp->data, bmp->width, bmp->height, stride);
#endif
    }
    end = clock();
    //bmpPrint(bmp); 
    bmpSave(bmp, savefile);
    bmpCalcSNR(openfile, bmp, bmp->width, bmp->height, stride);


    printf("Execution time of rgbaToBw() : %lf \n", ((double) (end - start)) / CLOCKS_PER_SEC / loop);
    bmpFreeBuf(bmp);
    free(bmp);

    return 0;
}
