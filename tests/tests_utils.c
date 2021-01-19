//
// Created by lara on 19/01/2021.
//

#include <stddef.h>
#include "utils.h"

void testCase1(){
    Point first;
    first.x_posn = 2;
    first.y_posn = 3;

    Point second;
    second.x_posn = 4;
    second.y_posn = 1;

    printf("Point 1:\n  x: %d\n  y: %d\n", first.x_posn, first.y_posn);
    printf("Point 2:\n  x: %d\n  y: %d\n", second.x_posn, second.y_posn);
}
void testCase2(){
    Point first;
    first.x_posn = 2;
    first.y_posn = 3;

    Point second;
    second.x_posn = 4;
    second.y_posn = 1;

    uint32_t distance = distance_ptp(&first, &second);
    assert_equal(3, distance, "Failed to calculate the distance of two points")
}
void testCase3(){
    Point first;
    first.x_posn = 4;
    first.y_posn = 4;

    Point second;
    second.x_posn = 4;
    second.y_posn = 4;

    uint32_t distance = distance_ptp(&first, &second);
    assert_equal(0, distance, "Failed to calculate the distance of two points")
}
void testCase4(){
    Point first;
    first.x_posn = 16;
    first.y_posn = 3;

    Point second;
    second.x_posn = 23;
    second.y_posn = 21;

    uint32_t distance = distance_ptp(&first, &second);
    assert_equal(19, distance, "Failed to calculate the distance of two points")
}
void testCase5(){
    Point first;
    first.x_posn = 0;
    first.y_posn = 0;

    Point second;
    second.x_posn = 17;
    second.y_posn = 23;

    uint32_t distance = distance_ptp(&first, &second);
    assert_equal(29, distance, "Failed to calculate the distance of two points")
}

int main(){
    //testCase1();
    testCase2();
    testCase3();
    testCase4();
    testCase5();
    return 0;
}