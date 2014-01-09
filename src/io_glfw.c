#include "internal.h"

#if defined(TARGET_OS_DESKTOP)

er_io *g_io = NULL;

static void _glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    struct er_io_triggermap *trigger = NULL;
    er_io_keyinfo keyinfo;

    HASH_FIND_INT(g_io->trigger_map, &key, trigger);
    if (trigger != NULL && trigger->action != NULL && trigger->action->callback != NULL) {
        keyinfo.sym = key;
        keyinfo.state = action;
        keyinfo.x = 0;
        keyinfo.y = 0;
        trigger->action->callback(keyinfo);
    }
}

static void _glfw_mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
    _glfw_key_callback(window, button, -1, action, mods);
}

ERAPI er_io_init__glfw(er_context *ctx)
{
    INITCHECK();
    g_io = er__malloc(sizeof(er_io));
    if (g_io == NULL) {
        return ERR_MEMORY_ERROR;
    }
    g_io->action_map = NULL;
    g_io->trigger_map = NULL;
    glfwSetKeyCallback((*ctx)->window, &_glfw_key_callback);
    glfwSetMouseButtonCallback((*ctx)->window, &_glfw_mouse_callback);

    return ERR_OK;
}

ERAPI er_io_quit__glfw(er_context *ctx)
{
    struct er_io_action *action = NULL, *tmp_action = NULL;
    struct er_io_triggermap *trigger = NULL, *tmp_trigger = NULL;

    INITCHECK();
    HASH_ITER(hh, g_io->trigger_map, trigger, tmp_trigger) {
        HASH_DEL(g_io->trigger_map, trigger);
        er__free(trigger);
    }
    HASH_ITER(hh, g_io->action_map, action, tmp_action) {
        HASH_DEL(g_io->action_map, action);
        er__free(action);
    }
    er__free(g_io);
    g_io = NULL;

    return ERR_OK;
}

ERAPI er_io_poll__glfw(void)
{
    INITCHECK();
    glfwPollEvents();
    if (glfwWindowShouldClose(g_ctx->window)) {
        return er_stop();
    }

    return ERR_OK;
}

er__io_init_f er__io_init = &er_io_init__glfw;
er__io_quit_f er__io_quit = &er_io_quit__glfw;
er__io_poll_f er__io_poll = &er_io_poll__glfw;

#endif
