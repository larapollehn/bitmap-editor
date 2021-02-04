//
// Created by lara on 04/01/2021.
//

#include <stddef.h>
#include "utils.h"
#include "bitmap.h"

/*
 * Test Bitmap_create
 */
void testCase2() {
    Bitmap bitmap;

    RGB mint_green;
    mint_green.red = 211;
    mint_green.green = 255;
    mint_green.blue = 206;

    uint32_t created = Bitmap_create(&bitmap, &mint_green, 8, 2);
    assert_equal(0, created, "Failed: Bitmap_create - testCase2")

    // check the image specific infos
    assert_equal(102, bitmap.bfSize, "Wrong: bfSize")
    assert_equal(54, bitmap.bfOffBits, "Wrong: bfOffBits")
    assert_equal(40, bitmap.biSize, "Wrong: biSize")
    assert_equal(8, bitmap.biWidth, "Wrong: biWidth")
    assert_equal(2, bitmap.biHeight, "Wrong: biHeight")
    assert_equal(24, bitmap.biBitCount, "Wrong: biBitCount")
    assert_equal(48, bitmap.biSizeImage, "Wrong: biSizeImage")
    assert_equal(48, bitmap.data_size, "Wrong: data_size")
    assert_equal(0, bitmap.biClrUsed, "Wrong: biClrUsed")
    assert_equal(0, bitmap.biClrImportant, "Wrong: biClrImportant")
    assert_equal(0, bitmap.colorTable_size, "Wrong: colorTable_size")
    assert_equal(NULL, bitmap.colorTable, "Wrong: colorTable")

    // check the standardized infos
    assert_equal(0, bitmap.biCompression, "Wrong: biCompression")
    assert_equal(1, bitmap.biPlanes, "Wrong: biPlanes")
    assert_equal(0, bitmap.biXPelsPerMeter, "Wrong: biXPelsPerMeter")
    assert_equal(0, bitmap.biYPelsPerMeter, "Wrong: biYPelsPerMeter")
    assert_equal(0, bitmap.bfReserved, "Wrong: bfReserved")

    for (int i = 0; i < (bitmap.data_size / sizeof(RGB)); i += 3) {
        assert_equal(*(bitmap.data + i), mint_green.blue, "Wrong: data")
        assert_equal(*(bitmap.data + i + 1), mint_green.green, "Wrong: data")
        assert_equal(*(bitmap.data + i + 2), mint_green.red, "Wrong: data")
    }

    Bitmap_destroy(&bitmap);
}

/*
 * Test Bitmap_create
 * create Bitmap and copy into file for visual test
 */
void testCase3() {
    Bitmap bitmap;

    RGB orange;
    orange.red = 255;
    orange.green = 165;
    orange.blue = 0;

    uint32_t created = Bitmap_create(&bitmap, &orange, 8, 2);
    assert_equal(0, created, "Failed: Bitmap_create - testCase2")

    FILE *dest = fopen("testCase3.bmp", "wb");
    uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
    assert_equal(0, copied, "Failed: Bitmap_copyIntoFile  - testCase2")

    fclose(dest);
    Bitmap_destroy(&bitmap);
}

/*
 * Test Bitmap_scan and Bitmap_copyIntoFile
 * Scan bitmap, copy into new file and scan the copy.
 * Compare both versions of the same bitmap image
 */
void testCase4() {
    Bitmap bitmap;

    FILE *image = fopen("testCase3.bmp", "rb");
    uint32_t scanned = Bitmap_scan(image, &bitmap);
    assert_equal(0, scanned, "Failed: Bitmap_scan - testCase1")

    FILE *dest = fopen("testCase3_copy.bmp", "wb");
    uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
    assert_equal(0, copied, "Failed: Bitmap_copyIntoFile  - testCase2")
    rewind(dest);

    Bitmap copy;

    FILE *bmp_copy = fopen("testCase3_copy.bmp", "rb");
    uint32_t scanned_copy = Bitmap_scan(bmp_copy, &copy);
    assert_equal(0, scanned_copy, "Failed: Bitmap_scan - testCase1")

    assert_equal(copy.bfSize, bitmap.bfSize, "Wrong: bfSize")
    assert_equal(copy.bfOffBits, bitmap.bfOffBits, "Wrong: bfOffBits")
    assert_equal(copy.biSize, bitmap.biSize, "Wrong: biSize")
    assert_equal(copy.biWidth, bitmap.biWidth, "Wrong: biWidth")
    assert_equal(copy.biHeight, bitmap.biHeight, "Wrong: biHeight")
    assert_equal(copy.biBitCount, bitmap.biBitCount, "Wrong: biBitCount")
    assert_equal(copy.biSizeImage, bitmap.biSizeImage, "Wrong: biSizeImage")
    assert_equal(copy.data_size, bitmap.data_size, "Wrong: data_size")
    assert_equal(copy.biClrUsed, bitmap.biClrUsed, "Wrong: biClrUsed")
    assert_equal(copy.biClrImportant, bitmap.biClrImportant, "Wrong: biClrImportant")
    assert_equal(copy.colorTable_size, bitmap.colorTable_size, "Wrong: colorTable_size")

    // check the standardized infos
    assert_equal(copy.biCompression, bitmap.biCompression, "Wrong: biCompression")
    assert_equal(copy.biPlanes, bitmap.biPlanes, "Wrong: biPlanes")
    assert_equal(copy.biXPelsPerMeter, bitmap.biXPelsPerMeter, "Wrong: biXPelsPerMeter")
    assert_equal(copy.biYPelsPerMeter, bitmap.biYPelsPerMeter, "Wrong: biYPelsPerMeter")
    assert_equal(copy.bfReserved, bitmap.bfReserved, "Wrong: bfReserved")

    for (int i = 0; i < bitmap.data_size; i++) {
        assert_equal(*(copy.data + i), *(bitmap.data + i), "Wrong: data at index %d", i)
    }

    fclose(image);
    Bitmap_destroy(&bitmap);
}

/*
 * Test Bitmap_naive_grayscaling_px
 * grayscale a bitmap image with 24 bpp and no colorTable
 * check bitmap data on accuracy
 */
void testCase5() {

    char filepath[] = "testCase3.bmp";

    Bitmap bitmap;

    FILE *bmp = fopen(filepath, "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan")

    uint32_t expected_gray = (*(bitmap.data) + *(bitmap.data + 1) + *(bitmap.data + 2)) / 3;

    Bitmap_naive_grayscaling_px(&bitmap);

    for (int i = 0; i < bitmap.data_size; i++) {
        // check with the manually and programmatically computed gray-tone
        assert_equal(140, *(uint8_t *) (bitmap.data + i), "Grayscale did not work")
        assert_equal(expected_gray, *(uint8_t *) (bitmap.data + i), "Failed: gray_tone")
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
void testCase6() {

    char filepath[] = "all_gray.bmp";

    Bitmap bitmap;

    FILE *bmp = fopen(filepath, "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan")

    uint32_t colorTable_index = *bitmap.data;

    Color *color = bitmap.colorTable + colorTable_index;

    uint32_t expected_gray = (color->blue + color->green + color->red) / 3;

    Bitmap_naive_grayscaling_ct(&bitmap);

    for (int i = 0; i < bitmap.data_size; i++) {
        // check with the manually and programmatically computed gray-tone
        assert_equal(135, *(uint8_t *) (bitmap.data + i), "Grayscale did not work")
        assert_equal(expected_gray, *(uint8_t *) (bitmap.data + i), "Failed to compute gray")
    }

    Bitmap_destroy(&bitmap);
    fclose(bmp);
}

/*
 * Test Bitmap_create and Bitmap_draw_triangle
 * create a unicolored bitmap image and draw a triangle in it
 */
void testCase7() {

    // create Bitmap
    Bitmap bitmap;

    RGB orange;
    orange.red = 255;
    orange.green = 165;
    orange.blue = 0;

    uint32_t created = Bitmap_create(&bitmap, &orange, 400, 400);
    assert_equal(0, created, "Failed: Bitmap_create - testCase7")

    // draw triangle in the newly created Bitmap
    RGB mint_green;
    mint_green.red = 211;
    mint_green.green = 255;
    mint_green.blue = 206;

    Point A;
    A.x_posn = 25;
    A.y_posn = 344;

    Point B;
    B.x_posn = 390;
    B.y_posn = 312;

    Point C;
    C.x_posn = 200;
    C.y_posn = 7;

    uint32_t drawn = Bitmap_draw_triangle(&bitmap, &A, &B, &C, &mint_green);
    assert_equal(0, drawn, "Failed: Bitmap_draw_triangle() - testCase 7")

    //copy the bitmap into a file

    FILE *dest = fopen("testCase7.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
    assert_equal(0, copied, "Failed: Bitmap_copyIntoFile() - testCase7")

    Bitmap_destroy(&bitmap);
    fclose(dest);
}

/*
 * Test Bitmap_create and Bitmap_draw_triangle and Bitmap_draw_circle
 * create a unicolored bitmap image and draw a triangle and circle in it
 * than copy the bitmap into the file
 */
void testCase8() {

    // create Bitmap
    Bitmap bitmap;

    RGB rose;
    rose.red = 248;
    rose.green = 203;
    rose.blue = 219;

    uint32_t created = Bitmap_create(&bitmap, &rose, 400, 400);
    assert_equal(0, created, "Failed: Bitmap_create - testCase8")

    // draw circle in the newly created Bitmap
    RGB mint_green;
    mint_green.red = 25;
    mint_green.green = 255;
    mint_green.blue = 175;

    Point origin;
    origin.x_posn = 234;
    origin.y_posn = 287;

    uint32_t radius = 156;

    uint32_t drawn = Bitmap_draw_circle(&bitmap, &origin, radius, &mint_green);
    assert_equal(0, drawn, "Failed: Bitmap_draw_circle() - testCase8")

    // draw triangle in the newly created Bitmap
    RGB blue;
    blue.red = 136;
    blue.green = 172;
    blue.blue = 239;

    Point A;
    A.x_posn = 25;
    A.y_posn = 344;

    Point B;
    B.x_posn = 390;
    B.y_posn = 312;

    Point C;
    C.x_posn = 200;
    C.y_posn = 7;

    uint32_t drawn2 = Bitmap_draw_triangle(&bitmap, &A, &B, &C, &blue);
    assert_equal(0, drawn2, "Failed: Bitmap_draw_triangle() - testCase8")

    //copy the bitmap into a file

    FILE *dest = fopen("testCase8.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
    assert_equal(0, copied, "Failed: Bitmap_copyIntoFile() - testCase8")

    Bitmap_destroy(&bitmap);
    fclose(dest);
}

/*
 * Test Bitmap_create and Bitmap_draw_circle
 * create a unicolored bitmap image and draw a circle in it
 * than copy the bitmap into a File
 */
void testCase9() {

    // create Bitmap
    Bitmap bitmap;

    RGB orange;
    orange.red = 255;
    orange.green = 165;
    orange.blue = 0;

    uint32_t created = Bitmap_create(&bitmap, &orange, 400, 400);
    assert_equal(0, created, "Failed: Bitmap_create - testCase9")

    // draw circle in the newly created Bitmap
    RGB mint_green;
    mint_green.red = 211;
    mint_green.green = 255;
    mint_green.blue = 206;

    Point origin;
    origin.x_posn = 234;
    origin.y_posn = 287;

    uint32_t radius = 156;

    uint32_t drawn = Bitmap_draw_circle(&bitmap, &origin, radius, &mint_green);
    assert_equal(0, drawn, "Failed: Bitmap_draw_triangle() - testCase9")

    //copy the bitmap into a file

    FILE *dest = fopen("testCase9.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
    assert_equal(0, copied, "Failed: Bitmap_copyIntoFile() - testCase9")

    Bitmap_destroy(&bitmap);
    fclose(dest);
}

/*
 * Test Bitmap_create and Bitmap_draw_rectangle
 * create a unicolored bitmap image and draw a rectangle in it
 * than copy the bitmap into a File
 */
void testCase10() {

    // create Bitmap
    Bitmap bitmap;

    RGB orange;
    orange.red = 255;
    orange.green = 165;
    orange.blue = 0;

    uint32_t created = Bitmap_create(&bitmap, &orange, 400, 400);
    assert_equal(0, created, "Failed: Bitmap_create - testCase10")

    // draw circle in the newly created Bitmap
    RGB mint_green;
    mint_green.red = 211;
    mint_green.green = 255;
    mint_green.blue = 206;

    Point A;
    A.x_posn = 20;
    A.y_posn = 200;

    Point B;
    B.x_posn = 200;
    B.y_posn = 200;

    Point C;
    C.x_posn = 200;
    C.y_posn = 20;

    Point D;
    D.x_posn = 20;
    D.y_posn = 20;

    uint32_t drawn = Bitmap_draw_rect(&bitmap, &A, &B, &C, &D, &mint_green);
    assert_equal(0, drawn, "Failed: Bitmap_draw_rect() - testCase10")

    //copy the bitmap into a file
    FILE *dest = fopen("testCase10.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
    assert_equal(0, copied, "Failed: Bitmap_copyIntoFile() - testCase10")

    Bitmap_destroy(&bitmap);
    fclose(dest);
}

/*
 * Test Bitmap_create and Bitmap_draw_rectangle
 * create a unicolored bitmap image and draw a rectangle in it
 * than copy the bitmap into a File
 */
void testCase11() {

    // create Bitmap
    Bitmap bitmap;

    RGB orange;
    orange.red = 255;
    orange.green = 165;
    orange.blue = 0;

    uint32_t created = Bitmap_create(&bitmap, &orange, 400, 400);
    assert_equal(0, created, "Failed: Bitmap_create - testCase11")

    // draw circle in the newly created Bitmap
    RGB mint_green;
    mint_green.red = 211;
    mint_green.green = 255;
    mint_green.blue = 206;

    Point A;
    A.x_posn = 100;
    A.y_posn = 300;

    Point B;
    B.x_posn = 300;
    B.y_posn = 300;

    Point C;
    C.x_posn = 250;
    C.y_posn = 100;

    Point D;
    D.x_posn = 50;
    D.y_posn = 100;

    uint32_t drawn = Bitmap_draw_rect(&bitmap, &A, &B, &C, &D, &mint_green);
    assert_equal(0, drawn, "Failed: Bitmap_draw_rect() - testCase11")

    //copy the bitmap into a file
    FILE *dest = fopen("testCase11.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
    assert_equal(0, copied, "Failed: Bitmap_copyIntoFile() - testCase11")

    Bitmap_destroy(&bitmap);
    fclose(dest);
}

/*
 * Test Bitmap_create and Bitmap_draw_rectangle and Bitmap_draw_circle and Bitmap_draw_rect
 * create a unicolored bitmap image and draw a rectangle in it
 * than copy the bitmap into a File
 */
void testCase12() {

    // create Bitmap
    Bitmap bitmap;

    RGB orange;
    orange.red = 255;
    orange.green = 216;
    orange.blue = 89;

    uint32_t created = Bitmap_create(&bitmap, &orange, 400, 400);
    assert_equal(0, created, "Failed: Bitmap_create - testCase12")

    // draw triangle in the newly created Bitmap
    RGB red;
    red.red = 255;
    red.green = 25;
    red.blue = 105;

    Point A2;
    A2.x_posn = 25;
    A2.y_posn = 344;

    Point B2;
    B2.x_posn = 390;
    B2.y_posn = 312;

    Point C2;
    C2.x_posn = 200;
    C2.y_posn = 7;

    uint32_t drawn3 = Bitmap_draw_triangle(&bitmap, &A2, &B2, &C2, &red);
    assert_equal(0, drawn3, "Failed: Bitmap_draw_triangle() - testCase12")

    // draw circle in the newly created Bitmap
    RGB green;
    green.red = 25;
    green.green = 255;
    green.blue = 175;

    Point origin;
    origin.x_posn = 234;
    origin.y_posn = 287;

    uint32_t radius = 89;

    uint32_t drawn2 = Bitmap_draw_circle(&bitmap, &origin, radius, &green);
    assert_equal(0, drawn2, "Failed: Bitmap_draw_circle() - testCase12")

    // draw rect in the newly created Bitmap
    RGB pink;
    pink.red = 243;
    pink.green = 166;
    pink.blue = 192;

    Point A;
    A.x_posn = 20;
    A.y_posn = 200;

    Point B;
    B.x_posn = 200;
    B.y_posn = 200;

    Point C;
    C.x_posn = 200;
    C.y_posn = 20;

    Point D;
    D.x_posn = 20;
    D.y_posn = 20;

    uint32_t drawn = Bitmap_draw_rect(&bitmap, &A, &B, &C, &D, &pink);
    assert_equal(0, drawn, "Failed: Bitmap_draw_rect() - testCase12")

    //copy the bitmap into a file
    FILE *dest = fopen("testCase12.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
    assert_equal(0, copied, "Failed: Bitmap_copyIntoFile() - testCase12")

    Bitmap_destroy(&bitmap);
    fclose(dest);
}

/**
 * Test Bitmap_convolution with Identity Kernel
 */
void testCase13() {
    // create Bitmap
    Bitmap bitmap;

    RGB color;
    color.red = 255;
    color.green = 128;
    color.blue = 237;

    uint32_t created = Bitmap_create(&bitmap, &color, 4, 4);
    assert_equal(0, created, "Failed: Bitmap_create - testCase13")

    //copy the bitmap into a file pre-change
    FILE *dest = fopen("testCase13_1.bmp", "wb");

    uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
    assert_equal(0, copied, "Failed: Bitmap_copyIntoFile() - testCase13")

    // build another bitmap with the same setup
    Bitmap copy;

    uint32_t created_copy = Bitmap_create(&copy, &color, 4, 4);
    assert_equal(0, created_copy, "Failed: Bitmap_create - testCase13")

    //check if both bitmaps are the same before any change was made
    for (int i = 0; i < bitmap.data_size; i++) {
        RGB * bitmap_pixel = (RGB *) (bitmap.data + i);
        RGB * copy_pixel = (RGB *) (copy.data + i);
        assert_equal(bitmap_pixel->red, copy_pixel->red, "Bitmaps are not the same - testCase13")
        assert_equal(bitmap_pixel->green, copy_pixel->green, "Bitmaps are not the same - testCase13")
        assert_equal(bitmap_pixel->blue, copy_pixel->blue, "Bitmaps are not the same - testCase13")
    }


    // apply convolution and check if the two bitmaps still are equal
    // the identity kernel does not change the picture
    // copy was not changed
    int32_t kernel[] = {0, 0, 0, 0, 1, 0, 0, 0, 0}; // Identity Kernel
    Bitmap_convolution(&bitmap, kernel, 1);

    for (int i = 0; i < bitmap.data_size; i++) {
        RGB * bitmap_pixel = (RGB *) (bitmap.data + i);
        RGB * copy_pixel = (RGB *) (copy.data + i);
        assert_equal(bitmap_pixel->red, copy_pixel->red, "Bitmaps are not the same - testCase13")
        assert_equal(bitmap_pixel->green, copy_pixel->green, "Bitmaps are not the same - testCase13")
        assert_equal(bitmap_pixel->blue, copy_pixel->blue, "Bitmaps are not the same - testCase13")
    }

    //copy the mutated bitmap into a file
    FILE *dest_2 = fopen("testCase13_2.bmp", "wb");

    uint32_t copied_2 = Bitmap_copyIntoFile(dest_2, &bitmap);
    assert_equal(0, copied_2, "Failed: Bitmap_copyIntoFile() - testCase13")

    Bitmap_destroy(&bitmap);
    Bitmap_destroy(&copy);
    fclose(dest);
    fclose(dest_2);
}

/**
 * Test Bitmap_convolution with Box Blur Kernel
 */
void testCase14() {
    Bitmap bitmap;

    FILE *image = fopen("lena.bmp", "rb");

    uint32_t scanned = Bitmap_scan(image, &bitmap);
    assert_equal(0, scanned, "Failed: Bitmap_scan - testCase14")

    // apply convolution and check if the two bitmaps still are equal
    // the identity kernel does not change the picture
    // copy was not changed
    int32_t kernel[] = {1, 1, 1, 1, 1, 1, 1, 1, 1}; // Box Blur
    Bitmap_convolution(&bitmap, kernel, 9);

    //copy the mutated bitmap into a file
    FILE *dest_2 = fopen("testCase14_lena.bmp", "wb");

    uint32_t copied_2 = Bitmap_copyIntoFile(dest_2, &bitmap);
    assert_equal(0, copied_2, "Failed: Bitmap_copyIntoFile() - testCase14")

    Bitmap_destroy(&bitmap);
    fclose(image);
    fclose(dest_2);
}

/**
 * Test Bitmap_convolution with Gaussian Blur
 */
void testCase15() {
    Bitmap bitmap;

    FILE *image = fopen("testCase12.bmp", "rb");

    uint32_t scanned = Bitmap_scan(image, &bitmap);
    assert_equal(0, scanned, "Failed: Bitmap_scan - testCase15")

    // apply convolution and check if the two bitmaps still are equal
    // the identity kernel does not change the picture
    // copy was not changed
    int32_t kernel[] = {1,0,-1,0,0,0,-1,0,1}; //
    Bitmap_convolution(&bitmap, kernel, 1);

    //copy the mutated bitmap into a file
    FILE *dest_2 = fopen("testCase15.bmp", "wb");

    uint32_t copied_2 = Bitmap_copyIntoFile(dest_2, &bitmap);
    assert_equal(0, copied_2, "Failed: Bitmap_copyIntoFile() - testCase15")

    Bitmap_destroy(&bitmap);
    fclose(image);
    fclose(dest_2);
}

/**
 * Test Bitmap_convolution with Identity Kernel
 */
void testCase16() {
    // create Bitmap
    Bitmap bitmap;

    RGB color;
    color.red = 255;
    color.green = 128;
    color.blue = 237;

    uint32_t created = Bitmap_create(&bitmap, &color, 4, 4);
    assert_equal(0, created, "Failed: Bitmap_create - testCase16")

    Bitmap_print(&bitmap);

    // apply convolution
    int32_t kernel[] = {1,0,-1,0,0,0,-1,0,1};
    Bitmap_convolution(&bitmap, kernel, 1);

    //copy the mutated bitmap into a file
    FILE *dest_2 = fopen("testCase16.bmp", "wb");

    uint32_t copied_2 = Bitmap_copyIntoFile(dest_2, &bitmap);
    assert_equal(0, copied_2, "Failed: Bitmap_copyIntoFile() - testCase16")

    Bitmap_destroy(&bitmap);
    fclose(dest_2);
}

void testCase17() {
    // create Bitmap
    Bitmap bitmap;

    FILE * dest = fopen("testCase12.bmp", "rb");

    uint32_t scanned = Bitmap_scan(dest, &bitmap);
    assert_equal(0, scanned, "Failed: Bitmap_scan - testCase17")

    // apply convolution
    int32_t kernel[] = {1,2,1,2,4,2,1,2,1}; // Gaussian Blur
    Bitmap_convolution(&bitmap, kernel, 16);

    //copy the mutated bitmap into a file
    FILE *dest_2 = fopen("testCase17.bmp", "wb");

    uint32_t copied_2 = Bitmap_copyIntoFile(dest_2, &bitmap);
    assert_equal(0, copied_2, "Failed: Bitmap_copyIntoFile() - testCase17")

    Bitmap_destroy(&bitmap);
    fclose(dest_2);
}

void testCase18() {
    // create Bitmap
    Bitmap bitmap;

    FILE * dest = fopen("testCase12.bmp", "rb");

    uint32_t scanned = Bitmap_scan(dest, &bitmap);
    assert_equal(0, scanned, "Failed: Bitmap_scan - testCase18")

    // apply convolution
    int32_t kernel[] = {1,2,1,2,4,2,1,2,1};// Gaussian Blur
    Bitmap_convolution(&bitmap, kernel, 16);

    //copy the mutated bitmap into a file
    FILE *dest_2 = fopen("testCase18.bmp", "wb");

    uint32_t copied_2 = Bitmap_copyIntoFile(dest_2, &bitmap);
    assert_equal(0, copied_2, "Failed: Bitmap_copyIntoFile() - testCase18")

    Bitmap_destroy(&bitmap);
    fclose(dest_2);
}

void printAllPictureInfos() {

    Bitmap bitmap;

    // open bmp picture and scan into Bitmap
    FILE *bmp = fopen("testCase12.bmp", "rb");

    uint32_t scanned = Bitmap_scan(bmp, &bitmap);
    assert_equal(0, scanned, "Failed: scan");

    Bitmap_print(&bitmap);

    Bitmap_destroy(&bitmap);
    fclose(bmp);

}

int main() {
    /*
    testCase1();
    testCase2();
    testCase3();
    testCase4();
    testCase5();
    testCase6();
    testCase7();
    testCase8();
    testCase9();
    testCase10();
    testCase11();

     */
    //testCase12();
    //testCase13();
    //testCase14();
    testCase15();
    testCase16();
    testCase17();
    testCase18();
    printAllPictureInfos();
    return 0;
}