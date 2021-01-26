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

uint8_t point_in_triangle(const Point *A, const Point *B,const Point *C,const Point *P) {

    // https://math.stackexchange.com/questions/51326/determining-if-an-arbitrary-point-lies-inside-a-triangle-defined-by-three-points

    // check if P is one of the triangles corners
    if(A->x_posn == P->x_posn && A->y_posn == P->y_posn){
        return 1;
    }
    if(B->x_posn == P->x_posn && B->y_posn == P->y_posn){
        return 1;
    }
    if(C->x_posn == P->x_posn && C->y_posn == P->y_posn){
        return 1;
    }

    // Move Origin at one Vertex
    // Go with A
    fp_t b_x = (B->x_posn - A->x_posn);
    fp_t b_y = (B->y_posn - A->y_posn);

    fp_t c_x = (C->x_posn - A->x_posn);
    fp_t c_y = (C->y_posn - A->y_posn);

    fp_t p_x = (P->x_posn - A->x_posn);
    fp_t p_y = (P->y_posn - A->y_posn);

    // calculate the scalar d
    fp_t d = (b_x * c_y) - (c_x * b_y);

    // calculate the three Barycentric weights
    fp_t weight_A = (((p_x * (b_y - c_y)) + (p_y * (c_x - b_x)) + (b_x * c_y) - (c_x * b_y)) / d );
    fp_t weight_B = (((p_x * c_y) - (p_y * c_x)) / d );
    fp_t weight_C = (((p_y * b_x) - (p_x * b_y)) / d );


    // If all three weights are between 0 and 1, Point P is within/on the triangle ABC
    if((weight_A >= 0 && weight_A <= 1) && (weight_B >= 0 && weight_B <= 1) && (weight_C >= 0 && weight_C <= 1)){
        return 1;
    } else {
        return 0;
    }
}
