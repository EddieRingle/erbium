#include "internal.h"

char *er__strdup(const char *str)
{
    size_t len = 1 + strlen(str);
    char *p = er__malloc(len);
    return p ? memcpy(p, str, len) : NULL;
}

char *er__strtolower(const char *str)
{
    char *lowered = er__strdup(str);
    char *p = lowered;
    for (; *p; ++p) *p = tolower(*p);
    return lowered;
}
