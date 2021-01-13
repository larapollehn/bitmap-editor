//
// Created by lara on 04/01/2021.
//

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "utils.h"
#include "bitmap.h"

uint32_t Bitmap_scan(FILE *source, Bitmap * bitmap) {

    // Fill the given bitmap with information for the headers
    uint32_t readElements = fread(bitmap, sizeof (Bitmap), 1, source);
    check_exit(readElements == 1, "error: trying to read the content of a file\n");

    // check for specific requirements a picture has to have to be processed
    check_exit(bitmap->biCompression == 0, "Error: compressed file"); // no compressed files allowed, only allowed file format is BI_RGB

    // determine if a colorTable is present and its size
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
    uint32_t size_data;
    if(bitmap->biCompression == 0 && (bitmap->biWidth % 4 == 0)){
        size_data = ((bitmap->biWidth * bitmap->biHeight * bitmap->biBitCount)/ 8);
    } else {
        size_data = bitmap->biSizeImage;
    }
    check_exit(size_data > 0, "Error: image data is corrupt or missing");
    bitmap->data_size = size_data;


    //initialize the image-data
    // the size of each data unit depends:
    // if the colors are indexed trough a colorTable each pixel has 1 byte
    // otherwise each pixels directly holds the color information and is of bigger size
    uint32_t pixel_size;
    if(size_colorTable > 0){
        pixel_size = sizeof (uint8_t);
        bitmap->data = malloc(sizeof (uint8_t) * size_data);
        check_mem(bitmap->data);
    } else if(bitmap->biBitCount == 16){
        pixel_size = sizeof (uint16_t);
        bitmap->data = malloc(sizeof (uint16_t) * size_data);
        check_mem(bitmap->data);
    } else if(bitmap->biBitCount == 24){
        pixel_size = sizeof (uint8_t) * 3;
        bitmap->data = malloc((sizeof (uint8_t) * 3) * size_data);
        check_mem(bitmap->data);
    } else if(bitmap->biBitCount == 32){
        pixel_size = sizeof (uint32_t);
        bitmap->data = malloc(sizeof (uint32_t) * size_data);
        check_mem(bitmap->data);
    }


    fseek(source, bitmap->bfOffBits, SEEK_SET);
    uint32_t readData = fread(bitmap->data, pixel_size, size_data, source);
    check_exit(readData == (size_data / pixel_size), "error: trying to read the content of a file");

    return 0;

    error_handling:
        Bitmap_destroy(bitmap);
        return 1;
}

uint32_t Bitmap_destroy(Bitmap *bitmap) {

    if(bitmap->colorTable){
        free(bitmap->colorTable);
    }

    if(bitmap->data){
        free(bitmap->data);
    }

    return 0;
}

uint32_t Bitmap_copyIntoFile(FILE *dest, Bitmap *bitmap) {

    uint32_t writtenHeaders = fwrite(bitmap,  sizeof (uint8_t), (FILEHEADER_SIZE + bitmap->biSize), dest); // copy the file and info-header into the bmp pic
    check_exit(writtenHeaders == (FILEHEADER_SIZE + bitmap->biSize), "Failed: writing headers");

    if(bitmap->colorTable_size > 0) {
        uint32_t writtenColorTable = fwrite(bitmap->colorTable, sizeof(Color), bitmap->colorTable_size, dest); // copy only the colorTable (not colorTable_size) into the bmp pic
        check_exit(writtenColorTable == bitmap->colorTable_size, "Failed: writing colorTable");
    }
    uint32_t pixel_size;
    if(bitmap->colorTable_size == 0){
        pixel_size = sizeof (uint8_t);
    } else if(bitmap->biBitCount == 16){
        pixel_size = sizeof (uint16_t);
    } else if(bitmap->biBitCount == 24){
        pixel_size = sizeof (uint8_t) * 3;
    } else if(bitmap->biBitCount == 32){
        pixel_size = sizeof (uint32_t);
    }

    uint32_t writtenData = fwrite(bitmap->data, pixel_size, bitmap->data_size, dest); // copy only the data (not data_size) into bmp pic
    check_exit(writtenData == bitmap->data_size, "Failed: writing data");

    return 0;
    error_handling:
        return 1;
}

uint32_t Bitmap_print(Bitmap *bitmap, char * filepath) {

    printf("\nBitmap %s:\n", filepath);
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

    return 0;
}

uint32_t Bitmap_create(Bitmap *bitmap, FILE *dest) {

    // these are chosen by me
    bitmap->biWidth = 40;
    bitmap->biHeight = 8;
    bitmap->biBitCount = 24;
    bitmap->colorTable_size = 0;
    bitmap->bfType = 0x4D42;  // https://stackoverflow.com/questions/601430/multibyte-character-constants-and-bitmap-file-header-type-constants

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

    // write the now initialized Headers into the destination file
    uint32_t writtenHeaders = fwrite(bitmap,  sizeof (uint8_t), (FILEHEADER_SIZE + bitmap->biSize), dest); // copy the file and info-header into the bmp pic
    check_exit(writtenHeaders == (FILEHEADER_SIZE + bitmap->biSize), "Failed: writing headers");

    // write the pixels of bitmap->data into the file
    for( int i = 0; i < bitmap->biSizeImage; i++){
        uint8_t bgr[] = {222,128,193};
        uint32_t writtenData = fwrite(bgr, sizeof(bgr), 1, dest);
        check_exit(writtenData == 1, "Failed: writing data");
    }

    return 0;
    error_handling:
        return 1;
}

uint32_t Bitmap_naive_grayscaling(Bitmap * bitmap) {

    uint8_t blue = *(bitmap->data);
    uint8_t green = *(bitmap->data +1);
    uint8_t red = *(bitmap->data +2);

    uint8_t gray_tone = (blue + green + red) /3;

    return 0;
}
