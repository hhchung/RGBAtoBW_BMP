#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "bmp.h"

void bmpLoad(BMP *bmp, const char *fileName) {
    FILE *file;
    BYTE header[14];
    BYTE info[40];
    int n;
    if ((file = fopen(fileName, "rb") ) == NULL ) {
        fprintf(stderr, "Error: bmpLoad(), %s open fail!\n", fileName);
        exit(EXIT_FAILURE);
    }

    n = fread(header, 1, HEADER_SIZE, file);
    assert(n==HEADER_SIZE);
    n = fread(info, 1, INFO_SIZE, file);
    assert(n==INFO_SIZE);
    memcpy(bmp->header, header, HEADER_SIZE);
    memcpy(bmp->info, info, INFO_SIZE);

    // Header
    bmp->signature = B2U16(header,0); assert(bmp->signature == 0x4D42);
    bmp->fileSize = B2U32(header,2);
    bmp->dataOffset = B2U32(header,10);

    // InfoHeader
    bmp->size = B2U32(info,0); assert(bmp->size==40);
    bmp->width = B2U32(info,4);
    bmp->height = B2U32(info,8);
    bmp->planes = B2U16(info,12); assert(bmp->planes==1);
    bmp->bitsPerPixel = B2U16(info,14); assert(bmp->bitsPerPixel==32);
    bmp->compression = B2U32(info,16);
    bmp->imageSize = B2U32(info,20);
    bmp->xPixelsPerM = B2U32(info,24);
    bmp->yPixelsPerM = B2U32(info,28);
    bmp->colorsUsed = B2U32(info,32);
    bmp->colorsImportant = B2U32(info,36);

    // Image data
    bmp->data = (uint32_t*) malloc( bmp->fileSize );
    fseek(file, bmp->dataOffset, SEEK_SET);
    n = fread(bmp->data, 1, bmp->fileSize - (HEADER_SIZE+INFO_SIZE), file);
    assert(n==bmp->fileSize - (HEADER_SIZE+INFO_SIZE));
    fclose (file);
}

void bmpPrint(BMP *bmp) {
    printf("==== Header ====\n");
    printf("Signature = %04X\n", bmp->signature); // 0x4d42 = BM
    printf("FileSize = %u \n", bmp->fileSize);
    printf("DataOffset = %u \n", bmp->dataOffset);
    printf("==== Info ======\n");
    printf("Info size = %u \n", bmp->size);
    printf("Width = %u \n", bmp->width);
    printf("Height = %u \n", bmp->height);
    printf("BitsPerPixel = %u \n", bmp->bitsPerPixel);
    printf("Compression = %u \n", bmp->compression);
    printf("================\n");
}

void bmpSave(BMP *bmp, const char *fileName) {
    FILE *file;

    if ((file = fopen(fileName, "wb") ) == NULL ) {
        fprintf(stderr, "Error: bmpSave(), File create fail!\n");
        exit(EXIT_FAILURE);
    }

    fwrite(bmp->header, 1, HEADER_SIZE, file);
    fwrite(bmp->info, 1, INFO_SIZE, file);
    fseek(file, bmp->dataOffset, SEEK_SET);
    fwrite(bmp->data, 1,  bmp->fileSize-(HEADER_SIZE+INFO_SIZE), file);
    fclose(file);

    printf("Save the picture successfully!\n");
}

void bmpFreeBuf(BMP *bmp)
{
    free(bmp->data);
}

void bmpCalcSNR(const char *fileName, BMP *new_bmp, int width, int height, long stride)
{
    int row, col;
    uint32_t signal_pwr, noise_pwr;
    uint32_t *ref_img_data, *new_img_data;
    uint32_t ref_pixel,  ref_r, ref_g, ref_b;
    uint32_t new_pixel,  new_r, new_g, new_b;
    BMP org_bmp;

    bmpLoad(&org_bmp, fileName);

    ref_img_data = org_bmp.data;
    new_img_data = new_bmp->data;
    signal_pwr = 0;
    noise_pwr = 0;
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            ref_pixel = ref_img_data[col + row * stride / 4];
            ref_r = (ref_pixel >> 16) & 0xff;
            ref_g = (ref_pixel >> 8) & 0xff;
            ref_b = ref_pixel & 0xff;
            signal_pwr += ((ref_r) * (ref_r) + (ref_g) * (ref_g) + (ref_b) * (ref_b));

            new_pixel = new_img_data[col + row * stride / 4];
            new_r = (new_pixel >> 16) & 0xff;
            new_g = (new_pixel >> 8) & 0xff;
            new_b = new_pixel & 0xff;
            noise_pwr += ((new_r - ref_r) * (new_r - ref_r) +
                          (new_g - ref_g) * (new_g - ref_g) + (new_b - ref_b) * (new_b - ref_b));
        }
    }
    printf("Signal Power : %u Noise Power : %u SNR : %lf dB\n", signal_pwr, noise_pwr, 10*log10(1.0*signal_pwr/noise_pwr));

    bmpFreeBuf(&org_bmp);
}

