#ifndef __included_er_internal_h
#define __included_er_internal_h

#include "erbium.h"

typedef struct er_app_t {
    int initialized;
} er_app_t;

extern er_app_t *g_app;

#define INITCHECK() \
    {if (g_app == NULL || !g_app->initialized) { return ERR_UNINITIALIZED; }}

void *er__malloc(size_t size);
void  er__free(void *mem);

#endif /* __included_er_internal_h */
