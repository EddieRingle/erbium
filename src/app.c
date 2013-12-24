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
    g_app->name = NULL;
    g_app->author = NULL;
    return ERR_OK;
}

ERAPI er_quit(void)
{
    INITCHECK();
    er__free(g_app);
    g_app = NULL;
    return ERR_OK;
}

#if defined(TARGET_OS_ANDROID)

ERAPI er_exec_android(er_app_attrs_t *attrs, struct android_app *state)
{
    return ERR_NOT_IMPLEMENTED;
}

#endif

#if defined(TARGET_OS_DESKTOP)

ERAPI er_exec_cli(er_app_attrs_t *attrs, int argc, char **argv)
{
    INITCHECK();
    if (attrs != NULL) {
        if (attrs->name != NULL) {
            if (g_app->name != NULL) {
                er__free(g_app->name);
            }
            g_app->name = er__strdup(attrs->name);
        }
        if (attrs->author != NULL) {
            if (g_app->author != NULL) {
                er__free(g_app->author);
            }
            g_app->author = er__strdup(attrs->author);
        }
    }
    /* TODO: Start making things happen */
    return ERR_OK;
}

#endif
