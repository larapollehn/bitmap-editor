//
// Created by lara on 04/01/2021.
//

#ifndef LIBRARY_BITMAP_H
#define LIBRARY_BITMAP_H

#include <stdint.h>

typedef struct FileHeader{ // total: 14 bytes
    uint16_t *signature; // 2 bytes
    uint32_t file_size; // file size in bytes
    uint32_t reserved;
    uint32_t data_offset; // offset from beginning of file to the beginning of the bitmap data
}  __attribute__((packed)) FileHeader;

typedef struct InfoHeader{ // total: 40 bytes
    uint32_t header_size; // size of InfoHeader - 4 bytes
    int32_t width; // horizontal width of bitmap in pixels - 4 bytes
    int32_t height; // vertical heigth of bitmap in pixels - 4 bytes
    uint16_t planes; // number of planes - 2 bytes
    uint16_t bit_per_px; // bits per pixel used to store palette entry info (indicates number of possible colors) - 2 bytes
    uint32_t compression; // type of compression: should be 0(no compression, BI_RGB)! - 4 bytes
    uint32_t image_size; // size of compressed image, 0 if compression is 0 - 4 bytes
    int32_t x_px_m; // horizontal resolution: pixels/meter - 4 bytes
    int32_t y_px_m; // vertical resolution: pixels/meter - 4 bytes
    uint32_t colors_used; // number of actually used colors (8 bit = 256), the number of values in ColorTable - 4 bytes
    uint32_t important_colors; // number of important colors, 0 if all - 4 bytes
}  __attribute__((packed)) InfoHeader;

typedef struct ColorTable{
    uint8_t blue; // blue intensity - 1 byte
    uint8_t green; // green intensity - 1 byte
    uint8_t red; // red intensity - 1 byte
    uint8_t reserved; // unused, 0 - 1 byte
}  __attribute__((packed)) ColorTable;

/**
 * Initialize the bmp photo trough scanning the picture byte by byte
 * @param filepath contains the path to the bmp picture
 * @param fileHeader contains information about the file containing the bmp picture
 * @param infoHeader contains information about the bmp picture
 * @param pixels represents all pixels of the bmp picture trough the RGB colors (and their intesity)
 * @return 0 if successful, 1 if failed
 */
uint32_t Bitmap_initialize(FILE * source, FileHeader *fileHeader, InfoHeader *infoHeader, ColorTable * pixels);


#endif //LIBRARY_BITMAP_H
