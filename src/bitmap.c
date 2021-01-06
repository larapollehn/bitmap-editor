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
    uint32_t readElements = fread(&bitmap, sizeof (Bitmap), 1, source);
    check_exit(readElements == 1, "error: trying to read the content of a file\n");

    // check for specific requirements a picture has to have to be processed
    check_exit(bitmap->biCompression == 0, "Error: compressed file");
    check_exit(bitmap->biSizeImage > 0, "Error: corrupt file");


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

    // initialize the colorTable
    if(size_colorTable > 0){
        bitmap->colorTable = malloc(sizeof (Color) * size_colorTable);
        check_mem(bitmap->colorTable);

        fseek(source, offsetof(Bitmap, colorTable), SEEK_SET);
        uint32_t readColors= fread(bitmap->colorTable, sizeof (Color), size_colorTable, source);
        check_exit(readColors == size_colorTable, "error: trying to read the content of a file");

    } else {
        bitmap->colorTable = NULL;
    }




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
