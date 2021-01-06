//
// Created by lara on 04/01/2021.
//

#include <stddef.h>
#include "utils.h"
#include "bitmap.h"

void testCase1(){

    char filepath[] = "snail.bmp";

    FILE * bmp = fopen(filepath, "rb");

    Bitmap bitmap;
    fread(&bitmap, sizeof (Bitmap), 1, bmp);
    printf("bfType: %s\n", (char *) &bitmap.bfType);
    printf("bfSize: %d byte\n", bitmap.bfSize);
    printf("bfReserved: %d\n", bitmap.bfReserved);
    printf("bfOffBits: %d\n", bitmap.bfOffBits);
    printf("biSize: %d\n", bitmap.biSize);
    printf("biWidth: %d px\n", bitmap.biWidth);
    printf("biHeight: %d px\n", bitmap.biHeight);
    printf("biPlanes: %d\n", bitmap.biPlanes);
    printf("biBitCount: %d\n", bitmap.biBitCount);
    printf("biCompression: %d\n", bitmap.biCompression);
    printf("biSizeImage: %d\n", bitmap.biSizeImage);
    printf("biXPelsPerMeter: %d px/m\n", bitmap.biXPelsPerMeter);
    printf("biYPelsPerMeter: %d px/m\n", bitmap.biYPelsPerMeter);
    printf("biClrUsed: %d\n", bitmap.biClrUsed);
    printf("biClrImportant: %d\n", bitmap.biClrImportant);

    bitmap.colorTable = malloc(sizeof (Color) * bitmap.biClrUsed);
    fseek(bmp, offsetof(Bitmap, colorTable), SEEK_SET);
    fread(bitmap.colorTable, sizeof (Color), bitmap.biClrUsed, bmp);

    int size = 0;
    for(int i = 0; i < bitmap.biClrUsed; i++){
        Color *color = (bitmap.colorTable +i);
        //printf("b %d  g %d  r %d  a %d\n", color->blue, color->green, color->red);
        ++size;
    }
    printf("colorTable size %d", size);

    //printf("\nb %d  g %d  r %d  a %d\n", bitmap.colorTable->blue, bitmap.colorTable->green, bitmap.colorTable->red, bitmap.colorTable->alpha);

    bitmap.data = malloc(sizeof (uint8_t) * bitmap.biSizeImage);
    fseek(bmp, bitmap.bfOffBits, SEEK_SET);
    fread(bitmap.data, sizeof (uint8_t), bitmap.biSizeImage, bmp);


    for(int i = 0; i < bitmap.biSizeImage; i++){
        uint8_t *data = (bitmap.data +i);
        //printf("%d\n", *data);
    }

    FILE * result = fopen("output.bmp", "wb");
    fwrite((uint8_t *)&bitmap, 1,  sizeof (Bitmap) - sizeof (Color *) - sizeof (uint8_t *), result);
    fwrite(bitmap.colorTable, sizeof (Color), bitmap.biClrUsed, result);
    fwrite(bitmap.data, sizeof (uint8_t), bitmap.biSizeImage, result);

    fclose(bmp);
    free(bitmap.colorTable);
    free(bitmap.data);
}

void testCase2(){
    char filepath[] = "all_gray.bmp";

    FILE * bmp = fopen(filepath, "rb");

    Bitmap bitmap;

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    assert_equal(bitmap.colorTable_size, bitmap.biClrUsed, "Wrong: size of colorTable");
    assert_equal(0, bitmap.biCompression, "Wrong: compression");
    assert_equal(40, bitmap.biSize, "Wrong: header size");
    assert_equal( bitmap.bfSize, (bitmap.bfOffBits + bitmap.biSizeImage), "Wrong: image size");


    Bitmap_destroy(&bitmap);
}

void testCase3(){
    char filepath[] = "dots.bmp";

    FILE * bmp = fopen(filepath, "rb");

    Bitmap bitmap;

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    assert_equal(bitmap.colorTable_size, bitmap.biClrUsed, "Wrong: size of colorTable");
    assert_equal(0, bitmap.biCompression, "Wrong: compression");
    assert_equal(40, bitmap.biSize, "Wrong: header size");
    assert_equal( bitmap.bfSize, (bitmap.bfOffBits + bitmap.biSizeImage), "Wrong: image size");


    Bitmap_destroy(&bitmap);
}

void testCase4(){
    char filepath[] = "lena.bmp";

    FILE * bmp = fopen(filepath, "rb");

    Bitmap bitmap;

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    assert_equal(bitmap.colorTable_size, bitmap.biClrUsed, "Wrong: size of colorTable");
    assert_equal(0, bitmap.biCompression, "Wrong: compression");
    assert_equal(40, bitmap.biSize, "Wrong: header size");
    assert_equal( bitmap.bfSize, (bitmap.bfOffBits + bitmap.biSizeImage), "Wrong: image size");


    Bitmap_destroy(&bitmap);
}

void testCase5(){
    char filepath[] = "snail.bmp";

    FILE * bmp = fopen(filepath, "rb");

    Bitmap bitmap;

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(1, scanned, "Failed: scan");

    assert_equal(NULL, bitmap.colorTable, "Wrong: size of colorTable");

}

int main(){
    testCase1();
    testCase2();
    testCase3();
    testCase4();
    testCase5();
    return 0;
}