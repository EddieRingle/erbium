#include "internal.h"

#if defined(TARGET_OS_MACOSX)
extern char **NXArgv;
#endif

er_app_t *g_app = NULL;

er_context_t g_ctx = NULL;

void *er__malloc(size_t size)
{
    return malloc(size);
}

void er__free(void *mem)
{
    free(mem);
}

void er__glfw_error_cb(int error, const char *description)
{
    LOGE("GLFW error: %s\n", description);
}

ERAPI er__create_default_context(er_context_t *ctx)
{
    ERR ret = ERR_OK;
    er_context_attrs_t attrs;
    char title[255];

    if (ctx == NULL) {
        return ERR_INVALID_ARGS;
    }
    er_ctx_attrs_init(&attrs);
    er_ctx_attrs_set_screen_height(&attrs, 480);
    er_ctx_attrs_set_screen_width(&attrs, 854);
    ret = er_ctx_open(&attrs, ctx);
    er_ctx_attrs_destroy(&attrs);
    return ret;
}

void er__loop(void)
{
    do {
#if defined(TARGET_OS_DESKTOP)
        glfwPollEvents();

        if (glfwWindowShouldClose(g_ctx->window)) {
            g_app->running = 0;
        }
#endif
    } while (g_app->running);
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
    g_app->running = 0;
    g_app->name = NULL;
    g_app->author = NULL;
#if defined(TARGET_OS_DESKTOP)
    if (!glfwInit()) {
        er_quit();
        return ERR_UNKNOWN;
    }
    glfwSetErrorCallback(er__glfw_error_cb);
#endif
    return ERR_OK;
}

ERAPI er_quit(void)
{
    INITCHECK();
    if (g_ctx != NULL) {
        er_ctx_close(&g_ctx);
        g_ctx = NULL;
    }
#if defined(TARGET_OS_DESKTOP)
    glfwTerminate();
#endif
    er__free(g_app);
    g_app = NULL;
    return ERR_OK;
}

#if defined(TARGET_OS_ANDROID)

ERAPI er_exec_android(er_app_attrs_t *attrs, er_context_t *ctx, struct android_app *state)
{
    return ERR_NOT_IMPLEMENTED;
}

#endif

#if defined(TARGET_OS_DESKTOP)

ERAPI er_exec_cli(er_app_attrs_t *attrs, er_context_t *ctx, int argc, char **argv)
{
    ERR ret;

    INITCHECK();
    if (attrs != NULL) {
        if ((*attrs)->name == NULL || (*attrs)->author == NULL) {
            LOGE("You must call er_exec() with both a name and author\n");
            return ERR_INVALID_ARGS;
        }
        if (g_app->name != NULL) {
            er__free(g_app->name);
        }
        g_app->name = er__strdup((*attrs)->name);
        if (g_app->author != NULL) {
            er__free(g_app->author);
        }
        g_app->author = er__strdup((*attrs)->author);
    }
    if (ctx != NULL) {
        g_ctx = *ctx;
    } else {
        ret = er__create_default_context(&g_ctx);
        if (ret != ERR_OK) {
            return ret;
        }
    }
    g_app->running = 1;
    er__loop();
    return ERR_OK;
}

#endif

ERAPI er_app_attrs_init(er_app_attrs_t *attrs)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    *attrs = er__malloc(sizeof(struct er_app_attrs_t));
    if (*attrs == NULL) {
        return ERR_MEMORY_ERROR;
    }
    (*attrs)->name = NULL;
    (*attrs)->author = NULL;

    return ERR_OK;
}

ERAPI er_app_attrs_set_name(er_app_attrs_t *attrs, const char *name)
{
    if (attrs == NULL || name == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((*attrs)->name != NULL) {
        er__free((*attrs)->name);
    }
    (*attrs)->name = er__strdup(name);
    if ((*attrs)->name == NULL) {
        return ERR_MEMORY_ERROR;
    }

    return ERR_OK;
}

ERAPI er_app_attrs_set_author(er_app_attrs_t *attrs, const char *author)
{
    if (attrs == NULL || author == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((*attrs)->author != NULL) {
        er__free((*attrs)->author);
    }
    (*attrs)->author = er__strdup(author);
    if ((*attrs)->author == NULL) {
        return ERR_MEMORY_ERROR;
    }

    return ERR_OK;
}

ERAPI er_app_attrs_destroy(er_app_attrs_t *attrs)
{
    if (attrs == NULL || *attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((*attrs)->name != NULL) {
        er__free((*attrs)->name);
    }
    if ((*attrs)->author != NULL) {
        er__free((*attrs)->author);
    }
    er__free(*attrs);
    *attrs = NULL;

    return ERR_OK;
}

ERAPI er__app_get_binary_path(er_path_result_t *result)
{
    char temp_path[2048];
    char *ptr;

    memset(temp_path, 0, sizeof temp_path);
#if defined(TARGET_OS_WINDOWS)
    GetModuleFileNameA(NULL, temp_path, sizeof temp_path);
    if (strlen(temp_path)) {
        ptr = &temp_path[strlen(temp_path)];
        while (*(--ptr) != '\\');
        ptr[1] = '\x0';
    } else {
        strcpy(temp_path, ".\\");
    }
#elif defined(TARGET_OS_LINUX)
    readlink("/proc/self/exe", temp_path, (sizeof temp_path) - 1);
    if (strlen(temp_path)) {
        ptr = &temp_path[strlen(temp_path)];
        while (*(--ptr) != '/');
        ptr[1] = '\x0';
    } else {
        strcpy(temp_path, "./");
    }
#elif defined(TARGET_OS_MACOSX)
    strncpy(temp_path, NXArgv[0], sizeof temp_path);
    if (strlen(temp_path)) {
        ptr = &temp_path[strlen(temp_path)];
        while (*(--ptr) != '/');
        ptr[1] = '\x0';
    } else {
        strcpy(temp_path, "./");
    }
#endif

    result->path = er__strdup(temp_path);
    if (result->path == NULL) {
        return ERR_MEMORY_ERROR;
    }
    result->len = strlen(temp_path);

    return ERR_OK;
}

ERAPI er__app_get_support_path(er_path_result_t *result)
{
    char temp_path[2048];
#if defined(TARGET_OS_LINUX) || defined(TARGET_OS_MACOSX)
    const char *home_path;
#   if defined(TARGET_OS_LINUX)
    char *str;
#   endif
#endif

    memset(temp_path, 0, sizeof temp_path);
#if defined(TARGET_OS_WINDOWS)
    SHGetFolderPathA(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, temp_path);
    sprintf(temp_path, "%s\\%s\\%s\\", temp_path, g_app->author, g_app->name);
#elif defined(TARGET_OS_LINUX)
    home_path = getenv("HOME");
    str = er__strtolower(g_app->author);
    if (str == NULL) {
        return ERR_MEMORY_ERROR;
    }
    sprintf(temp_path, "%s/.%s/", home_path, str);
    er__free(str);
    str = er__strtolower(g_app->name);
    if (str == NULL) {
        return ERR_MEMORY_ERROR;
    }
    strcat(temp_path, str);
    er__free(str);
#elif defined(TARGET_OS_MACOSX)
    home_path = getenv("HOME");
    sprintf(temp_path, "%s/Library/Application Support/%s/%s", home_path, g_app->author, g_app->name);
#endif

    result->path = er__strdup(temp_path);
    if (result->path == NULL) {
        return ERR_MEMORY_ERROR;
    }
    result->len = strlen(temp_path);

    return ERR_OK;
}

ERAPI er_app_get_path(er_path_e path, er_path_result_t *result)
{
    if (result == NULL) {
        return ERR_UNREACHABLE_RESULT;
    }
    if (result->path != NULL) {
        /*
         * Result struct already has a path, so try to clean it up.
         * This could end badly if the struct has garbage in it,
         * for whatever reason.
         */
        er_app_cleanup_path_result(result);
    }
    switch (path) {
        case ER_PATH_BINARY:
            return er__app_get_binary_path(result);
        case ER_PATH_SUPPORT:
            return er__app_get_support_path(result);
    }
    return ERR_INVALID_PATH;
}

ERAPI er_app_cleanup_path_result(er_path_result_t *target)
{
    if (target != NULL) {
        if (target->path != NULL) {
            er__free(target->path);
            target->path = NULL;
        }
        target->len = 0;
    }
    return ERR_OK;
}
