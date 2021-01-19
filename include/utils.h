//
// Created by lara on 02/01/2021.
//

#ifndef BITMAP_EDITOR_UTILS_H
#define BITMAP_EDITOR_UTILS_H

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define MODE 0 // 0 = development, 1 = production

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: %s() : [System's message: %s] " M "\n", __FILE__, __LINE__, __FUNCTION__, clean_errno(), ##__VA_ARGS__); fflush(stderr);

#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: %s() : [System's message: %s] " M "\n", __FILE__, __LINE__, __FUNCTION__, clean_errno(), ##__VA_ARGS__); fflush(stderr)

#define log_info(M, ...) fprintf(stdout, "[INFO] (%s:%d: %s()) " M "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); fflush(stdout); fflush(stdout)

#define log_debug(M, ...) fprintf(stdout, "[DEBUG] (%s:%d: %s()): " M "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); fflush(stdout);

#define assert_equal(expected, actual, message, ...) if (MODE == 0 && expected != actual) { log_err(message,##__VA_ARGS__); exit(1); }

#define assert_true(condition, message, ...) if (!(condition)) { log_err(message,##__VA_ARGS__); exit(1); }

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error_handling;}

#define check_exit(A, M, ...) if(MODE == 0 && !(A)) {log_info(M, ##__VA_ARGS__); goto error_handling;}

#define check_mem(A) check((A), "Out of memory.")

#define ARR_SIZE(A) sizeof(A)/sizeof(A[0])

#define check_error(A, M, ARG ...) if(!(A)) {fprintf(stderr, "%s: %s\n", ARG, M); goto error_handling;}

#define check_file(A, M, ARG ...) if(!(A)) { if(mode == 0) fprintf(stderr, "%s: %s: %s\n", ARG[0], ARG[2], M); goto error_handling;}

/* https://stackoverflow.com/questions/3553296/sizeof-single-struct-member-in-c */
#define member_size(type, member) sizeof(((type *)0)->member)

typedef struct Point{
    uint32_t x_posn;
    uint32_t y_posn;
} Point;

/**
 * calculates the distance between two points in a 2d space
 * uses pythagoras
 * @param first point
 * @param second point
 * @return distance, rounded to the nearest integer
 */
uint32_t distance_ptp(Point * first, Point * second);

#endif //BITMAP_EDITOR_UTILS_H
