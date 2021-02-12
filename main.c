//
// Created by lara on 02/01/2021.
//

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <bitmap.h>

void info(char ** argv);
void copy(char ** argv);

int main(int argc, char **argv){

    if(argc == 3 && strcmp(argv[1], "info") == 0 || strcmp(argv[1], "-i") == 0){
        info(argv);
    } else if(argc == 4 && strcmp(argv[1], "copy") == 0 || strcmp(argv[1], "-cp") == 0){
        copy(argv);
    } else {
        if(argc < 3){
            fprintf(stderr, "Not enough arguments were passed\n");
        } else {
            fprintf(stderr, "%s: was not recognized as a valid command\n", argv[1]);
        }
        return 1;
    }

    return 0;
}

void info(char ** argv){
    // ./main [info|-i] lena.bmp

    FILE * source = fopen(argv[2], "rb");
    if(source != NULL){
        Bitmap bitmap;
        uint32_t scanned = Bitmap_scan(source, &bitmap);
        if(scanned == 0){
            printf("Bitmap %s:\n", argv[2]);
            Bitmap_print(&bitmap);
            Bitmap_destroy(&bitmap);
        } else {
            fprintf(stderr, "That did not work\n");
        }

    } else {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    }

    fclose(source);
}

void copy(char ** argv){
    // ./main [copy|-cp] lena.bmp lena_copy.bmp

    FILE * source = fopen(argv[2], "rb");
    if(source != NULL){
        Bitmap bitmap;
        uint32_t scanned = Bitmap_scan(source, &bitmap);
        if(scanned == 0){
            FILE * dest = fopen(argv[3], "wr");
            uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
            if(copied == 0){
                Bitmap_destroy(&bitmap);
                fclose(dest);
            } else {
                fprintf(stderr, "That did not work\n");
            }
        } else {
            fprintf(stderr, "That did not work\n");
        }
    } else {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    }
}