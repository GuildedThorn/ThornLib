#include <stdio.h>

#include "thorn/core/version.h"

int main(void)
{
    printf("thorn %s\n", thorn_version_string());
    return 0;
}
