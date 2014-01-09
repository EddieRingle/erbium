#include "internal.h"

#if defined(TARGET_OS_DESKTOP)

ERAPI er_ctx_open__glfw(er_context *ctx)
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

ERAPI er_ctx_close__glfw(er_context *ctx)
{
    if ((*ctx)->window != NULL) {
        glfwDestroyWindow((*ctx)->window);
        (*ctx)->window = NULL;
    }

    return ERR_OK;
}

er__ctx_open_f er__ctx_open = &er_ctx_open__glfw;
er__ctx_close_f er__ctx_close = &er_ctx_close__glfw;

#endif
