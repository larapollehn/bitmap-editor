//
// Created by lara on 04/01/2021.
//

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "utils.h"
#include "bitmap.h"

//-#########################################################################
// Bitmap & File handling
//-#########################################################################

uint32_t Bitmap_scan(FILE *source, Bitmap *bitmap) {

    // Fill the given bitmap with information for the headers
    uint32_t readElements = fread(bitmap, sizeof(Bitmap), 1, source);
    check_exit(readElements == 1, "error: trying to read the content of a file\n");

    // check for specific requirements a picture has to have to be processed
    check_exit(bitmap->biCompression == 0,
               "Error: compressed file"); // no compressed files allowed, only allowed file format is BI_RGB

    // determine if a colorTable is present and if so its size
    uint32_t size_colorTable;
    if (bitmap->biClrUsed == 0) {
        if (bitmap->biBitCount == 1 || bitmap->biBitCount == 4 || bitmap->biBitCount == 8) {
            size_colorTable = pow(2.0, (double) bitmap->biBitCount);
        } else {
            size_colorTable = 0;
        }
    } else {
        size_colorTable = bitmap->biClrUsed;
    }
    bitmap->colorTable_size = size_colorTable;

    // initialize the colorTable or assign Null-pointer if no colorTable is used
    if (size_colorTable > 0) {
        bitmap->colorTable = malloc(sizeof(Color) * size_colorTable);
        check_mem(bitmap->colorTable);

        fseek(source, offsetof(Bitmap, colorTable), SEEK_SET);
        uint32_t readColors = fread(bitmap->colorTable, sizeof(Color), size_colorTable, source);
        check_exit(readColors == size_colorTable, "error: trying to read the content of a file");

    } else {
        bitmap->colorTable = NULL;
    }

    // determine size of data
    // a data unit is always 1 byte
    uint32_t size_data;
    if (bitmap->biCompression == 0 && (bitmap->biWidth % 4 == 0)) {
        // calculate the size of the data by the height, width and bit per pixel count
        size_data = ((bitmap->biWidth * bitmap->biHeight * bitmap->biBitCount) / 8);
    } else if (bitmap->biSizeImage != 0) {
        // sometimes the size of the image is saved in the bitmap header
        size_data = bitmap->biSizeImage;
    } else {
        // calculate the number of pixels of the image
        // depending on the bits per pixel calculate the size of the data
        if (bitmap->biBitCount == 8) {
            size_data = (bitmap->biWidth * bitmap->biHeight);
        } else if (bitmap->biBitCount == 16) {
            size_data = (bitmap->biWidth * bitmap->biHeight) * 2;
        } else if (bitmap->biBitCount == 24) {
            size_data = (bitmap->biWidth * bitmap->biHeight) * 3;
        } else if (bitmap->biBitCount == 32) {
            size_data = (bitmap->biWidth * bitmap->biHeight) * 4;
        }
    }
    check_exit(size_data > 0, "Error: image data is corrupt or missing");
    bitmap->data_size = size_data;


    // allocate space for the image data
    bitmap->data = malloc(sizeof(uint8_t) * size_data);
    check_mem(bitmap->data);

    // read image data into the bitmap
    fseek(source, bitmap->bfOffBits, SEEK_SET);
    uint32_t readData = fread(bitmap->data, sizeof(uint8_t), size_data, source);
    check_exit(readData == size_data, "error: trying to read the content of a file");

    return 0;

    error_handling:
    Bitmap_destroy(bitmap);
    return 1;
}

uint32_t Bitmap_copyIntoFile(FILE *dest, Bitmap *bitmap) {

    uint32_t writtenHeaders = fwrite(bitmap, sizeof(uint8_t), (FILEHEADER_SIZE + bitmap->biSize),
                                     dest); // copy the file and info-header into the bmp pic
    check_exit(writtenHeaders == (FILEHEADER_SIZE + bitmap->biSize), "Failed: writing headers");

    if (bitmap->colorTable_size > 0) {
        uint32_t writtenColorTable = fwrite(bitmap->colorTable, sizeof(Color), bitmap->colorTable_size,
                                            dest); // copy only the colorTable (not colorTable_size) into the bmp pic
        check_exit(writtenColorTable == bitmap->colorTable_size, "Failed: writing colorTable");
    }

    uint32_t writtenData = fwrite(bitmap->data, sizeof(uint8_t), bitmap->data_size,
                                  dest); // copy only the data (not data_size) into bmp pic
    check_exit(writtenData == bitmap->data_size, "Failed: writing data");

    return 0;
    error_handling:
    return 1;
}

void Bitmap_print(const Bitmap *bitmap) {

    printf("\nBitmap:\n");
    printf("bfType: %s\n", (char *) &bitmap->bfType);
    printf("bfSize: %d byte\n", bitmap->bfSize);
    printf("bfReserved: %d\n", bitmap->bfReserved);
    printf("bfOffBits: %d\n", bitmap->bfOffBits);
    printf("\n");
    printf("biSize: %d\n", bitmap->biSize);
    printf("biWidth: %d px\n", bitmap->biWidth);
    printf("biHeight: %d px\n", bitmap->biHeight);
    printf("biPlanes: %d\n", bitmap->biPlanes);
    printf("biBitCount: %d\n", bitmap->biBitCount);
    printf("biCompression: %d\n", bitmap->biCompression);
    printf("biSizeImage: %d\n", bitmap->biSizeImage);
    printf("biXPelsPerMeter: %d px/m\n", bitmap->biXPelsPerMeter);
    printf("biYPelsPerMeter: %d px/m\n", bitmap->biYPelsPerMeter);
    printf("biClrUsed: %d\n", bitmap->biClrUsed);
    printf("biClrImportant: %d\n", bitmap->biClrImportant);
    printf("\n");
    printf("colorTable_size: %d\n", bitmap->colorTable_size);
    printf("data_size: %d\n", bitmap->data_size);
}

uint32_t Bitmap_destroy(Bitmap *bitmap) {

    if (bitmap->colorTable != NULL) {
        free(bitmap->colorTable);
    }

    if (bitmap->data) {
        free(bitmap->data);
    }

    return 0;
}

//-#########################################################################
// Modify & Change Bitmap images
//-#########################################################################

uint32_t Bitmap_naive_grayscaling_px(Bitmap *bitmap) {

    uint32_t size = bitmap->biSizeImage;
    if (bitmap->biSizeImage == 0) {
        size = bitmap->data_size;
    }

    for (int i = 0; i < size; i += 3) {
        uint8_t *blue = (bitmap->data + i);
        uint8_t *green = (bitmap->data + i + 1);
        uint8_t *red = (bitmap->data + i + 2);

        uint8_t gray_tone = (*blue + *green + *red) / 3;

        *blue = gray_tone;
        *green = gray_tone;
        *red = gray_tone;
    }

    return 0;
}

uint32_t Bitmap_naive_grayscaling_ct(Bitmap *bitmap) {

    for (int i = 0; i < bitmap->colorTable_size; i++) {
        uint8_t blue = (bitmap->colorTable + i)->blue;
        uint8_t green = (bitmap->colorTable + i)->green;
        uint8_t red = (bitmap->colorTable + i)->red;

        uint8_t gray_tone = (blue + green + red) / 3;

        (bitmap->colorTable + i)->blue = gray_tone;
        (bitmap->colorTable + i)->green = gray_tone;
        (bitmap->colorTable + i)->red = gray_tone;
    }

    return 0;
}

//-#########################################################################
// Creating Bitmap images
//-#########################################################################

void Bitmap_initialize_header(Bitmap *bitmap, uint32_t width, uint32_t height) {
    // these are chosen by me
    bitmap->biWidth = width;
    bitmap->biHeight = height;
    bitmap->biBitCount = 24;
    bitmap->colorTable_size = 0;
    bitmap->bfType = 0x4D42;  // https://stackoverflow.com/questions/601430/multibyte-character-constants-and-bitmap-file-header-type-constants
    bitmap->colorTable = NULL;

    // these are standard
    bitmap->bfReserved = 0;
    bitmap->biSize = 40;
    bitmap->biPlanes = 1;
    bitmap->biCompression = 0;
    bitmap->biXPelsPerMeter = 0;
    bitmap->biYPelsPerMeter = 0;
    bitmap->biClrUsed = 0;
    bitmap->biClrImportant = 0;

    // the following must be calculated
    bitmap->biSizeImage = ((bitmap->biWidth * bitmap->biHeight * bitmap->biBitCount) /
                           8); // expects biWidth to be divisible by 4
    bitmap->bfSize = bitmap->biSizeImage + FILEHEADER_SIZE + bitmap->biSize;
    bitmap->bfOffBits = FILEHEADER_SIZE + bitmap->biSize;
    bitmap->data_size = bitmap->biSizeImage;
}

uint32_t Bitmap_create(Bitmap *bitmap, const RGB *backgroundColor, uint32_t width, uint32_t height) {

    // initialize the Bitmap Headers containing infos about the image itself
    Bitmap_initialize_header(bitmap, width, height);

    // determine size of data
    // a data unit is always 1 byte
    uint32_t size_data;
    if (bitmap->biCompression == 0 && (bitmap->biWidth % 4 == 0)) {
        // calculate the size of the data by the height, width and bit per pixel count
        size_data = ((bitmap->biWidth * bitmap->biHeight * bitmap->biBitCount) / 8);
    } else if (bitmap->biSizeImage != 0) {
        // sometimes the size of the image is saved in the bitmap header
        size_data = bitmap->biSizeImage;
    } else {
        // calculate the number of pixels of the image
        // depending on the bits per pixel calculate the size of the data
        if (bitmap->biBitCount == 8) {
            size_data = (bitmap->biWidth * bitmap->biHeight);
        } else if (bitmap->biBitCount == 16) {
            size_data = (bitmap->biWidth * bitmap->biHeight) * 2;
        } else if (bitmap->biBitCount == 24) {
            size_data = (bitmap->biWidth * bitmap->biHeight) * 3;
        } else if (bitmap->biBitCount == 32) {
            size_data = (bitmap->biWidth * bitmap->biHeight) * 4;
        }
    }
    check_exit(size_data > 0, "size_data should not be zero");

    // allocate the bitmap data array
    bitmap->data = malloc(sizeof(uint8_t) * size_data);

    // copy the backgroundColor into the pixel array
    for (int i = 0; i < (size_data / 3); i++) {
        memcpy((bitmap->data + i * 3), backgroundColor, 3);
    }

    return 0;
    error_handling:
    return 1;
}

//-#########################################################################
// Draw in Bitmap images
//-#########################################################################

uint32_t Bitmap_draw_triangle(Bitmap *bitmap, const Point *A, const Point *B, const Point *C, const RGB *color) {
    for (int y = 0; y < bitmap->biHeight; y++) {
        for (int x = 0; x < bitmap->biWidth; x++) {
            Point P;
            P.x_posn = (fp_t) x;
            P.y_posn = (fp_t) y;

            uint32_t inTriangle = point_in_triangle(A, B, C, &P);

            if (inTriangle == 1) {
                uint32_t index = (bitmap->biHeight * y) + x;
                uint32_t startIndex = (3 * index);
                memcpy((bitmap->data + startIndex), color, 3);
            }
        }
    }

    return 0;
}

uint32_t Bitmap_draw_circle(Bitmap *bitmap, const Point *origin, uint32_t radius, const RGB *color) {

    for (int y = 0; y < bitmap->biHeight; y++) {
        for (int x = 0; x < bitmap->biWidth; x++) {
            Point P;
            P.x_posn = x;
            P.y_posn = y;

            uint32_t distance = distance_ptp(&P, origin);
            if (distance <= radius) {
                uint32_t index = (bitmap->biHeight * y) + x;
                uint32_t startIndex = (3 * index);
                memcpy((bitmap->data + startIndex), color, 3);
            }
        }
    }

    return 0;
}

uint32_t Bitmap_draw_rect(Bitmap *bitmap, const Point *A, const Point *B, const Point *C, const Point *D, RGB *color) {

    for (int y = 0; y < bitmap->biHeight; y++) {
        for (int x = 0; x < bitmap->biWidth; x++) {
            Point P;
            P.x_posn = x;
            P.y_posn = y;

            uint32_t pInRect = point_in_rect(A, B, C, D, &P);
            if (pInRect == 1) {
                uint32_t index = (bitmap->biHeight * y) + x;
                uint32_t startIndex = (3 * index);
                memcpy((bitmap->data + startIndex), color, 3);
            }
        }
    }

    return 0;
}

uint32_t Bitmap_convolution(Bitmap *bitmap, const float *kernel, float divider) {

    /*
     * Identity Kernel
     * float kernel[] = {0, 0, 0
     *                   0, 1, 0
     *                   0, 0, 0}
     */

    for (int y = 1; y < bitmap->biHeight - 1; y++) {
        for (int x = 1; x < bitmap->biWidth - 1; x++) {
            int32_t index = (bitmap->biHeight * y) + x;

            // get the indices of all kernel relevant data_pixels
            RGB * self = bitmap->data + (index*3);
            RGB * left = bitmap->data + ((index*3) - 1);
            RGB * right = bitmap->data + ((index*3) + 1);

            RGB * top = bitmap->data + ((index*3) - bitmap->biWidth);
            RGB * top_left = bitmap->data + (((index*3) - bitmap->biWidth) - 1);
            RGB * top_right = bitmap->data + (((index*3) - bitmap->biWidth) + 1);

            RGB * bottom = bitmap->data + ((index*3) + bitmap->biWidth);
            RGB * bottom_left = bitmap->data + (((index*3) + bitmap->biWidth) - 1);
            RGB * bottom_right = bitmap->data + (((index*3) + bitmap->biWidth) + 1);

            float red = ((top_left->red * kernel[0]) + (top->red * kernel[1]) + (top_right->red * kernel[2]) +
                        (left->red * kernel[3]) + (self->red * kernel[4]) + (right->red * kernel[5]) +
                        (bottom_left->red * kernel[6]) + (bottom->red * kernel[7]) + (bottom_right->red * kernel[8])) / divider;

            float green = ((top_left->green * kernel[0]) + (top->green * kernel[1]) + (top_right->green * kernel[2]) +
                          (left->green * kernel[3]) + (self->green * kernel[4]) + (right->green * kernel[5]) +
                          (bottom_left->green * kernel[6]) + (bottom->green * kernel[7]) + (bottom_right->green * kernel[8])) / divider;

            float blue = ((top_left->blue * kernel[0]) + (top->blue * kernel[1]) + (top_right->blue * kernel[2]) +
                         (left->blue * kernel[3]) + (self->blue * kernel[4]) + (right->blue * kernel[5]) +
                         (bottom_left->blue * kernel[6]) + (bottom->blue * kernel[7]) + (bottom_right->blue * kernel[8])) / divider;


            if(red < 0){
                red = 0;
            } else if(red > 255){
                red = 255;
            }

            if(green < 0){
                green = 0;
            } else if(green > 255){
                green = 255;
            }

            if(blue < 0){
                blue = 0;
            } else if(blue > 255){
                blue = 255;
            }

            // change the color of the current pixel based on the calculation made with the kernel
            self->red = red;
            self->green = green;
            self->blue = blue;

        }
    }

    return 0;
}


















