#include "internal.h"

#if defined(TARGET_OS_DESKTOP)

er_gfx *g_gfx = NULL;

ERAPI er_gfx_init__glfw(er_context *ctx)
{
    INITCHECK();
    g_gfx = er__malloc(sizeof(er_gfx));
    if (g_gfx == NULL) {
        return ERR_MEMORY_ERROR;
    }
    glewExperimental = 1;
    glewInit();

    return ERR_OK;
}

ERAPI er_gfx_quit__glfw(er_context *ctx)
{
    INITCHECK();
    er__free(g_gfx);
    g_gfx = NULL;

    return ERR_OK;
}

ERAPI er_gfx_draw__glfw(void)
{
    INITCHECK();
    glfwSwapBuffers(g_ctx->window);

    return ERR_OK;
}

er__gfx_init_f er__gfx_init = &er_gfx_init__glfw;
er__gfx_quit_f er__gfx_quit = &er_gfx_quit__glfw;
er__gfx_draw_f er__gfx_draw = &er_gfx_draw__glfw;

#endif
