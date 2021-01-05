//
// Created by lara on 04/01/2021.
//

#include "utils.h"
#include "bitmap.h"

void testCase1(){

    char filepath[] = "all_gray.bmp";

    FILE * bmp = fopen(filepath, "rb");

    fseek(bmp, 0, SEEK_END);
    uint32_t file_size = ftell(bmp);
    rewind(bmp);

    // FileHeader
    uint8_t * buffer = malloc(sizeof (uint8_t) * file_size);
    fread(buffer, sizeof (uint8_t), file_size, bmp);

    uint16_t *signature = (uint16_t *) buffer;
    printf("signature: %s\n", (char *)signature);

    uint32_t *fileSize = (uint32_t *)(buffer+1);
    printf("file_size: %d byte\n", *fileSize);

    uint32_t *reserved_fh = fileSize+1;
    printf("reserved: %d\n", *reserved_fh);

    uint32_t *data_offset = fileSize+2;
    printf("data_offset: %d\n", *data_offset);

    // InfoHeader
    uint32_t *header_size = fileSize+3;
    printf("header_size: %d byte\n", *header_size);

    int32_t *width = (int32_t *) (header_size +1);
    printf("width: %d pixels\n", *width);

    int32_t *height = width+1;
    printf("height: %d pixels\n", *height);
}

int main(){
    testCase1();
    return 0;
}