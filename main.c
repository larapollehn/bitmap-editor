//
// Created by lara on 02/01/2021.
//

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <bitmap.h>

void info(char **argv);

void copy(char **argv);

void create(char **argv);

void grayscale(char **argv);

void process(char **argv);

void drawCircle(char **argv);

void drawRectangle(char **argv);

void drawTriangle(char **argv);

void help();


int main(int argc, char **argv) {

    if(strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "?") == 0){
        help();
    }else if (argc == 3 && (strcmp(argv[1], "info") == 0 || strcmp(argv[1], "-i") == 0)) {
        info(argv);
    } else if (argc == 4 && (strcmp(argv[1], "copy") == 0 || strcmp(argv[1], "-cp") == 0)) {
        copy(argv);
    } else if (argc == 11 && (strcmp(argv[1], "create") == 0 || strcmp(argv[1], "-c") == 0)) {
        create(argv);
    } else if (strcmp(argv[1], "draw") == 0 || strcmp(argv[1], "-d") == 0) {
        // ./main [draw|-d] [circle/-c/rectangle/-r/triangle/-t] [...] test.bmp
        if (argc == 13 && (strcmp(argv[2], "circle") == 0 || strcmp(argv[2], "-c") == 0)) {
            drawCircle(argv);
        } else if (argc == 20 && (strcmp(argv[2], "rectangle") == 0 || strcmp(argv[2], "-r") == 0)) {
            drawRectangle(argv);
        } else if ( argc == 17 && (strcmp(argv[2], "triangle") == 0 || strcmp(argv[2], "-t") == 0)) {
            drawTriangle(argv);
        } else {
            fprintf(stderr, "%s: was not recognized as a valid command\n", argv[2]);
        }
    } else if (argc == 4 && (strcmp(argv[1], "grayscale") == 0 || strcmp(argv[1], "-gs") == 0)) {
        grayscale(argv);
    } else if (argc == 16 && (strcmp(argv[1], "convolution") == 0 || strcmp(argv[1], "-cv") == 0)) {
        process(argv);
    } else {
        if (argc < 3) {
            fprintf(stderr, "Not enough arguments were passed\n");
        } else {
            fprintf(stderr, "%s: was not recognized as a valid command\n", argv[1]);
        }
        return 1;
    }
    return 0;
}

void help(){
    char * name = "./lib";
    printf("usage: %s\n\n", name);
    printf("The following are common commands:\n\n");

    printf("get an overview of the bitmap infos (like headers)\n");
    printf("   info          %s [info|-i] <src-file>\n", name);
    printf("                 e.g.: '%s info lena.bmp'\n\n", name);

    printf("make a copy of an bitmap image\n");
    printf("   copy          %s [copy|-cp] <src-file> <dest-file>\n", name);
    printf("                 e.g.: '%s copy lena.bmp lena_copy.bmp'\n\n", name);

    printf("grayscale an image (naive (r+g+b)/3 )\n");
    printf("   grayscale     %s [grayscale|-gs] <src-file> <dest-file>\n", name);
    printf("                 e.g.: '%s grayscale lena.bmp lena_gray.bmp'\n\n", name);

    printf("create 24bpp images and draw geometric shapes on them\n");
    printf("   create        %s [create|-c] [width:|w:] int [height:/h:] int [color:/c:] int(r) int(g) int(b) <dest-file>\n", name);
    printf("                 e.g.: '%s create w: 200 h: 200 c: 241 136 201 pink_panther.bmp'\n\n", name);
    printf("   draw-circle   %s [draw|-d] [circle|-c] [radius:|r:] int [origin:/o:] int(x) int(y) [color:/c:] int(r) int(g) int(b) <dest-file>\n", name);
    printf("                 e.g.: '%s draw circle r: 100 o: 50 75 c: 247 255 173 pink_panther.bmp'\n\n", name);
    printf("   draw-rect     %s [draw|-d] [rectangle|-r] [pointE:/e:] int(x) int(y) [pointF:/f:] int(x) int(y) [pointG:/g:] int(x) int(y) [pointH:/h:] int(x) int(y) [color:/c:] int(r) int(g) int(b) <dest-file>\n", name);
    printf("                 e.g.: '%s draw rectangle e: 50 50 f: 50 100 g: 100 100 h: 100 50 c: 71 167 255 pink_panther.bmp'\n\n", name);
    printf("   draw-tri      %s [draw|-d] [triangle|-t] [pointE:/e:] int(x) int(y) [pointF:/f:] int(x) int(y) [pointG:/g:] int(x) int(y) [color:/c:] int(r) int(g) int(b) <dest-file>\n", name);
    printf("                 e.g.: '%s draw triangle e: 50 150 f: 20 300 g: 300 300 c: 16 95 74 pink_panther.bmp'\n\n", name);

    printf("simple image processing using image convolution and an 3x3 kernel\n");
    printf("   convolution   %s [convolution|-cv] [kernel:|k:] int int int int int int int int int [divider:|d:] int <src-file> <dest-file>\n", name);
    printf("                 e.g.: '%s convolution k: 1 0 -1 0 0 0 -1 0 1 d: 1 lena.bmp lena_edge_detect.bmp'\n\n", name);
}


void info(char **argv) {
    // ./main [info|-i] lena.bmp

    FILE *source = fopen(argv[2], "rb");
    if (source != NULL) {
        Bitmap bitmap;
        uint32_t scanned = Bitmap_scan(source, &bitmap);
        if (scanned == 0) {
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

void copy(char **argv) {
    // ./main [copy|-cp] lena.bmp lena_copy.bmp

    FILE *source = fopen(argv[2], "rb");
    if (source != NULL) {
        Bitmap bitmap;
        uint32_t scanned = Bitmap_scan(source, &bitmap);
        if (scanned == 0) {
            FILE *dest = fopen(argv[3], "wr");
            uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
            if (copied == 0) {
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

void create(char **argv) {
    // ./main [create|-c] [width:/w:] int [height:/h:] int [color:/c:] int int int test.bmp
    if ((strcmp(argv[2], "width:") == 0 || strcmp(argv[2], "w:") == 0) &&
        (strcmp(argv[4], "height:") == 0 || strcmp(argv[4], "h:") == 0) &&
        (strcmp(argv[6], "color:") == 0 || strcmp(argv[6], "c:") == 0)) {

        RGB color;
        color.red = atoi(argv[7]);
        color.green = atoi(argv[8]);
        color.blue = atoi(argv[9]);

        uint32_t width = atoi(argv[3]);
        uint32_t height = atoi(argv[5]);

        Bitmap bitmap;
        uint32_t created = Bitmap_create(&bitmap, &color, width, height);

        if (created == 0) {
            FILE *dest = fopen(argv[10], "wb");
            uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
            if (copied == 0) {
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

void grayscale(char **argv) {

    // ./lib [grayscale/-gs] test.bmp test_gray.bmp

    FILE *source = fopen(argv[2], "rb");
    if (source != NULL) {
        Bitmap bitmap;
        uint32_t scanned = Bitmap_scan(source, &bitmap);
        if (scanned == 0 && bitmap.biBitCount == 24) {

            if (bitmap.colorTable_size == 0) {
                uint32_t grayPixels = Bitmap_naive_grayscaling_px(&bitmap);
                if (grayPixels == 1) {
                    fprintf(stderr, "That did not work\n");
                }
            } else {
                uint32_t grayTable = Bitmap_naive_grayscaling_ct(&bitmap);
                if (grayTable == 1) {
                    fprintf(stderr, "That did not work\n");
                }
            }

            FILE *dest = fopen(argv[3], "wr");
            uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
            if (copied == 0) {
                Bitmap_destroy(&bitmap);
                fclose(dest);
            } else {
                fprintf(stderr, "That did not work\n");
            }
        } else {
            fprintf(stderr, "That did not work\n");
        }
    } else {
        fprintf(stderr, "That did not work. Images must be 24bpp\n");
    }
}

void process(char **argv) {
    // ./lib [convolution/-cv] [kernel:/k:] int int int int int int int int int [divider:/d:] int test.bmp test_conv.bmp

    if ((strcmp(argv[2], "kernel:") == 0 || strcmp(argv[2], "k:") == 0) &&
        (strcmp(argv[12], "divider:") == 0 || strcmp(argv[12], "d:") == 0)) {

        FILE * source = fopen(argv[14], "rb");

        Bitmap bitmap;
        uint32_t scanned = Bitmap_scan(source, &bitmap);
        if(scanned == 0 && bitmap.biBitCount == 24){
            int32_t kernel[9];
            kernel[0] = atoi(argv[3]);
            kernel[1] = atoi(argv[4]);
            kernel[2] = atoi(argv[5]);

            kernel[3] = atoi(argv[6]);
            kernel[4] = atoi(argv[7]);
            kernel[5] = atoi(argv[8]);

            kernel[6] = atoi(argv[9]);
            kernel[7] = atoi(argv[10]);
            kernel[8] = atoi(argv[11]);

            float divider = atoi(argv[13]);

            uint32_t processed = Bitmap_convolution(&bitmap, kernel, divider);
            if(processed == 0){
                FILE * dest = fopen(argv[15], "wb");
                uint32_t written = Bitmap_copyIntoFile(dest, &bitmap);
                if(written == 1){
                    fprintf(stderr, "That did not work\n");
                }
                fclose(source);
                fclose(dest);
                Bitmap_destroy(&bitmap);
            } else {
                fprintf(stderr, "That did not work\n");
            }
        } else {
            fprintf(stderr, "That did not work. Images must be 24bpp\n");
            fclose(source);
        }
    }
}

void drawCircle(char **argv) {
    // ./main [draw|-d] [circle/-c] [radius:/r:] int [origin:/o:] int int [color:/c:] int int int test.bmp

    FILE *source = fopen(argv[12], "rb");
    rewind(source);

    if (source != NULL) {
        if ((strcmp(argv[3], "radius:") == 0 || strcmp(argv[3], "r:") == 0) &&
            (strcmp(argv[5], "origin:") == 0 || strcmp(argv[5], "o:") == 0) &&
            (strcmp(argv[8], "color:") == 0 || strcmp(argv[8], "c:") == 0)) {

            Bitmap bitmap;
            uint32_t scanned = Bitmap_scan(source, &bitmap);
            if (scanned == 0) {
                RGB color;
                color.red = atoi(argv[9]);
                color.green = atoi(argv[10]);
                color.blue = atoi(argv[11]);

                uint32_t radius = atoi(argv[4]);

                Point origin;
                origin.x_posn = atoi(argv[6]);
                origin.y_posn = atoi(argv[7]);

                uint32_t drawn = Bitmap_draw_circle(&bitmap, &origin, radius, &color);
                if (drawn == 0) {
                    FILE * dest = fopen(argv[12], "wb");
                    rewind(dest);

                    uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
                    if(copied == 1){
                        fprintf(stderr, "That did not work\n");
                    }

                } else {
                    fprintf(stderr, "That did not work\n");
                }
            } else {
                fprintf(stderr, "That did not work\n");
            }
        }
    } else {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    }
}

void drawRectangle(char **argv) {
    // ./main [draw|-d] [rectangle/-r] [pointE:/e:] int int [pointF:/f:] int int [pointG:/g:] int int [pointH:/h:] int int [color:/c:] int int int test.bmp

    FILE *source = fopen(argv[19], "rb");
    rewind(source);

    if (source != NULL) {
        if ((strcmp(argv[3], "pointE:") == 0 || strcmp(argv[3], "e:") == 0) &&
            (strcmp(argv[6], "pointF:") == 0 || strcmp(argv[6], "f:") == 0) &&
            (strcmp(argv[9], "pointG:") == 0 || strcmp(argv[9], "g:") == 0) &&
            (strcmp(argv[12], "pointH:") == 0 || strcmp(argv[12], "h:") == 0)) {

            Bitmap bitmap;
            uint32_t scanned = Bitmap_scan(source, &bitmap);
            if (scanned == 0) {

                RGB color;
                color.red = atoi(argv[16]);
                color.green = atoi(argv[17]);
                color.blue = atoi(argv[18]);

                Point E;
                E.x_posn = atoi(argv[4]);
                E.y_posn = atoi(argv[5]);

                Point F;
                F.x_posn = atoi(argv[7]);
                F.y_posn = atoi(argv[8]);

                Point G;
                G.x_posn = atoi(argv[10]);
                G.y_posn = atoi(argv[11]);

                Point H;
                H.x_posn = atoi(argv[13]);
                H.y_posn = atoi(argv[14]);

                uint32_t drawn = Bitmap_draw_rect(&bitmap, &E, &F, &G, &H, &color);
                if (drawn == 0) {
                    FILE * dest = fopen(argv[19], "wb");
                    rewind(dest);

                    uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
                    if(copied == 1){
                        fprintf(stderr, "That did not work\n");
                    }
                } else {
                    fprintf(stderr, "That did not work\n");
                }
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

void drawTriangle(char **argv) {
    // ./main [draw|-d] [triangle/-t] [pointE:/e:] int int [pointF:/f:] int int [pointG:/g:] int int [color:/c:] int int int test.bmp

    FILE *source = fopen(argv[16], "rb");
    rewind(source);

    if (source != NULL) {
        if ((strcmp(argv[3], "pointE:") == 0 || strcmp(argv[3], "e:") == 0) &&
            (strcmp(argv[6], "pointF:") == 0 || strcmp(argv[6], "f:") == 0) &&
            (strcmp(argv[9], "pointG:") == 0 || strcmp(argv[9], "g:") == 0)) {

            Bitmap bitmap;
            uint32_t scanned = Bitmap_scan(source, &bitmap);
            if (scanned == 0) {

                RGB color;
                color.red = atoi(argv[13]);
                color.green = atoi(argv[14]);
                color.blue = atoi(argv[15]);

                Point E;
                E.x_posn = atoi(argv[4]);
                E.y_posn = atoi(argv[5]);

                Point F;
                F.x_posn = atoi(argv[7]);
                F.y_posn = atoi(argv[8]);

                Point G;
                G.x_posn = atoi(argv[10]);
                G.y_posn = atoi(argv[11]);


                uint32_t drawn = Bitmap_draw_triangle(&bitmap, &E, &F, &G, &color);
                if (drawn == 0) {
                    FILE * dest = fopen(argv[16], "wb");
                    rewind(dest);

                    uint32_t copied = Bitmap_copyIntoFile(dest, &bitmap);
                    if(copied == 1){
                        fprintf(stderr, "That did not work\n");
                    }
                } else {
                    fprintf(stderr, "That did not work\n");
                }
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
