//
// Created by lara on 02/01/2021.
//

#include "utils.h"
#include <math.h>

uint32_t distance_ptp(Point *first, Point *second) {

    /*
    printf("Point 1:\n  x: %d\n  y: %d\n", first->x_posn, first->y_posn);
    printf("Point 2:\n  x: %d\n  y: %d\n", second->x_posn, second->y_posn);
     */

    double x_difference = pow(((double) second->x_posn - (double) first->x_posn), 2);
    double y_difference = pow(((double) second->y_posn - (double) first->y_posn), 2);

    // add 0,5 so the double will be rounded to the nearest int
    // https://stackoverflow.com/questions/9695329/c-how-to-round-a-double-to-an-int/9695338
    double distance = sqrt(x_difference + y_difference) + 0.5;

    return (uint32_t) distance;
}
