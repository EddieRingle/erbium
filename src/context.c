#include "internal.h"

struct er_context_t {
    er_context_attrs_t attrs;

#if defined(TARGET_OS_DESKTOP)
    GLFWwindow *window;
#endif
};

struct er_context_attrs_t {
    char *window_title;
    unsigned screen_width;
    unsigned screen_height;
    int fullscreen;
    int decorated;
};

ERAPI er_ctx_attrs_init(er_context_attrs_t *attrs)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    *attrs = er__malloc(sizeof(struct er_context_attrs_t));
    if (*attrs == NULL) {
        return ERR_MEMORY_ERROR;
    }
    (*attrs)->window_title = NULL;
    er_ctx_attrs_set_window_title(attrs, NULL);
    (*attrs)->screen_width = 0;
    (*attrs)->screen_height = 0;
    (*attrs)->fullscreen = 0;
    (*attrs)->decorated = 1;

    return ERR_OK;
}

ERAPI er_ctx_attrs_set_window_title(er_context_attrs_t *attrs, const char *title)
{
    char tmp[256];

    INITCHECK();
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((*attrs)->window_title != NULL) {
        er__free((*attrs)->window_title);
    }
    memset(tmp, 0, sizeof tmp);
    if (title == NULL) {
        sprintf(tmp, "%s - Erbium %s", g_app->name, er_version_full);
    } else {
        strncpy(tmp, title, strlen(title));
    }
    (*attrs)->window_title = er__strdup(tmp);

    return ERR_OK;
}

ERAPI er_ctx_attrs_set_screen_width(er_context_attrs_t *attrs, unsigned width)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*attrs)->screen_width = width;

    return ERR_OK;
}

ERAPI er_ctx_attrs_set_screen_height(er_context_attrs_t *attrs, unsigned height)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*attrs)->screen_height = height;

    return ERR_OK;
}

ERAPI er_ctx_attrs_set_fullscreen(er_context_attrs_t *attrs, int fullscreen)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*attrs)->fullscreen = fullscreen;

    return ERR_OK;
}

ERAPI er_ctx_attrs_set_decorated(er_context_attrs_t *attrs, int decorated)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*attrs)->decorated = decorated;

    return ERR_OK;
}

ERAPI er_ctx_attrs_destroy(er_context_attrs_t *attrs)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((*attrs)->window_title != NULL) {
        er__free((*attrs)->window_title);
    }
    er__free(*attrs);
    *attrs = NULL;

    return ERR_OK;
}

#if defined(TARGET_OS_DESKTOP)

ERAPI er_ctx_open__glfw(er_context_t *ctx)
{
    GLFWmonitor *monitor = NULL;

    if ((*ctx)->attrs->fullscreen) {
        monitor = glfwGetPrimaryMonitor();
    }
    glfwWindowHint(GLFW_DECORATED, (*ctx)->attrs->decorated);
    (*ctx)->window = glfwCreateWindow(854, 480, (*ctx)->attrs->window_title, monitor, NULL);
    if (!(*ctx)->window) {
        return ERR_UNKNOWN;
    }
    glfwMakeContextCurrent((*ctx)->window);

    return ERR_OK;
}

#endif

ERAPI er_ctx_open(er_context_attrs_t *attrs, er_context_t *ctx)
{
    ERR ret = ERR_OK;

    if (attrs == NULL || ctx == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*ctx) = er__malloc(sizeof(er_context_t));
    if ((*ctx) == NULL) {
        return ERR_MEMORY_ERROR;
    }
    ret = er_ctx_attrs_init(&(*ctx)->attrs);
    if (ret != ERR_OK) {
        er__free(*ctx);
        return ERR_MEMORY_ERROR;
    }
    (*ctx)->attrs->window_title = er__strdup((*attrs)->window_title);
    if ((*ctx)->attrs->window_title == NULL) {
        er_ctx_attrs_destroy(&(*ctx)->attrs);
        er__free(*ctx);
        return ERR_MEMORY_ERROR;
    }
    (*ctx)->attrs->screen_height = (*attrs)->screen_height;
    (*ctx)->attrs->screen_width = (*attrs)->screen_width;
    (*ctx)->attrs->fullscreen = (*attrs)->fullscreen;
    (*ctx)->attrs->decorated = (*attrs)->decorated;

#if defined(TARGET_OS_DESKTOP)
    (*ctx)->window = NULL;
    if ((ret = er_ctx_open__glfw(ctx)) != ERR_OK) {
        er_ctx_attrs_destroy(&(*ctx)->attrs);
        er__free(*ctx);
        return ret;
    }
#endif

    return ERR_OK;
}

ERAPI er_ctx_close(er_context_t *ctx)
{
    if (ctx == NULL) {
        return ERR_INVALID_ARGS;
    }

#if defined(TARGET_OS_DESKTOP)
    if ((*ctx)->window != NULL) {
        glfwDestroyWindow((*ctx)->window);
        (*ctx)->window = NULL;
    }
#endif

    er_ctx_attrs_destroy(&(*ctx)->attrs);
    er__free(*ctx);
    *ctx = NULL;

    return ERR_OK;
}
