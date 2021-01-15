//
// Created by lara on 04/01/2021.
//

#include <stddef.h>
#include "utils.h"
#include "bitmap.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

void byte_to_bits(uint8_t bits[], uint8_t byte){
    uint8_t mask = 1; // Bit mask

    // Extract the bits
    for (int i = 0; i < 8; i++) {
    // Mask each bit in the byte and store it
        bits[i] = (byte & (mask << i)) != 0;
    }
}

char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1) {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

void testCase1(){

    char filepath[] = "all_gray.bmp";

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
    printf("colorTable size %d\n", size);

    //printf("\nb %d  g %d  r %d  a %d\n", bitmap.colorTable->blue, bitmap.colorTable->green, bitmap.colorTable->red, bitmap.colorTable->alpha);

    bitmap.data = malloc(sizeof (uint8_t) * bitmap.biSizeImage);
    fseek(bmp, bitmap.bfOffBits, SEEK_SET);
    fread(bitmap.data, sizeof (uint8_t), bitmap.biSizeImage, bmp);

    int data_size = 0;
    for(int i = 0; i < bitmap.biSizeImage; i++){
        uint8_t *data = (bitmap.data +i);
        //printf("%d\n", *data);
        ++data_size;
    }
    printf("data_size %d\n", data_size);

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

    fclose(bmp);
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

    fclose(bmp);
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

    fclose(bmp);
    Bitmap_destroy(&bitmap);
}

void testCase6(){

    char filepath[] = "all_gray.bmp";

    Bitmap bitmap;

    // open bmp picture and scan into Bitmap
    FILE * bmp = fopen(filepath, "rb");
    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    Bitmap_print(&bitmap, filepath);

    // copy the bitmap into a new bmp picture

    FILE *copy = fopen("copy_all_gray.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(copy, &bitmap);
    assert_equal(0, copied, "Failed: copy");
    rewind(copy);


    char filepath2[] = "copy_all_gray.bmp";

    Bitmap bitmap2;

    // open bmp picture and scan into Bitmap
    FILE * bmp2 = fopen(filepath2, "rb");
    uint32_t scanned2 = Bitmap_scan(bmp2, &bitmap2);
    assert_equal(0, scanned2, "Failed: scan");

    Bitmap_print(&bitmap2, "copy_all_gray.bmp");

    fclose(bmp);
    fclose(copy);
    Bitmap_destroy(&bitmap);
    Bitmap_destroy(&bitmap2);
}

void testCase7(){
    char filepath[] = "all_gray.bmp";

    FILE * bmp = fopen(filepath, "rb");

    Bitmap bitmap;

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    Bitmap_print(&bitmap, filepath);

    // copy the bitmap into a new bmp picture

    FILE *copy = fopen("copy_all_gray.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(copy, &bitmap);
    assert_equal(0, copied, "Failed: copy");

    fclose(bmp);
    fclose(copy);
    Bitmap_destroy(&bitmap);
}

void testCase8(){
    char filepath[] = "all_gray.bmp";

    //#############################################################################
    // OPEN, SCAN, COPY
    //#############################################################################

    // open bmp picture and scan into Bitmap
    FILE * bmp = fopen(filepath, "rb");
    Bitmap bitmap;
    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    // create bpm picture and copy the scanned bitmap into the new picture
    FILE *copy = fopen("copy_all_gray.bmp", "wb");
    uint32_t copied = Bitmap_copyIntoFile(copy, &bitmap);
    assert_equal(0, copied, "Failed: copy");

    // open the file of the copy again in read-mode
    FILE *copyImage = fopen("copy_all_gray.bmp", "rb");

    //#############################################################################
    // FILE SIZE
    //#############################################################################
    fseek(bmp, 0, SEEK_END);
    uint32_t fileSize_original = ftell(bmp);
    rewind(bmp);

    fseek(copy, 0, SEEK_END);
    uint32_t fileSize_copy = ftell(copy);
    rewind(copy);

    printf("FileSize:\noriginal: %d byte\ncopy:%d byte\n\n", fileSize_original, fileSize_copy);
    assert_equal(fileSize_original, fileSize_copy, "File size of original and copy differs");

    //#############################################################################
    // READ FILE CONTENT INTO BUFFER
    //#############################################################################
    uint8_t * original = malloc(sizeof (uint8_t) * fileSize_original);
    uint32_t read_original = fread(original, sizeof (uint8_t), fileSize_original, bmp);
    assert_equal(fileSize_original, read_original, "Error: reading original file");

    uint8_t * thecopy = malloc(sizeof (uint8_t) * fileSize_copy);
    uint32_t read_copy = fread(thecopy, sizeof (uint8_t), fileSize_copy, copyImage);
    assert_equal(fileSize_copy, read_copy, "Error: reading copy file");

    //#############################################################################
    // Check file content
    //#############################################################################

    for(int i = 0; i < fileSize_original; i++){
        printf("\n%d:",i);
        printf(" "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*(original +i)));
        printf(" "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*(thecopy +i)));
    }

    //#############################################################################
    free(original);
    free(thecopy);
    fclose(bmp);
    fclose(copy);
    Bitmap_destroy(&bitmap);
}

void testCase9(){
    char filepath[] = "all_gray.bmp";

    //#############################################################################
    // OPEN, SCAN, COPY
    //#############################################################################

    // open bmp picture and scan into Bitmap
    FILE * bmp = fopen(filepath, "rb");
    Bitmap bitmap;
    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    // create bpm picture and copy the scanned bitmap into the new picture
    FILE *copy = fopen("copy_all_gray.bmp", "wb");
    uint32_t copied = Bitmap_copyIntoFile(copy, &bitmap);
    assert_equal(0, copied, "Failed: copy");

    // open the file of the copy again in read-mode
    FILE *copyImage = fopen("copy_all_gray.bmp", "rb");

    //#############################################################################
    // FILE SIZE
    //#############################################################################
    fseek(bmp, 0, SEEK_END);
    uint32_t fileSize_original = ftell(bmp);
    rewind(bmp);

    fseek(copy, 0, SEEK_END);
    uint32_t fileSize_copy = ftell(copy);
    rewind(copy);

    printf("FileSize:\noriginal: %d byte\ncopy:%d byte\n\n", fileSize_original, fileSize_copy);
    assert_equal(fileSize_original, fileSize_copy, "File size of original and copy differs");

    //#############################################################################
    // READ FILE CONTENT INTO BUFFER
    //#############################################################################
    uint8_t * original = malloc(sizeof (uint8_t) * fileSize_original);
    uint32_t read_original = fread(original, sizeof (uint8_t), fileSize_original, bmp);
    assert_equal(fileSize_original, read_original, "Error: reading original file");

    uint8_t * thecopy = malloc(sizeof (uint8_t) * fileSize_copy);
    uint32_t read_copy = fread(thecopy, sizeof (uint8_t), fileSize_copy, copyImage);
    assert_equal(fileSize_copy, read_copy, "Error: reading copy file");

    //#############################################################################
    // Check file content
    //#############################################################################

    for(int i = 0; i < fileSize_original; i++){
        printf("\n%d:",i);
        printf(" "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*(original +i)));
        printf(" "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*(thecopy +i)));
    }

    Bitmap_print(&bitmap, filepath);

    for(int i = 0; i < bitmap.biClrUsed; i++){
        printf("%d: %hhu %hhu %hhu %hhu\n", i, (bitmap.colorTable +i)->green, (bitmap.colorTable +i)->blue, (bitmap.colorTable +i)->red, (bitmap.colorTable +i)->alpha);
    }

    for(int i = 0; i < bitmap.biSizeImage; i++){
        printf("%d\n", *(bitmap.data + i));
    }

    //#############################################################################
    free(original);
    free(thecopy);
    fclose(bmp);
    fclose(copy);
    Bitmap_destroy(&bitmap);
}

/**
 * Alter the bmp all_gray.bmp (bitmap.data only)
 * and assign a different color from the colorTable
 */
void testCase10(){
    char filepath[] = "all_gray.bmp";

    Bitmap bitmap;

    // open bmp picture and scan into Bitmap
    FILE * bmp = fopen(filepath, "rb");
    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    Bitmap_print(&bitmap, filepath);

    for(int i = 0; i < bitmap.data_size; i++){
        uint8_t * pixel = (uint8_t *)(bitmap.data +i);
        *pixel = 0;
    }

    FILE *alteredPic = fopen("all_black.bmp", "wb");
    uint32_t altered = Bitmap_copyIntoFile(alteredPic, &bitmap);
    assert_equal(0, altered, "Failed: copy");

}

void testCase11(){
    char filepath[] = "all_gray.bmp";

    Bitmap bitmap;

    // open bmp picture and scan into Bitmap
    FILE * bmp = fopen(filepath, "rb");
    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    Bitmap_print(&bitmap, filepath);

    Bitmap_destroy(&bitmap);
    fclose(bmp);
}

void testCase12(){
    char filepath[] = "sample_1920_1280.bmp";

    Bitmap bitmap;

    // open bmp picture and scan into Bitmap
    FILE * bmp = fopen(filepath, "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    // create bpm picture and copy the scanned bitmap into the new picture
    FILE *copy = fopen("copy_sample_1920_1280.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(copy, &bitmap);
    assert_equal(0, copied, "Failed: copy");

    Bitmap_destroy(&bitmap);
    fclose(bmp);

}

void testCase13(){
    char filepath[] = "blackbuck.bmp";

    Bitmap bitmap;

    // open bmp picture and scan into Bitmap
    FILE * bmp = fopen(filepath, "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    // create bpm picture and copy the scanned bitmap into the new picture
    FILE *copy = fopen("copy_blackbuck.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(copy, &bitmap);
    assert_equal(0, copied, "Failed: copy");

    Bitmap_destroy(&bitmap);
    fclose(bmp);
}

void testCase14(){
    char filepath[] = "sample_640_426.bmp";

    Bitmap bitmap;

    // open bmp picture and scan into Bitmap
    FILE * bmp = fopen(filepath, "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    // create bpm picture and copy the scanned bitmap into the new picture
    FILE *copy = fopen("copy_sample_640_426.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(copy, &bitmap);
    assert_equal(0, copied, "Failed: copy");

    Bitmap_destroy(&bitmap);
    fclose(bmp);
}

void testCase15(){
    char filepath[] = "trial.bmp";

    Bitmap bitmap;

    FILE * bmp = fopen(filepath, "wb");

    Bitmap_create(&bitmap, bmp);

    fclose(bmp);
}

void testCase16(){

    char filepath[] = "trial.bmp";

    Bitmap bitmap;

    FILE * bmp = fopen(filepath, "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    Bitmap_naive_grayscaling_px(&bitmap);

    for(int i = 0; i < bitmap.data_size; i++){
        assert_equal(181, *(uint8_t *)(bitmap.data + i), "Grayscale did not work");
    }

    // create bpm picture and copy the scanned bitmap into the new picture
    FILE *copy = fopen("gray_trial.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(copy, &bitmap);
    assert_equal(0, copied, "Failed: copy");

    Bitmap_destroy(&bitmap);
    fclose(bmp);
    fclose(copy);
}

void testCase17(){

    char filepath[] = "blackbuck.bmp";

    Bitmap bitmap;

    FILE * bmp = fopen(filepath, "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    Bitmap_naive_grayscaling_px(&bitmap);

    // create bpm picture and copy the scanned bitmap into the new picture
    FILE *copy = fopen("gray_blackbuck.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(copy, &bitmap);
    assert_equal(0, copied, "Failed: copy");

    Bitmap_destroy(&bitmap);
    fclose(bmp);
    fclose(copy);
}

void testCase18(){

    char filepath[] = "greenland_grid_velo.bmp";

    Bitmap bitmap;

    FILE * bmp = fopen(filepath, "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    Bitmap_naive_grayscaling_px(&bitmap);

    // create bpm picture and copy the scanned bitmap into the new picture
    FILE *copy = fopen("gray_greenland_grid_velo.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(copy, &bitmap);
    assert_equal(0, copied, "Failed: copy");

    Bitmap_destroy(&bitmap);
    fclose(bmp);
    fclose(copy);
}

void testCase19(){
    char filepath[] = "dots.bmp";

    Bitmap bitmap;

    FILE * bmp = fopen(filepath, "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    Bitmap_naive_grayscaling_ct(&bitmap);

    FILE *copy = fopen("gray_dots.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(copy, &bitmap);
    assert_equal(0, copied, "Failed: copy");
}

void testCase20(){
    char filepath[] = "lena.bmp";

    Bitmap bitmap;

    FILE * bmp = fopen(filepath, "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    Bitmap_naive_grayscaling_ct(&bitmap);

    FILE *copy = fopen("gray_lena.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(copy, &bitmap);
    assert_equal(0, copied, "Failed: copy");
}

void printAllPictureInfos(){
    char * filepaths[7] = { "sample_640_426.bmp","all_gray.bmp", "dots.bmp", "lena.bmp", "greenland_grid_velo.bmp", "blackbuck.bmp", "trial.bmp"};

    for(int i = 0; i < 7; i++){
        Bitmap bitmap;

        // open bmp picture and scan into Bitmap
        FILE * bmp = fopen(filepaths[i], "rb");

        uint32_t scanned = Bitmap_scan(bmp, &bitmap);
        assert_equal(0, scanned, "Failed: scan");

        printf("\n---#################---\n");
        Bitmap_print(&bitmap, filepaths[i]);

        Bitmap_destroy(&bitmap);
        fclose(bmp);
    }
}




int main(){
    /*
    testCase1();
    testCase2();
    testCase3();
    testCase4();
    testCase6();
    testCase7();
    testCase8();
    testCase9();
    testCase10();
    testCase11();
    testCase12();
     */
    //testCase13();
    //testCase14();
    //testCase15();
    //testCase16();
    //testCase17();
    //testCase18();
    //testCase19();
    testCase20();
    //printAllPictureInfos();
    return 0;
}