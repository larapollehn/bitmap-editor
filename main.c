//
// Created by lara on 02/01/2021.
//

#include <stdio.h>

int main(int argc, char **argv){
    printf("Hello World\n");
    printf("Count: %d\n", argc);
    for(int i = 0; i < argc; i++){
        printf("Argument %d: %s\n", i, argv[i]);
    }
    return 0;
}