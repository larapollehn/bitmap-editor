//
// Created by lara on 02/01/2021.
//

#include "utils.h"
#include <math.h>

uint32_t distance_ptp(Point *first, Point *second) {
    double x_difference = pow(((double) second->x_posn - (double) first->x_posn), 2);
    double y_difference = pow(((double) second->y_posn - (double) first->y_posn), 2);

    // add 0,5 so the double will be rounded to the nearest int
    // https://stackoverflow.com/questions/9695329/c-how-to-round-a-double-to-an-int/9695338
    double distance = sqrt(x_difference + y_difference) + 0.5;

    return (uint32_t) distance;
}

uint8_t point_in_triangle(Point *A, Point *B, Point *C, Point *P) {

    // http://totologic.blogspot.com/2014/01/accurate-point-in-triangle-test.html

    // get the three orthogonal vectors based on triangle
    Point left_orthogonal_v1;
    left_orthogonal_v1.y_posn = (B->y_posn - A->x_posn);
    left_orthogonal_v1.x_posn = (A->x_posn - B->x_posn);

    Point left_orthogonal_v2;
    left_orthogonal_v2.y_posn = (C->y_posn - B->y_posn);
    left_orthogonal_v2.x_posn = (B->x_posn - C->x_posn);

    Point left_orthogonal_v3;
    left_orthogonal_v3.y_posn = (A->y_posn - C->y_posn);
    left_orthogonal_v3.x_posn = (C->x_posn - A->x_posn);

    // get the vectors based on triangle and point P
    Point v1;
    v1.x_posn = (P->x_posn - A->x_posn);
    v1.y_posn = (P->y_posn - A->y_posn);

    Point v2;
    v2.x_posn = (P->x_posn - B->x_posn);
    v2.y_posn = (P->y_posn - B->y_posn);

    Point v3;
    v3.x_posn = (P->x_posn - C->x_posn);
    v3.y_posn = (P->y_posn - C->y_posn);


    // compute the three dots
    int32_t dot1 = (left_orthogonal_v1.y_posn * v1.x_posn) + (left_orthogonal_v1.x_posn * v1.y_posn);
    int32_t dot2 = (left_orthogonal_v2.y_posn * v2.x_posn) + (left_orthogonal_v2.x_posn * v2.y_posn);
    int32_t dot3 = (left_orthogonal_v3.y_posn * v3.x_posn) + (left_orthogonal_v3.x_posn * v3.y_posn);

    // check if point therefore lies in triangle
    if(0 <= dot1 && 0 <= dot2 && 0 <= dot3){
        return 1;
    } else {
        return 0;
    }
}
