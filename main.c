//
// Created by lara on 02/01/2021.
//

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <bitmap.h>

void info(char ** argv);
void copy(char ** argv);
void create(char ** argv);
void drawCircle(char ** argv);
void drawRectangle(char ** argv)
void drawTriangle(char ** argv)

int main(int argc, char **argv){

    // ./main [draw|-d] [circle/-c/rectangle/-r/triangle/-t] test.bmp

    if(argc == 3 && strcmp(argv[1], "info") == 0 || strcmp(argv[1], "-i") == 0){
        info(argv);
    } else if(argc == 4 && strcmp(argv[1], "copy") == 0 || strcmp(argv[1], "-cp") == 0){
        copy(argv);
    } else if(argc == 11 && strcmp(argv[1], "create") == 0 || strcmp(argv[1], "-c") == 0){
        create(argv);
    } else if(strcmp(argv[1], "draw") == 0 || strcmp(argv[1], "-d") == 0){
        if(strcmp(argv[2], "circle") == 0 || strcmp(argv[2], "-c") == 0){
            drawCircle(argv);
        } else if(strcmp(argv[2], "rectangle") == 0 || strcmp(argv[2], "-r") == 0){
            drawRectangle(argv);
        } else if(strcmp(argv[2], "triangle") == 0 || strcmp(argv[2], "-t") == 0){
            drawTriangle(argv);
        } else {
            fprintf(stderr, "%s: was not recognized as a valid command\n", argv[2]);
        }
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

void create(char ** argv){
    // ./main [create|-c] [width:/w:] int [height:/h:] int [color:/c:] r g b test.bmp
    if((strcmp(argv[2], "width:") == 0 || strcmp(argv[2], "w:") == 0) &&
        (strcmp(argv[4], "height:") == 0 || strcmp(argv[4], "h:") == 0) &&
        (strcmp(argv[6], "color:") == 0 || strcmp(argv[6], "c:") == 0)){

        RGB color;
        color.red = atoi(argv[7]);
        color.green = atoi(argv[8]);
        color.blue = atoi(argv[9]);

        uint32_t width = atoi(argv[3]);
        uint32_t height = atoi(argv[5]);

        Bitmap bitmap;
        uint32_t created = Bitmap_create(&bitmap,&color,width,height);

        if(created == 0){
            FILE * dest = fopen(argv[10], "wr");
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
    }
}