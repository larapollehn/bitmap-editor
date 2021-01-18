//
// Created by lara on 04/01/2021.
//

#ifndef LIBRARY_BITMAP_H
#define LIBRARY_BITMAP_H

#define FILEHEADER_SIZE 14

#include <stdint.h>

typedef struct Color{
    uint8_t blue; // blue intensity - 1 byte
    uint8_t green; // green intensity - 1 byte
    uint8_t red; // red intensity - 1 byte
    uint8_t alpha; // unused, 0 - 1 byte
}  __attribute__((packed)) Color;

/* Naming-Schema https://de.wikipedia.org/wiki/Windows_Bitmap#Dateiformat_(Version_3) */
typedef struct Bitmap {
    // BITMAPFILEHEADER
    uint16_t bfType; // 2 bytes
    uint32_t bfSize; // file size in bytes
    uint32_t bfReserved;
    uint32_t bfOffBits; // offset from beginning of file to the beginning of the bitmap data
    // BITMAPINFOHEADER
    uint32_t biSize; // size of InfoHeader - 4 bytes
    int32_t biWidth; // horizontal biWidth of bitmap in pixels - 4 bytes
    int32_t biHeight; // vertical heigth of bitmap in pixels - 4 bytes
    uint16_t biPlanes; // number of biPlanes - 2 bytes
    uint16_t biBitCount; // bits per pixel used to store palette entry info (indicates number of possible colors) - 2 bytes
    uint32_t biCompression; // type of biCompression: should be 0(no biCompression, BI_RGB)! - 4 bytes
    uint32_t biSizeImage; // size of compressed image, 0 if biCompression is 0 - 4 bytes
    int32_t biXPelsPerMeter; // horizontal resolution: pixels/meter - 4 bytes
    int32_t biYPelsPerMeter; // vertical resolution: pixels/meter - 4 bytes
    uint32_t biClrUsed; // number of actually used colors (8 bit = 256), the number of values in Color - 4 bytes
    uint32_t biClrImportant; // number of important colors, 0 if all - 4 bytes
    Color * colorTable; // lists all colors used by the image
    uint8_t * data;
    uint32_t data_size;
    uint32_t colorTable_size;
}__attribute__((packed)) Bitmap;


/**
 * Initialize the bmp photo trough scanning the picture byte by byte
 * @param filepath contains the path to the bmp picture
 * @param the destination Bitmap to contain the read data
 * @return 0 if successful, 1 if failed
 */
uint32_t Bitmap_scan(FILE * source, Bitmap * bitmap);
/**
 * Empty the allocated mem-space used for the bitmap
 * @param bitmap whose content (data, colorTable) will be freed
 * @return 0 if successfull, 1 if failed
 */
uint32_t Bitmap_destroy(Bitmap * bitmap);
/**
 * Use the content of a Bitmap and produce a new bitmap picture,
 * this is essentially a copy of an original bmp picture
 * @param dest is the empty bmp file
 * @param bitmap is the original picture to be copied into the dest file
 * @return 0 if successful, 1 if failed
 */
uint32_t Bitmap_copyIntoFile(FILE * dest, Bitmap * bitmap);
/**
 * Prints a bitmap in command Line interface/terminal
 * @param bitmap to be printed
 * @param filepath to the file containing the bitmap
 * @return 0 if successful, 1 if failed
 */
uint32_t Bitmap_print(Bitmap * bitmap, char * filepath);

/**
 * Create a bitmap with 24 bit per px
 * @param bitmap
 * @param dest file that will contain the bitmap
 * @return 0 if successful, 1 if failed
 */
uint32_t Bitmap_create(Bitmap * bitmap, FILE * dest);

/**
 * Grayscaling in the most naive way
 * gray_tone = (red + green + blue) / 3
 * For bitmap pictures without a colorTable and 24 bpp
 * The actual bitmap.data (pixels) are changed to the gray_tone
 * @param source id the file containing the bitmap image
 * @return 0 if successful, 1 if failed
 */
uint32_t Bitmap_naive_grayscaling_px(Bitmap * bitmap);

/**
 * Grayscaling in the most naive way
 * gray_tone = (red + green + blue) / 3
 * For bitmap pictures with a colorTable
 * The entries of the colorTable are changed to the gray_tone
 * @param source id the file containing the bitmap image
 * @return 0 if successful, 1 if failed
 */
uint32_t Bitmap_naive_grayscaling_ct(Bitmap * bitmap);

/**
 * Create a bitmap picture with an unfilled square around
 * Start point of painting is always O(0/0) in the upper left corner
 * @param bitmap
 * @param dest file that will contain the bitmap
 * @return 0 if successful, 1 if failed
 */
uint32_t Bitmap_draw_square(Bitmap * bitmap, FILE * dest);

/**
 * Create a bitmap with a filled triangle on it
 * Start point of painting is always O(0/0) in the upper left corner
 * @param bitmap
 * @param dest file that will contain the bitmap
 * @return 0 if successful, 1 if failed
 */
uint32_t Bitmap_draw_triangle(Bitmap * bitmap, FILE * dest);

#endif //LIBRARY_BITMAP_H
