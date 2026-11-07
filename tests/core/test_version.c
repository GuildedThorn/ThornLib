#include "thorn/core/version.h"

#include "test.h"

static void test_version_string(void)
{
    THORN_TEST_CHECK(thorn_version_string() != NULL);
    THORN_TEST_CHECK_STR(thorn_version_string(), THORN_VERSION_STRING);
}

static void test_version_components(void)
{
    THORN_TEST_CHECK_INT(thorn_version_major(), THORN_VERSION_MAJOR);
    THORN_TEST_CHECK_INT(thorn_version_minor(), THORN_VERSION_MINOR);
    THORN_TEST_CHECK_INT(thorn_version_patch(), THORN_VERSION_PATCH);
}

int main(void)
{
    THORN_TEST_RUN(test_version_string);
    THORN_TEST_RUN(test_version_components);

    return thorn_test_report();
}
