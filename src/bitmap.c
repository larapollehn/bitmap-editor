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
    check_exit(readElements == 1, "error: trying to read the content of a file\n")

    // check for specific requirements a picture has to have to be processed
    check_exit(bitmap->biCompression == 0,
               "Error: compressed file") // no compressed files allowed, only allowed file format is BI_RGB

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
        check_mem(bitmap->colorTable)

        fseek(source, offsetof(Bitmap, colorTable), SEEK_SET);
        uint32_t readColors = fread(bitmap->colorTable, sizeof(Color), size_colorTable, source);
        check_exit(readColors == size_colorTable, "error: trying to read the content of a file")

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
    check_exit(size_data > 0, "Error: image data is corrupt or missing")
    bitmap->data_size = size_data;


    // allocate space for the image data
    bitmap->data = malloc(sizeof(uint8_t) * size_data);
    check_mem(bitmap->data)

    // read image data into the bitmap
    fseek(source, bitmap->bfOffBits, SEEK_SET);
    uint32_t readData = fread(bitmap->data, sizeof(uint8_t), size_data, source);
    check_exit(readData == size_data, "error: trying to read the content of a file")

    return 0;

    error_handling:
    Bitmap_destroy(bitmap);
    return 1;
}

uint32_t Bitmap_copyIntoFile(FILE *dest, Bitmap *bitmap) {

    uint32_t writtenHeaders = fwrite(bitmap, sizeof(uint8_t), (FILEHEADER_SIZE + bitmap->biSize),
                                     dest); // copy the file and info-header into the bmp pic
    check_exit(writtenHeaders == (FILEHEADER_SIZE + bitmap->biSize), "Failed: writing headers")

    if (bitmap->colorTable_size > 0) {
        uint32_t writtenColorTable = fwrite(bitmap->colorTable, sizeof(Color), bitmap->colorTable_size,
                                            dest); // copy only the colorTable (not colorTable_size) into the bmp pic
        check_exit(writtenColorTable == bitmap->colorTable_size, "Failed: writing colorTable")
    }

    uint32_t writtenData = fwrite(bitmap->data, sizeof(uint8_t), bitmap->data_size,
                                  dest); // copy only the data (not data_size) into bmp pic
    check_exit(writtenData == bitmap->data_size, "Failed: writing data")

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

    if (bitmap->colorTable ) {
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
    check_exit(size_data > 0, "size_data should not be zero")

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

uint32_t Bitmap_convolution(Bitmap *bitmap, const int32_t *kernel, float divider) {

    // copy the bitmap data
    uint8_t * copy_data = malloc(sizeof(uint8_t) * bitmap->data_size);
    memcpy(copy_data, bitmap->data, bitmap->data_size);

    for (int32_t y = 1; y < bitmap->biHeight - 1; y++) {
        for (int32_t x = 1; x < bitmap->biWidth - 1; x++) {
            int32_t index = (bitmap->biWidth * y) + x;

            // get the indices of all kernel relevant data_pixels
            RGB * self = (RGB *) (copy_data + (index*3));
            RGB * left = self - 1;
            RGB * right = self + 1;

            RGB * top = (RGB *) (copy_data + ((index - bitmap->biWidth)*3));
            RGB * top_left = top - 1;
            RGB * top_right = top + 1;

            RGB * bottom = (RGB *)(copy_data + ((index + bitmap->biWidth)*3));
            RGB * bottom_left = bottom - 1;
            RGB * bottom_right = bottom + 1;

            int32_t red_1 = top_left->red * kernel[0];
            int32_t red_2 = top->red * kernel[1];
            int32_t red_3 = top_right->red * kernel[2];
            int32_t red_4 = left->red * kernel[3];
            int32_t red_5 = self->red * kernel[4];
            int32_t red_6 = right->red * kernel[5];
            int32_t red_7 = bottom_left->red * kernel[6];
            int32_t red_8 = bottom->red * kernel[7];
            int32_t red_9 = bottom_right->red * kernel[8];

            int32_t red_sum = red_1 + red_2 + red_3 + red_4 + red_5 + red_6 + red_7 + red_8 + red_9;
            float red = (float)red_sum / divider;


            int32_t green_1 = top_left->green * kernel[0];
            int32_t green_2 = top->green * kernel[1];
            int32_t green_3 = top_right->green * kernel[2];
            int32_t green_4 = left->green * kernel[3];
            int32_t green_5 = self->green * kernel[4];
            int32_t green_6 = right->green * kernel[5];
            int32_t green_7 = bottom_left->green * kernel[6];
            int32_t green_8 = bottom->green * kernel[7];
            int32_t green_9 = bottom_right->green * kernel[8];

            int32_t green_sum = green_1 + green_2 + green_3 + green_4 + green_5 + green_6 + green_7 + green_8 + green_9;
            float green = (float)green_sum / divider;


            int32_t blue_1 = top_left->blue * kernel[0];
            int32_t blue_2 = top->blue * kernel[1];
            int32_t blue_3 = top_right->blue * kernel[2];
            int32_t blue_4 = left->blue * kernel[3];
            int32_t blue_5 = self->blue * kernel[4];
            int32_t blue_6 = right->blue * kernel[5];
            int32_t blue_7 = bottom_left->blue * kernel[6];
            int32_t blue_8 = bottom->blue * kernel[7];
            int32_t blue_9 = bottom_right->blue * kernel[8];

            int32_t blue_sum = blue_1 + blue_2 + blue_3 + blue_4 + blue_5 + blue_6 + blue_7 + blue_8 + blue_9;
            float blue = (float)blue_sum / divider;



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

            RGB * pixel = (RGB *) (bitmap->data + (index*3));

            // change the color of the current pixel based on the calculation made with the kernel
            pixel->red = red;
            pixel->green = green;
            pixel->blue = blue;
        }
    }

    free(copy_data);
    return 0;
}

uint32_t Bitmap_transform(Bitmap *bitmap) {
    check_exit(bitmap->biBitCount == 8, "Only 8 bpp images can be transformed with Bitmap_transform()");

    // calculate the new size for the bitmap data after transformation
    uint32_t new_data_size = sizeof (RGB) * bitmap->data_size;

    // allocate space for the data to be transformed
    uint8_t * transformed_data = malloc(new_data_size);

    for(int i = 0; i < bitmap->data_size; i++){
        // get the color for the current pixel according to its reference to the indexed colorTable
        RGB * color = (RGB *) ((bitmap->colorTable + *(bitmap->data + i)));

        // since the bitmap.data is stored inverse, the RGB color values have to be flipped
        uint8_t *blue = (transformed_data + (i*3));
        uint8_t *green = (transformed_data + (i*3) + 1);
        uint8_t *red = (transformed_data + (i*3) + 2);

        *blue =  color->red;
        *green = color->green;
        *red = color->blue;
    }

    // free the old data and assign the transformed bitmap.data
    // as well as the newly calculated data_size post transformation
    free(bitmap->data);
    bitmap->data = transformed_data;
    bitmap->data_size = new_data_size;

    // change the image accordingly to its new form
    bitmap->biBitCount = 24;

    // remove the colorTable info
    bitmap->biClrUsed = 0;
    bitmap->biClrImportant = 0;

    bitmap->bfSize = bitmap->bfOffBits * new_data_size;

    return 0;
    error_handling:
        return 1;
}


















