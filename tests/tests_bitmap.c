//
// Created by lara on 04/01/2021.
//

#include <stddef.h>
#include "utils.h"
#include "bitmap.h"

/*
 * Test Bitmap_scan
 * Image: 8bpp with colorTable
 * Scan image and test the created bitmap on accuracy
 */
void testCase1(){
    Bitmap bitmap;

    FILE * image = fopen("all_gray.bmp", "rb");

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
 * Test Bitmap_create
 * create Bitmap and copy into file for visual test
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

/*
 * Test Bitmap_scan and Bitmap_copyIntoFile
 * Scan bitmap, copy into new file and scan the copy.
 * Compare both versions of the same bitmap image
 */
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

/*
 * Test Bitmap_naive_grayscaling_px
 * grayscale a bitmap image with 24 bpp and no colorTable
 * check bitmap data on accuracy
 */
void testCase5(){

    char filepath[] = "testCase3.bmp";

    Bitmap bitmap;

    FILE * bmp = fopen(filepath, "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    uint32_t expected_gray = (*(bitmap.data ) + *(bitmap.data + 1) + *(bitmap.data + 2)) / 3;

    Bitmap_naive_grayscaling_px(&bitmap);

    for(int i = 0; i < bitmap.data_size; i++){
        // check with the manually and programmatically computed gray-tone
        assert_equal(140, *(uint8_t *)(bitmap.data + i), "Grayscale did not work");
        assert_equal(expected_gray, *(uint8_t *)(bitmap.data + i), "Failed: gray_tone");
    }

    Bitmap_destroy(&bitmap);
    fclose(bmp);
}

/*
 * Test Bitmap_naive_grayscaling_ct
 * grayscale a bitmap image with 8 bpp and a colorTable
 * check bitmap data on accuracy
 * Expects data to not change, gray stays gray
 */
void testCase6(){

    char filepath[] = "all_gray.bmp";

    Bitmap bitmap;

    FILE * bmp = fopen(filepath, "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    uint32_t colorTable_index = *bitmap.data;

    Color * color = bitmap.colorTable + colorTable_index;

    uint32_t expected_gray = (color->blue + color->green + color->red) / 3;

    Bitmap_naive_grayscaling_ct(&bitmap);

    for(int i = 0; i < bitmap.data_size; i++){
        // check with the manually and programmatically computed gray-tone
        assert_equal(135, *(uint8_t *)(bitmap.data + i), "Grayscale did not work");
        assert_equal(expected_gray, *(uint8_t *)(bitmap.data + i), "Failed to compute gray");
    }

    Bitmap_destroy(&bitmap);
    fclose(bmp);
}

void printAllPictureInfos(){

    Bitmap bitmap;

    // open bmp picture and scan into Bitmap
    FILE * bmp = fopen("all_gray.bmp", "rb");

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
    testCase5();
    testCase6();
    //printAllPictureInfos();
    return 0;
}