#include "internal.h"

er_app_t *g_app = NULL;

void *er__malloc(size_t size)
{
    return malloc(size);
}

void er__free(void *mem)
{
    free(mem);
}

ERAPI er_init(void)
{
    if (g_app != NULL) {
        return ERR_ALREADY_INITIALIZED;
    }
    g_app = er__malloc(sizeof(er_app_t));
    if (g_app == NULL) {
        return ERR_MEMORY_ERROR;
    }
    g_app->initialized = 1;
    return ERR_OK;
}

ERAPI er_quit(void)
{
    INITCHECK();
    er__free(g_app);
    g_app = NULL;
    return ERR_OK;
}
