//
// Created by lara on 19/01/2021.
//

#include <stddef.h>
#include "utils.h"

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

/* Cases were the Point DOES lie in the triangle ABC */
void testCase6(){
    // Triangle Points
    Point A;
    A.x_posn = 1;
    A.y_posn = 5;

    Point B;
    B.x_posn = 7;
    B.y_posn = 5;

    Point C;
    C.x_posn = 4;
    C.y_posn = 0;

    // Point in question
    Point P;
    P.x_posn = 4;
    P.y_posn = 3;

    uint8_t inTriangle = point_in_triangle(&A, &B, &C, &P);
    assert_equal(1, inTriangle, "failed: point_in_triangle");
}
void testCase7(){
    // Triangle Points
    Point A;
    A.x_posn = 1;
    A.y_posn = 5;

    Point B;
    B.x_posn = 7;
    B.y_posn = 5;

    Point C;
    C.x_posn = 4;
    C.y_posn = 0;

    // Point in question
    Point P;
    P.x_posn = 1;
    P.y_posn = 5;

    uint8_t inTriangle = point_in_triangle(&A, &B, &C, &P);
    assert_equal(1, inTriangle, "failed: point_in_triangle");
}
void testCase8(){
    // Triangle Points
    Point A;
    A.x_posn = 1;
    A.y_posn = 5;

    Point B;
    B.x_posn = 7;
    B.y_posn = 5;

    Point C;
    C.x_posn = 4;
    C.y_posn = 0;

    // Point in question
    Point P;
    P.x_posn = 4;
    P.y_posn = 0;

    uint8_t inTriangle = point_in_triangle(&A, &B, &C, &P);
    assert_equal(1, inTriangle, "failed: point_in_triangle");
}
void testCase9(){
    // Triangle Points
    Point A;
    A.x_posn = 1;
    A.y_posn = 5;

    Point B;
    B.x_posn = 7;
    B.y_posn = 5;

    Point C;
    C.x_posn = 4;
    C.y_posn = 0;

    // Point in question
    Point P;
    P.x_posn = 7;
    P.y_posn = 5;

    uint8_t inTriangle = point_in_triangle(&A, &B, &C, &P);
    assert_equal(1, inTriangle, "failed: point_in_triangle");
}

/* Cases where the Point does NOT lie in the triangle ABC */
void testCase10(){
    // Triangle Points
    Point A;
    A.x_posn = 1;
    A.y_posn = 5;

    Point B;
    B.x_posn = 7;
    B.y_posn = 5;

    Point C;
    C.x_posn = 4;
    C.y_posn = 0;

    // Point in question
    Point P;
    P.x_posn = 45;
    P.y_posn = 33;

    uint8_t inTriangle = point_in_triangle(&A, &B, &C, &P);
    assert_equal(0, inTriangle, "failed: point_in_triangle");
}
void testCase11(){
    // Triangle Points
    Point A;
    A.x_posn = 1;
    A.y_posn = 5;

    Point B;
    B.x_posn = 7;
    B.y_posn = 5;

    Point C;
    C.x_posn = 4;
    C.y_posn = 0;

    // Point in question
    Point P;
    P.x_posn = -2;
    P.y_posn = 3;

    uint8_t inTriangle = point_in_triangle(&A, &B, &C, &P);
    assert_equal(0, inTriangle, "failed: point_in_triangle");
}
void testCase12(){
    // Triangle Points
    Point A;
    A.x_posn = 1;
    A.y_posn = 5;

    Point B;
    B.x_posn = 7;
    B.y_posn = 5;

    Point C;
    C.x_posn = 4;
    C.y_posn = 0;

    // Point in question
    Point P;
    P.x_posn = 100;
    P.y_posn = 25;

    uint8_t inTriangle = point_in_triangle(&A, &B, &C, &P);
    assert_equal(0, inTriangle, "failed: point_in_triangle");
}
void testCase13(){
    // Triangle Points
    Point A;
    A.x_posn = 1;
    A.y_posn = 5;

    Point B;
    B.x_posn = 7;
    B.y_posn = 5;

    Point C;
    C.x_posn = 4;
    C.y_posn = 0;

    // Point in question
    Point P;
    P.x_posn = 3;
    P.y_posn = 0;

    uint8_t inTriangle = point_in_triangle(&A, &B, &C, &P);
    assert_equal(0, inTriangle, "failed: point_in_triangle");
}
void testCase14(){
    // Triangle Points
    Point A;
    A.x_posn = 1;
    A.y_posn = 5;

    Point B;
    B.x_posn = 7;
    B.y_posn = 5;

    Point C;
    C.x_posn = 4;
    C.y_posn = 0;

    // Point in question
    Point P;
    P.x_posn = 8;
    P.y_posn = 5;

    uint8_t inTriangle = point_in_triangle(&A, &B, &C, &P);
    assert_equal(0, inTriangle, "failed: point_in_triangle");
}

int main(){
    testCase2();
    testCase3();
    testCase4();
    testCase5();
    testCase6();
    testCase7();
    testCase8();
    testCase9();
    testCase10();
    testCase11();
    testCase12();
    testCase13();
    testCase14();
    return 0;
}