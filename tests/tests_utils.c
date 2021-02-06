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

/*
 * test two points with acute angle
 */
void testCase15(){
    Point A;
    A.x_posn = -6;
    A.y_posn = 8;

    Point B;
    B.x_posn = 5;
    B.y_posn = 12;

    double product = dot_product(&A, &B);
    assert_equal(66, product, "Wrong: dot_product: %f", product);
}

/*
 * test two points with obtuse angle
 */
void testCase16(){
    Point A;
    A.x_posn = -6;
    A.y_posn = 8;

    Point B;
    B.x_posn = 12;
    B.y_posn = 1;

    double product = dot_product(&A, &B);
    assert_equal(-64, product, "Wrong: dot_product: %f", product);
}

/*
 * test two points that are orthogonal
 */
void testCase17(){
    Point A;
    A.x_posn = 7;
    A.y_posn = 0;

    Point B;
    B.x_posn = 0;
    B.y_posn = 3;

    double product = dot_product(&A, &B);
    assert_equal(0, product, "Wrong: dot_product: %f", product);
}

/*
 * Test point_in_rect
 * Point IS in rectangle
 */
void testCase18(){

    Point A;
    A.x_posn = 0;
    A.y_posn = 10;

    Point B;
    B.x_posn = 10;
    B.y_posn = 10;

    Point C;
    C.x_posn = 10;
    C.y_posn = 0;

    Point D;
    D.x_posn = 0;
    D.y_posn = 0;

    Point P;
    P.x_posn = 5;
    P.y_posn = 5;

    uint8_t pInRect = point_in_rect(&A, &B, &C, &D, &P);
    assert_equal(1, pInRect, "Failed: point_in_rect");
}

/*
 * Test point_in_rect
 * Point is NOT in rectangle
 */
void testCase19(){

    Point A;
    A.x_posn = 0;
    A.y_posn = 10;

    Point B;
    B.x_posn = 10;
    B.y_posn = 10;

    Point C;
    C.x_posn = 10;
    C.y_posn = 0;

    Point D;
    D.x_posn = 0;
    D.y_posn = 0;

    Point P;
    P.x_posn = 12;
    P.y_posn = -3;

    uint8_t pInRect = point_in_rect(&A, &B, &C, &D, &P);
    assert_equal(0, pInRect, "Failed: point_in_rect");
}

/*
 * Test point_in_rect
 * Point IS in rectangle
 */
void testCase20(){

    Point A;
    A.x_posn = 1;
    A.y_posn = 20;

    Point B;
    B.x_posn = 32;
    B.y_posn = 11;

    Point C;
    C.x_posn = 27;
    C.y_posn = -16;

    Point D;
    D.x_posn = -5;
    D.y_posn = -2;

    Point P;
    P.x_posn = 13;
    P.y_posn = 7;

    uint8_t pInRect = point_in_rect(&A, &B, &C, &D, &P);
    assert_equal(1, pInRect, "Failed: point_in_rect");
}

/*
 * Test point_in_rect
 * Point IS in rectangle
 */
void testCase21(){

    Point A;
    A.x_posn = 1;
    A.y_posn = 20;

    Point B;
    B.x_posn = 32;
    B.y_posn = 11;

    Point C;
    C.x_posn = 27;
    C.y_posn = -16;

    Point D;
    D.x_posn = -5;
    D.y_posn = -2;

    Point P;
    P.x_posn = 37;
    P.y_posn = 2;

    uint8_t pInRect = point_in_rect(&A, &B, &C, &D, &P);
    assert_equal(0, pInRect, "Failed: point_in_rect");
}

/*
 * Test reverse_array
 */
void testCase22(){
    uint8_t array[] =    {2,4,5,6,8,9,0,18,4,24};
    uint8_t reversed[] = {24,4,18,0,9,8,6,5,4,2};

    reverse_array(array, 10);

    for(int i = 0; i < 10; i++){
        assert_equal(reversed[i], array[i], "Failed: reverse_array");
    }

}

/*
 * Test reverse_array
 */
void testCase23(){
    uint32_t size = 255;
    uint8_t array[size];

    for(uint8_t i = 0; i < size; i++){
        array[i] = i;
    }

    reverse_array(array, size);

    for(uint32_t i = 0; i < size; i++){
        assert_equal((size-i-1), array[i], "Failed: reverse_array: %d %d %d", size-i, array[i], i);
    }
}

/*
 * Test reverse_array
 */
void testCase24(){
    uint8_t array[] =    {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    uint8_t reversed[] = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};

    reverse_array(array, 15);

    for(int i = 0; i < 15; i++){
        assert_equal(reversed[i], array[i], "Failed: reverse_array");
    }

}

int main(){
    /*
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
    testCase15();
    testCase16();
    testCase17();
    testCase18();
    testCase19();
    testCase20();
    testCase21();
     */
    //testCase22();
    testCase23();
    //testCase24();
    return 0;
}