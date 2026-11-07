#include "thorn/core/version.h"

const char *thorn_version_string(void)
{
    return THORN_VERSION_STRING;
}

int thorn_version_major(void)
{
    return THORN_VERSION_MAJOR;
}

int thorn_version_minor(void)
{
    return THORN_VERSION_MINOR;
}

int thorn_version_patch(void)
{
    return THORN_VERSION_PATCH;
}
