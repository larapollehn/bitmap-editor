//
// Created by lara on 04/01/2021.
//

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "utils.h"
#include "bitmap.h"

//-#########################################################################
// Bitmap & File handling
//-#########################################################################

uint32_t Bitmap_scan(FILE *source, Bitmap * bitmap) {

    // Fill the given bitmap with information for the headers
    uint32_t readElements = fread(bitmap, sizeof (Bitmap), 1, source);
    check_exit(readElements == 1, "error: trying to read the content of a file\n");

    // check for specific requirements a picture has to have to be processed
    check_exit(bitmap->biCompression == 0, "Error: compressed file"); // no compressed files allowed, only allowed file format is BI_RGB

    // determine if a colorTable is present and if so its size
    uint32_t size_colorTable;
    if(bitmap->biClrUsed == 0){
        if(bitmap->biBitCount == 1 || bitmap->biBitCount == 4 || bitmap->biBitCount == 8){
            size_colorTable = pow(2.0, (double) bitmap->biBitCount);
        } else {
            size_colorTable = 0;
        }
    } else {
        size_colorTable = bitmap->biClrUsed;
    }
    bitmap->colorTable_size = size_colorTable;

    // initialize the colorTable or assign Null-pointer if no colorTable is used
    if(size_colorTable > 0){
        bitmap->colorTable = malloc(sizeof (Color) * size_colorTable);
        check_mem(bitmap->colorTable);

        fseek(source, offsetof(Bitmap, colorTable), SEEK_SET);
        uint32_t readColors= fread(bitmap->colorTable, sizeof (Color), size_colorTable, source);
        check_exit(readColors == size_colorTable, "error: trying to read the content of a file");

    } else {
        bitmap->colorTable = NULL;
    }

    // determine size of data
    // a data unit is always 1 byte
    uint32_t size_data;
    if(bitmap->biCompression == 0 && (bitmap->biWidth % 4 == 0)){
        // calculate the size of the data by the height, width and bit per pixel count
        size_data = ((bitmap->biWidth * bitmap->biHeight * bitmap->biBitCount)/ 8);
    } else if(bitmap->biSizeImage != 0) {
        // sometimes the size of the image is saved in the bitmap header
        size_data = bitmap->biSizeImage;
    } else {
        // calculate the number of pixels of the image
        // depending on the bits per pixel calculate the size of the data
        if(bitmap->biBitCount == 8){
            size_data = (bitmap->biWidth * bitmap->biHeight);
        } else if(bitmap->biBitCount == 16){
            size_data = (bitmap->biWidth * bitmap->biHeight) * 2;
        } else if(bitmap->biBitCount == 24){
            size_data = (bitmap->biWidth * bitmap->biHeight) * 3;
        } else if(bitmap->biBitCount == 32){
            size_data = (bitmap->biWidth * bitmap->biHeight) * 4;
        }
    }
    check_exit(size_data > 0, "Error: image data is corrupt or missing");
    bitmap->data_size = size_data;


    // allocate space for the image data
    bitmap->data = malloc(sizeof (uint8_t) * size_data);
    check_mem(bitmap->data);

    // read image data into the bitmap
    fseek(source, bitmap->bfOffBits, SEEK_SET);
    uint32_t readData = fread(bitmap->data, sizeof (uint8_t), size_data, source);
    check_exit(readData == size_data, "error: trying to read the content of a file");

    return 0;

    error_handling:
        Bitmap_destroy(bitmap);
        return 1;
}

uint32_t Bitmap_copyIntoFile(FILE *dest, Bitmap *bitmap) {

    uint32_t writtenHeaders = fwrite(bitmap,  sizeof (uint8_t), (FILEHEADER_SIZE + bitmap->biSize), dest); // copy the file and info-header into the bmp pic
    check_exit(writtenHeaders == (FILEHEADER_SIZE + bitmap->biSize), "Failed: writing headers");

    if(bitmap->colorTable_size > 0) {
        uint32_t writtenColorTable = fwrite(bitmap->colorTable, sizeof(Color), bitmap->colorTable_size, dest); // copy only the colorTable (not colorTable_size) into the bmp pic
        check_exit(writtenColorTable == bitmap->colorTable_size, "Failed: writing colorTable");
    }

    uint32_t writtenData = fwrite(bitmap->data, sizeof (uint8_t), bitmap->data_size, dest); // copy only the data (not data_size) into bmp pic
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

    if(bitmap->colorTable != NULL){
        free(bitmap->colorTable);
    }

    if(bitmap->data){
        free(bitmap->data);
    }

    return 0;
}

//-#########################################################################
// Modify & Change Bitmap images
//-#########################################################################

uint32_t Bitmap_naive_grayscaling_px(Bitmap * bitmap) {

    uint32_t size = bitmap->biSizeImage;
    if(bitmap->biSizeImage == 0){
        size = bitmap->data_size;
    }

    for(int i = 0; i < size; i+=3){
        uint8_t * blue = (bitmap->data + i);
        uint8_t * green = (bitmap->data + i + 1);
        uint8_t * red = (bitmap->data + i + 2);

        uint8_t gray_tone = (*blue + *green + *red) / 3;

        *blue = gray_tone;
        *green = gray_tone;
        *red = gray_tone;
    }

    return 0;
}

uint32_t Bitmap_naive_grayscaling_ct(Bitmap *bitmap) {

    for(int i = 0; i < bitmap->colorTable_size; i++){
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
    bitmap->biSizeImage = ((bitmap->biWidth * bitmap->biHeight * bitmap->biBitCount)/ 8); // expects biWidth to be divisible by 4
    bitmap->bfSize = bitmap->biSizeImage + FILEHEADER_SIZE + bitmap->biSize;
    bitmap->bfOffBits = FILEHEADER_SIZE + bitmap->biSize;
    bitmap->data_size = bitmap->biSizeImage;
}

uint32_t Bitmap_create(Bitmap *bitmap, FILE *dest, RGB *backgroundColor, uint32_t width, uint32_t height) {

    // initialize the Bitmap Headers containing infos about the image itself
    Bitmap_initialize_header(bitmap, width, height);

    uint32_t dataSize = width * height;

    //RGB * pixel = malloc(sizeof (RGB) * )

    for(int y = 0; y < (width *height); y++){
        //uint32_t writtenData = fwrite(bgr, sizeof(bgr), 1, dest);
        //check_exit(writtenData == 1, "Failed: writing data");
    }

    return 0;
    error_handling:
    return 1;
}

//-#########################################################################
// Draw in Bitmap images
//-#########################################################################

uint32_t Bitmap_draw_triangle(Bitmap *bitmap, FILE *dest, Point *A, Point *B, Point *C, uint32_t width, uint32_t height) {

    Bitmap_initialize_header(bitmap, width, height);

    uint8_t circle[] = {97, 139, 255};
    uint8_t background[] = {255, 255, 255};

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            Point P;
            P.x_posn = x;
            P.y_posn = y;

            uint32_t inTriangle = point_in_triangle(A, B, C, &P);

            if(inTriangle == 1){
                uint32_t writtenData = fwrite(circle, sizeof(circle), 1, dest);
                check_exit(writtenData == 1, "Failed: writing data");
            } else {
                uint32_t writtenData = fwrite(background, sizeof(background), 1, dest);
                check_exit(writtenData == 1, "Failed: writing data");
            }
        }
    }

    return 0;
    error_handling:
        return 1;
}

uint32_t Bitmap_draw_circle(Bitmap *bitmap, FILE *dest, Point * origin, uint32_t radius, uint32_t width, uint32_t height){

    Bitmap_initialize_header(bitmap, width, height);

    uint8_t circle[] = {97, 139, 255};
    uint8_t background[] = {255, 255, 255};

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            Point P;
            P.x_posn = x;
            P.y_posn = y;

            uint32_t distance = distance_ptp(&P, origin);
            if(distance <= radius){
                uint32_t writtenData = fwrite(circle, sizeof(circle), 1, dest);
                check_exit(writtenData == 1, "Failed: writing data");
            } else {
                uint32_t writtenData = fwrite(background, sizeof(background), 1, dest);
                check_exit(writtenData == 1, "Failed: writing data");
            }
        }
    }

    return 0;
    error_handling:
    return 1;
}

















