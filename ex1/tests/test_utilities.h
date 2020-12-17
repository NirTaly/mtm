#ifndef TEST_UTILITIES_H_
#define TEST_UTILITIES_H_

#include <stdbool.h>
#include <stdio.h>

/**
 * These macros are here to help you create tests more easily and keep them
 * clear.
 *
 * The basic idea with unit-testing is create a test function for every real
 * function and inside the test function declare some variables and execute the
 * function under test.
 *
 * Use the ASSERT_TEST and ASSERT_TEST_WITH_FREE to verify correctness of
 * values.
 */


/**
 * Evaluates expr and continues if expr is true.
 * If expr is false, ends the test by returning false and prints a detailed
 * message about the failure.
 */

#define COLOR_RED     "\x1b[01;31m"
#define COLOR_GREEN   "\x1b[01;32m"
#define COLOR_RESET   "\x1b[0m"

#define LOG_RED(X) { printf("%s %s %s\n",COLOR_RED,X,COLOR_RESET); }
#define LOG_GREEN(X) { printf("%s %s %s\n",COLOR_GREEN,X,COLOR_RESET); }

 #define ASSERT_TEST(expr, goto_label)                                                         \
     do {                                                                          \
         if (!(expr)) {                                                            \
             printf("\nAssertion failed at %s:%d %s ", __FILE__, __LINE__, #expr); \
             result = false;                                                       \
             goto goto_label;                                                         \
         }                                                                         \
     } while (0)

/**
 * Macro used for running a test from the main function
 */
#define RUN_TEST(test, name)                  \
    do {                                 \
      printf("Running %s ... ", name);   \
        if (test()) {                    \
            LOG_GREEN("[OK]");            \
        } else {                         \
            LOG_RED("[Failed]");        \
        }                                \
    } while (0)

#endif /* TEST_UTILITIES_H_ */
