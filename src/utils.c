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

    // https://math.stackexchange.com/questions/51326/determining-if-an-arbitrary-point-lies-inside-a-triangle-defined-by-three-points

    // Move Origin at one Vertex
    // Go with A
    B->x_posn = (B->x_posn - A->x_posn);
    B->y_posn = (B->y_posn - A->y_posn);

    C->x_posn = (C->x_posn - A->x_posn);
    C->y_posn = (C->y_posn - A->y_posn);

    P->y_posn = (P->x_posn - A->x_posn);
    P->y_posn = (P->y_posn - A->y_posn);

    // calculate the scalar d
    int32_t d = (B->x_posn * C->y_posn) - (C->x_posn * B->y_posn);

    // calculate the three Barycentric weights
    int32_t weight_A = (((P->x_posn * (B->y_posn - C->y_posn)) + (P->y_posn * (C->x_posn - B->x_posn)) + (B->x_posn * C->y_posn) - (C->x_posn * B->y_posn)) / d );
    int32_t weight_B = (((P->x_posn * C->y_posn) - (P->y_posn * C->x_posn)) / d );
    int32_t weight_C = (((P->y_posn * B->x_posn) - (P->x_posn * B->y_posn)) / d );

    // If all three weights are between 0 and 1, Point P is within/on the triangle ABC
    if((weight_A >= 0 && weight_A <= 1) && (weight_B >= 0 && weight_B <= 1) && (weight_C >= 0 && weight_C <= 1)){
        return 1;
    } else {
        return 0;
    }
}
