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
}

int main(){
    testCase1();
    return 0;
}