//
// Created by lara on 04/01/2021.
//

#include <stddef.h>
#include "utils.h"
#include "bitmap.h"

void hide(){
/*
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

void testCase21(){
    char filepath[] = "sample_640_426.bmp";

    Bitmap bitmap;

    FILE * bmp = fopen(filepath, "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    Bitmap_naive_grayscaling_px(&bitmap);

    FILE *copy = fopen("gray_sample_640_426.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(copy, &bitmap);
    assert_equal(0, copied, "Failed: copy");
}

void testCase22(){
    char filepath[] = "bmp_24.bmp";

    Bitmap bitmap;

    FILE * bmp = fopen(filepath, "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    Bitmap_naive_grayscaling_px(&bitmap);

    FILE *copy = fopen("gray_bmp_24.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(copy, &bitmap);
    assert_equal(0, copied, "Failed: copy");
}

void printAllPictureInfos(){
    char * filepaths[8] = { "bmp_24.bmp","sample_640_426.bmp","all_gray.bmp", "dots.bmp", "lena.bmp", "greenland_grid_velo.bmp", "blackbuck.bmp", "trial.bmp"};

    for(int i = 0; i < 8; i++){
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

void testCase25(){

    Bitmap bitmap;

    char filepath[] = "circle.bmp";
    FILE * bmp = fopen(filepath, "wb");

    Point origin;
    origin.x_posn = 4;
    origin.y_posn = 4;

    uint32_t drawn = Bitmap_draw_circle(&bitmap, bmp, &origin, 3, 8, 8);
}

void testCase26(){

    Bitmap bitmap;

    char filepath[] = "circle_2.bmp";
    FILE * bmp = fopen(filepath, "wb");

    Point origin;
    origin.x_posn = 8;
    origin.y_posn = 8;

    uint32_t drawn = Bitmap_draw_circle(&bitmap, bmp, &origin, 4, 16, 16);
}

void testCase28(){

    Bitmap bitmap;

    char filepath[] = "circle_3.bmp";
    FILE * bmp = fopen(filepath, "wb");

    Point origin;
    origin.x_posn = 20;
    origin.y_posn = 20;

    uint32_t drawn = Bitmap_draw_circle(&bitmap, bmp, &origin, 50, 200, 200);
}

void testCase29(){
    Bitmap bitmap;

    char filepath[] = "triangle_1.bmp";
    FILE * bmp = fopen(filepath, "wb");

    Point A;
    A.x_posn = 10;
    A.y_posn = 10;

    Point B;
    B.x_posn = 90;
    B.y_posn = 190;

    Point C;
    C.x_posn = 190;
    C.y_posn = 10;

    uint32_t drawn = Bitmap_draw_triangle(&bitmap, bmp, &A, &B, &C, 200, 200);
}

void testCase30(){
    Bitmap bitmap;

    char filepath[] = "triangle_2.bmp";
    FILE * bmp = fopen(filepath, "wb");

    Point A;
    A.x_posn = 10;
    A.y_posn = 10;

    Point B;
    B.x_posn = 30;
    B.y_posn = 190;

    Point C;
    C.x_posn = 190;
    C.y_posn = 70;

    uint32_t drawn = Bitmap_draw_triangle(&bitmap, bmp, &A, &B, &C, 200, 200);
}

void testCase31(){
    Bitmap bitmap;

    char filepath[] = "triangle_3.bmp";
    FILE * bmp = fopen(filepath, "wb");

    Point A;
    A.x_posn = -20;
    A.y_posn = -20;

    Point B;
    B.x_posn = 30;
    B.y_posn = 190;

    Point C;
    C.x_posn = 190;
    C.y_posn = 70;

    uint32_t drawn = Bitmap_draw_triangle(&bitmap, bmp, &A, &B, &C, 200, 200);
}

void testCase32(){
    Bitmap bitmap;

    char filepath[] = "create_layer.bmp";
    FILE * bmp = fopen(filepath, "wb");

    uint32_t width = 8;
    uint32_t height = 8;

    uint8_t bgr[] = {222,128,193};

    uint32_t create = Bitmap_create(&bitmap, bmp, bgr, width, height);

}

 */
}

/*
 * Image: 8bpp with colorTable
 * Scan image and test the created bitmap on accuracy
 */
void testCase1(){
    Bitmap bitmap;

    FILE * image = fopen("./samples/all_gray.bmp", "rb");

    uint32_t scanned = Bitmap_scan(image, &bitmap);
    assert_equal(0, scanned, "Failed: Bitmap_scan - testCase1");

    // check the image specific infos
    assert_equal(1094, bitmap.bfSize, "Wrong: bfSize");
    assert_equal(1078, bitmap.bfOffBits, "Wrong: bfOffBits");
    assert_equal(40, bitmap.biSize, "Wrong: biSize");
    assert_equal(8, bitmap.biWidth, "Wrong: biWidth");
    assert_equal(2, bitmap.biHeight, "Wrong: biHeight");
    assert_equal(8, bitmap.biBitCount, "Wrong: biBitCount");
    assert_equal(16, bitmap.biSizeImage, "Wrong: biSizeImage");
    assert_equal(16, bitmap.data_size, "Wrong: data_size");
    assert_equal(256, bitmap.biClrUsed, "Wrong: biClrUsed");
    assert_equal(256, bitmap.biClrImportant, "Wrong: biClrImportant");
    assert_equal(256, bitmap.colorTable_size, "Wrong: colorTable_size");

    // check the standardized infos
    assert_equal(0, bitmap.biCompression, "Wrong: biCompression");
    assert_equal(1, bitmap.biPlanes, "Wrong: biPlanes");
    assert_equal(0, bitmap.biXPelsPerMeter, "Wrong: biXPelsPerMeter");
    assert_equal(0, bitmap.biYPelsPerMeter, "Wrong: biYPelsPerMeter");
    assert_equal(0, bitmap.bfReserved, "Wrong: bfReserved");

    // check the colorTable entries
    // for this specific bitmap image the colorTables entries consist of the index/position for all three color channels
    for(int i = 0; i < bitmap.colorTable_size; i++){
        assert_equal(i, (bitmap.colorTable +i)->red, "Wrong: colorTable at index %d color red", i);
        assert_equal(i, (bitmap.colorTable +i)->green, "Wrong: colorTable at index %d color green", i);
        assert_equal(i, (bitmap.colorTable +i)->blue, "Wrong: colorTable at index %d color blue", i);
    }

    // check the pixel data
    // for this specific image all of them point to colorTable entry 135
    for(int i = 0; i < bitmap.data_size; i++){
        assert_equal(135, *(bitmap.data +i), "Wrong: data at index %d", i);
    }

    fclose(image);
    Bitmap_destroy(&bitmap);
}

/*
 * Test Bitmap_create
 */
void testCase2(){
    Bitmap bitmap;

    RGB mint_green;
    mint_green.red = 211;
    mint_green.green = 255;
    mint_green.blue = 206;

    uint32_t created = Bitmap_create(&bitmap, &mint_green, 8, 2);
    assert_equal(0, created, "Failed: Bitmap_create - testCase2");

    // check the image specific infos
    assert_equal(102, bitmap.bfSize, "Wrong: bfSize");
    assert_equal(54, bitmap.bfOffBits, "Wrong: bfOffBits");
    assert_equal(40, bitmap.biSize, "Wrong: biSize");
    assert_equal(8, bitmap.biWidth, "Wrong: biWidth");
    assert_equal(2, bitmap.biHeight, "Wrong: biHeight");
    assert_equal(24, bitmap.biBitCount, "Wrong: biBitCount");
    assert_equal(48, bitmap.biSizeImage, "Wrong: biSizeImage");
    assert_equal(48, bitmap.data_size, "Wrong: data_size");
    assert_equal(0, bitmap.biClrUsed, "Wrong: biClrUsed");
    assert_equal(0, bitmap.biClrImportant, "Wrong: biClrImportant");
    assert_equal(0, bitmap.colorTable_size, "Wrong: colorTable_size");
    assert_equal(NULL, bitmap.colorTable, "Wrong: colorTable");

    // check the standardized infos
    assert_equal(0, bitmap.biCompression, "Wrong: biCompression");
    assert_equal(1, bitmap.biPlanes, "Wrong: biPlanes");
    assert_equal(0, bitmap.biXPelsPerMeter, "Wrong: biXPelsPerMeter");
    assert_equal(0, bitmap.biYPelsPerMeter, "Wrong: biYPelsPerMeter");
    assert_equal(0, bitmap.bfReserved, "Wrong: bfReserved");

    for(int i = 0; i < (bitmap.data_size / sizeof (RGB)); i+=3){
        assert_equal(*(bitmap.data + i), mint_green.blue, "Wrong: data");
        assert_equal(*(bitmap.data + i +1), mint_green.green, "Wrong: data");
        assert_equal(*(bitmap.data + i +2), mint_green.red, "Wrong: data");
    }

    Bitmap_destroy(&bitmap);
}

/*
 * create Bitmap and copy into file
 */
void testCase3(){
    Bitmap bitmap;

    RGB orange;
    orange.red = 255;
    orange.green = 165;
    orange.blue = 0;

    uint32_t created = Bitmap_create(&bitmap, &orange, 8, 2);
    assert_equal(0, created, "Failed: Bitmap_create - testCase2");

    FILE * dest = fopen("testCase3.bmp", "wb");
    uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
    assert_equal(0, copied, "Failed: Bitmap_copyIntoFile  - testCase2");

    fclose(dest);
    Bitmap_destroy(&bitmap);
}

void testCase4(){
    Bitmap bitmap;

    FILE * image = fopen("testCase3.bmp", "rb");
    uint32_t scanned = Bitmap_scan(image, &bitmap);
    assert_equal(0, scanned, "Failed: Bitmap_scan - testCase1");

    FILE * dest = fopen("testCase3_copy.bmp", "wb");
    uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
    assert_equal(0, copied, "Failed: Bitmap_copyIntoFile  - testCase2");
    rewind(dest);

    Bitmap copy;

    FILE * bmp_copy = fopen("testCase3_copy.bmp", "rb");
    uint32_t scanned_copy = Bitmap_scan(bmp_copy, &copy);
    assert_equal(0, scanned_copy, "Failed: Bitmap_scan - testCase1");

    assert_equal(copy.bfSize, bitmap.bfSize, "Wrong: bfSize");
    assert_equal(copy.bfOffBits, bitmap.bfOffBits, "Wrong: bfOffBits");
    assert_equal(copy.biSize, bitmap.biSize, "Wrong: biSize");
    assert_equal(copy.biWidth, bitmap.biWidth, "Wrong: biWidth");
    assert_equal(copy.biHeight, bitmap.biHeight, "Wrong: biHeight");
    assert_equal(copy.biBitCount, bitmap.biBitCount, "Wrong: biBitCount");
    assert_equal(copy.biSizeImage, bitmap.biSizeImage, "Wrong: biSizeImage");
    assert_equal(copy.data_size, bitmap.data_size, "Wrong: data_size");
    assert_equal(copy.biClrUsed, bitmap.biClrUsed, "Wrong: biClrUsed");
    assert_equal(copy.biClrImportant, bitmap.biClrImportant, "Wrong: biClrImportant");
    assert_equal(copy.colorTable_size, bitmap.colorTable_size, "Wrong: colorTable_size");

    // check the standardized infos
    assert_equal(copy.biCompression, bitmap.biCompression, "Wrong: biCompression");
    assert_equal(copy.biPlanes, bitmap.biPlanes, "Wrong: biPlanes");
    assert_equal(copy.biXPelsPerMeter, bitmap.biXPelsPerMeter, "Wrong: biXPelsPerMeter");
    assert_equal(copy.biYPelsPerMeter, bitmap.biYPelsPerMeter, "Wrong: biYPelsPerMeter");
    assert_equal(copy.bfReserved, bitmap.bfReserved, "Wrong: bfReserved");

    for(int i = 0; i < bitmap.data_size; i++){
        assert_equal(*(copy.data +i), *(bitmap.data +i), "Wrong: data at index %d", i);
    }

    fclose(image);
    Bitmap_destroy(&bitmap);
}

void printAllPictureInfos(){

    Bitmap bitmap;

    // open bmp picture and scan into Bitmap
    FILE * bmp = fopen("./samples/pink.bmp", "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    Bitmap_print(&bitmap);

    Bitmap_destroy(&bitmap);
    fclose(bmp);

}

int main(){
    testCase1();
    testCase2();
    testCase3();
    testCase4();
    //printAllPictureInfos();
    return 0;
}