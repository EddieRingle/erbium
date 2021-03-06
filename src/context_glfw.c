#include "internal.h"

void er__ctx_glfw_framebuffer_resize_cb(GLFWwindow *window, int width, int height)
{
    if (g_ctx != NULL && width >= 0 && height >= 0) {
        g_ctx->attrs->screen_width = (unsigned)width;
        g_ctx->attrs->screen_height = (unsigned)height;
    }
}

ERAPI er_ctx_open__glfw(er_context *ctx)
{
    GLFWmonitor *monitor = NULL;

    if ((*ctx)->attrs->fullscreen) {
        monitor = glfwGetPrimaryMonitor();
    }
    glfwWindowHint(GLFW_DECORATED, (*ctx)->attrs->decorated);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    (*ctx)->window = glfwCreateWindow((*ctx)->attrs->screen_width, (*ctx)->attrs->screen_height, (*ctx)->attrs->window_title, monitor, NULL);
    if (!(*ctx)->window) {
        return ERR_UNKNOWN;
    }
    glfwSetFramebufferSizeCallback((*ctx)->window, &er__ctx_glfw_framebuffer_resize_cb);
    glfwGetFramebufferSize((*ctx)->window, (int*)&(*ctx)->attrs->screen_width, (int*)&(*ctx)->attrs->screen_height);
    glfwMakeContextCurrent((*ctx)->window);
    glfwSwapInterval(((*ctx)->attrs->vsync)?1:0);

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
