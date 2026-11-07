#ifndef THORN_TEST_H
#define THORN_TEST_H

/*
 * Minimal, dependency-free test harness.
 *
 * One test executable per translation unit keeps the bookkeeping trivial:
 * include this header, write static void test_* functions, run them with
 * THORN_TEST_RUN in main, and return thorn_test_report().
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int thorn_test_checks = 0;
static int thorn_test_failures = 0;

#define THORN_TEST_CHECK(condition)                                            \
    do {                                                                       \
        ++thorn_test_checks;                                                   \
        if (!(condition)) {                                                    \
            ++thorn_test_failures;                                             \
            fprintf(stderr, "%s:%d: check failed: %s\n",                       \
                    __FILE__, __LINE__, #condition);                           \
        }                                                                      \
    } while (0)

#define THORN_TEST_CHECK_INT(actual, expected)                                 \
    do {                                                                       \
        long thorn_test_actual_ = (long)(actual);                              \
        long thorn_test_expected_ = (long)(expected);                          \
        ++thorn_test_checks;                                                   \
        if (thorn_test_actual_ != thorn_test_expected_) {                      \
            ++thorn_test_failures;                                             \
            fprintf(stderr,                                                     \
                    "%s:%d: expected %s == %ld, got %ld\n",                    \
                    __FILE__, __LINE__, #actual,                               \
                    thorn_test_expected_, thorn_test_actual_);                 \
        }                                                                      \
    } while (0)

#define THORN_TEST_CHECK_STR(actual, expected)                                 \
    do {                                                                       \
        const char *thorn_test_actual_ = (actual);                             \
        const char *thorn_test_expected_ = (expected);                         \
        ++thorn_test_checks;                                                   \
        if (thorn_test_actual_ == NULL || thorn_test_expected_ == NULL ||      \
            strcmp(thorn_test_actual_, thorn_test_expected_) != 0) {           \
            ++thorn_test_failures;                                             \
            fprintf(stderr,                                                     \
                    "%s:%d: expected %s == \"%s\", got \"%s\"\n",              \
                    __FILE__, __LINE__, #actual,                               \
                    thorn_test_expected_ == NULL ? "(null)"                    \
                                                 : thorn_test_expected_,       \
                    thorn_test_actual_ == NULL ? "(null)"                      \
                                               : thorn_test_actual_);          \
        }                                                                      \
    } while (0)

#define THORN_TEST_RUN(test_function)                                          \
    do {                                                                       \
        fprintf(stderr, "RUN  %s\n", #test_function);                          \
        test_function();                                                       \
    } while (0)

static int thorn_test_report(void)
{
    if (thorn_test_failures == 0) {
        printf("OK - %d checks passed\n", thorn_test_checks);
        return EXIT_SUCCESS;
    }

    fprintf(stderr, "FAIL - %d of %d checks failed\n",
            thorn_test_failures, thorn_test_checks);
    return EXIT_FAILURE;
}

#endif /* THORN_TEST_H */
