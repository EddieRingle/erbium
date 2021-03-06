#include "internal.h"

ERAPI er_ctx_attrs_init(er_context_attrs *attrs)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    *attrs = er__malloc(sizeof(struct er_context_attrs));
    if (*attrs == NULL) {
        return ERR_MEMORY_ERROR;
    }
    (*attrs)->window_title = NULL;
    er_ctx_attrs_set_window_title(attrs, NULL);
    (*attrs)->screen_width = 0;
    (*attrs)->screen_height = 0;
    (*attrs)->fullscreen = 0;
    (*attrs)->decorated = 1;
    (*attrs)->vsync = 0;

    return ERR_OK;
}

ERAPI er_ctx_attrs_set_window_title(er_context_attrs *attrs, const char *title)
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

ERAPI er_ctx_attrs_set_screen_width(er_context_attrs *attrs, unsigned width)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*attrs)->screen_width = width;

    return ERR_OK;
}

ERAPI er_ctx_attrs_set_screen_height(er_context_attrs *attrs, unsigned height)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*attrs)->screen_height = height;

    return ERR_OK;
}

ERAPI er_ctx_attrs_set_fullscreen(er_context_attrs *attrs, int fullscreen)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*attrs)->fullscreen = fullscreen;

    return ERR_OK;
}

ERAPI er_ctx_attrs_set_decorated(er_context_attrs *attrs, int decorated)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*attrs)->decorated = decorated;

    return ERR_OK;
}

ERAPI er_ctx_attrs_set_vsync(er_context_attrs *attrs, int vsync)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*attrs)->vsync = vsync;
    return ERR_OK;
}

ERAPI er_ctx_attrs_destroy(er_context_attrs *attrs)
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

ERAPI er_ctx_open(er_context_attrs *attrs, er_context *ctx)
{
    ERR ret;

    if (attrs == NULL || ctx == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*ctx) = er__malloc(sizeof(er_context));
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
    (*ctx)->attrs->vsync = (*attrs)->vsync;

    if ((ret = er__ctx_open(ctx)) != ERR_OK) {
        er_ctx_attrs_destroy(&(*ctx)->attrs);
        er__free(*ctx);
        return ret;
    }

    if ((ret = er__gfx_init(ctx)) != ERR_OK) {
        er_ctx_close(ctx);
        return ret;
    }

    if ((ret = er__io_init(ctx)) != ERR_OK) {
        er__gfx_quit(ctx);
        er_ctx_close(ctx);
        return ret;
    }

    return ERR_OK;
}

ERAPI er_ctx_close(er_context *ctx)
{
    ERR ret;

    if (ctx == NULL) {
        return ERR_INVALID_ARGS;
    }

    if (g_io != NULL) {
        if ((ret = er__io_quit(ctx)) != ERR_OK) {
            return ret;
        }
    }

    if (g_gfx != NULL) {
        if ((ret = er__gfx_quit(ctx)) != ERR_OK) {
            return ret;
        }
    }

    ret = er__ctx_close(ctx);

    if (g_ctx == *ctx) {
        g_ctx = NULL;
    }

    er_ctx_attrs_destroy(&(*ctx)->attrs);
    er__free(*ctx);
    *ctx = NULL;

    return ret;
}
