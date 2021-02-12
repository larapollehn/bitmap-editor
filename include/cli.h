//
// Created by lara on 10.02.21.
//

#ifndef LIBRARY_CLI_H
#define LIBRARY_CLI_H

char * features[] = {"create/c","draw/d", "info/i", "copy/cp", "grayscale/gs"};

char * drawOptions[] = {"circle/c", "rectangle/r", "triangle/t"};

char * drawCircle[] = {"radius=/r=1", "origin=/o=(x,y)", "color=/c=(r,g,b)"};

char * drawRect[] = {"pointE=/e=(x,y)", "pointF=/f=(x,y)", "pointG=/g=(x,y)", "pointH=/h=(x,y)", "color=/c=(r,g,b)"};

char * drawTriangle[] = {"pointE=/e=(x,y)", "pointF=/f=(x,y)", "pointG=/g=(x,y)", "color=/c=(r,g,b)"};

char * convolution[] = {"kernel=/k=[0,0,0,0,0,0,0,0,0]", "divider=/d=1"};

void run(int argc, char ** argv);

char * run(int argc, char **argv) {

}

#endif //LIBRARY_CLI_H
