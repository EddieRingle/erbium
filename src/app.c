#include "internal.h"

#if defined(TARGET_OS_MACOSX)
extern char **NXArgv;
#endif

er_app *g_app = NULL;

er_context g_ctx = NULL;

er_scene g_rootscene = NULL;

static er_scene *current_scene = NULL;

void *er__malloc(size_t size)
{
    return malloc(size);
}

void *er__realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}

void er__free(void *mem)
{
    free(mem);
}

struct referenced_block {
    unsigned owners;
    er_ptr_dcon dcon_fn;
    void *ptr;
};

void *er__obtain(size_t size, er_ptr_dcon dcon_fn)
{
    struct referenced_block *blk = er__malloc(sizeof(struct referenced_block));
    blk->owners = 1;
    blk->dcon_fn = dcon_fn;
    blk->ptr = er__malloc(size);
    return blk->ptr;
}

void er__retain(void *ptr)
{
    struct referenced_block *blk = container_of(ptr, struct referenced_block, ptr);
    blk->owners++;
}

void er__release(void *ptr)
{
    struct referenced_block *blk = container_of(ptr, struct referenced_block, ptr);
    if (--(blk->owners) < 1) {
        if (blk->dcon_fn != NULL) {
            blk->dcon_fn(ptr);
        } else {
            er__free(blk->ptr);
        }
        er__free(blk);
    }
}

void *er__memdup(const void *mem, size_t sz)
{
    void *p = er__malloc(sz);
    return p ? memcpy(p, mem, sz) : NULL;
}

void er__glfw_error_cb(int error, const char *description)
{
    LOGE("GLFW error: %s\n", description);
}

ERAPI er__create_default_context(er_context *ctx)
{
    ERR ret;
    er_context_attrs attrs;

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
    double now_time = 0, last_time = 0, delta = 0;
    struct er_subsystem *subsystem = NULL, *tmp = NULL;
    do {
        last_time = now_time;
        er_time(&now_time);
        delta = now_time - last_time;
        er__io_poll();
        HASH_ITER(hh, g_subsystems, subsystem, tmp) {
            if (subsystem->attrs.update != NULL) {
                subsystem->attrs.update(g_rootscene->entity, delta);
            }
        }
        er__gfx_draw();
    } while (g_app->running);
}

#if defined(TARGET_OS_ANDROID)
static int64_t er_init_time;

static uint64_t __get_raw_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t) tv.tv_sec * (uint64_t) 1000000 + (uint64_t) tv.tv_usec;
}
#endif

ERAPI er_time(double *time)
{
    if (time == NULL) {
        return ERR_INVALID_ARGS;
    }
#if defined(TARGET_OS_DESKTOP)
    *time = glfwGetTime();
    return ERR_OK;
#elif defined(TARGET_OS_ANDROID)
    *time = (double) (__get_raw_time() - er_init_time) * 1e-6;
    return ERR_OK;
#else
    return ERR_NOT_IMPLEMENTED;
#endif
}

ERAPI er_init(er_app_attrs *attrs)
{
    ERR ret;
    if (g_app != NULL) {
        return ERR_ALREADY_INITIALIZED;
    }
    g_app = er__malloc(sizeof(er_app));
    if (g_app == NULL) {
        return ERR_MEMORY_ERROR;
    }
    g_app->initialized = 1;
    g_app->running = 0;
    g_app->name = NULL;
    g_app->author = NULL;
    if (attrs != NULL) {
        if ((*attrs)->name == NULL || (*attrs)->author == NULL) {
            LOGE("You must call er_init() with both a name and author\n");
            return ERR_INVALID_ARGS;
        }
        g_app->name = er__strdup((*attrs)->name);
        g_app->author = er__strdup((*attrs)->author);
    }
#if defined(TARGET_OS_DESKTOP)
    if (!glfwInit()) {
        er_quit();
        return ERR_UNKNOWN;
    }
    glfwSetErrorCallback(er__glfw_error_cb);
#elif defined(TARGET_OS_ANDROID)
    er_init_time = __get_raw_time();
#endif
    if ((ret = er_scene_create(&g_rootscene)) != ERR_OK) {
        er_quit();
        return ret;
    }
    return ERR_OK;
}

static void delete_subsystems(void)
{
    struct er_subsystem *subsystem = NULL, *tmp = NULL;
    HASH_ITER(hh, g_subsystems, subsystem, tmp) {
        er_subsystem_unregister(&subsystem);
    }
}

ERAPI er_quit(void)
{
    INITCHECK();
    if (g_subsystems != NULL) {
        delete_subsystems();
    }
    if (g_rootscene != NULL) {
        er_scene_destroy(&g_rootscene);
        g_rootscene = NULL;
    }
    if (g_ctx != NULL) {
        er_ctx_close(&g_ctx);
        g_ctx = NULL;
    }
#if defined(TARGET_OS_DESKTOP)
    glfwTerminate();
#endif
    if (g_app->name != NULL) {
        er__free(g_app->name);
    }
    if (g_app->author != NULL) {
        er__free(g_app->author);
    }
    er__free(g_app);
    g_app = NULL;
    return ERR_OK;
}

ERAPI er_stop(void)
{
    INITCHECK();
    g_app->running = 0;

    return ERR_OK;
}

static void init_default_subsystems(void)
{
    er__subsystem_register_default_renderer();
}

#if defined(TARGET_OS_ANDROID)

ERAPI er_exec_android(er_context *ctx, er_scene *scene, struct android_app *state)
{
    return ERR_NOT_IMPLEMENTED;
}

#endif

#if defined(TARGET_OS_DESKTOP)

ERAPI er_exec_cli(er_context *ctx, er_scene *scene, int argc, char **argv)
{
    ERR ret;

    INITCHECK();
    if (ctx != NULL) {
        g_ctx = *ctx;
    } else {
        ret = er__create_default_context(&g_ctx);
        if (ret != ERR_OK) {
            return ret;
        }
    }
    g_app->running = 1;
    if (scene != NULL && *scene != NULL) {
        current_scene = scene;
        if ((*scene)->on_create != NULL) {
            (*scene)->on_create((*scene)->entity);
        }
        er_entity_add_child(&g_rootscene->entity, &(*scene)->entity);
    }
    init_default_subsystems();
    er__loop();
    return ERR_OK;
}

#endif

ERAPI er_app_attrs_init(er_app_attrs *attrs)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    *attrs = er__malloc(sizeof(struct er_app_attrs));
    if (*attrs == NULL) {
        return ERR_MEMORY_ERROR;
    }
    (*attrs)->name = NULL;
    (*attrs)->author = NULL;

    return ERR_OK;
}

ERAPI er_app_attrs_set_name(er_app_attrs *attrs, const char *name)
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

ERAPI er_app_attrs_set_author(er_app_attrs *attrs, const char *author)
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

ERAPI er_app_attrs_destroy(er_app_attrs *attrs)
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

static ERAPI er__app_get_binary_path(er_path_result *result)
{
    uint32_t buf_size = 2048;
    char temp_path[buf_size];
    char *ptr;
#if !defined(TARGET_OS_WINDOWS)
    char real_path[PATH_MAX];
#endif

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

#if defined(TARGET_OS_WINDOWS)
    result->path = er__strdup(temp_path);
#else
    realpath(temp_path, real_path);
    result->path = er__strdup(real_path);
#endif
    if (result->path == NULL) {
        return ERR_MEMORY_ERROR;
    }
    result->len = strlen(result->path);

    return ERR_OK;
}

static ERAPI er__app_get_support_path(er_path_result *result)
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

static ERAPI er__append_path_with_path(er_path_result *result, ERAPI (*base)(er_path_result *result), const char *path)
{
    ERR ret;
    char temp_path[2048];
#if !defined(TARGET_OS_WINDOWS)
    char real_path[PATH_MAX];
#endif
    if ((ret = base(result)) != ERR_OK) {
        return ret;
    }
    memset(temp_path, 0, sizeof temp_path);
    sprintf(temp_path, "%s/%s", result->path, path);
    er__free(result->path);
#if defined(TARGET_OS_WINDOWS)
    result->path = er__strdup(temp_path);
#else
    realpath(temp_path, real_path);
    result->path = er__strdup(real_path);
#endif
    if (result->path == NULL) {
        return ERR_MEMORY_ERROR;
    }
    result->len = strlen(result->path);
    return ERR_OK;
}

static ERAPI er__app_get_assets_path(er_path_result *result)
{
#if defined(TARGET_OS_MACOSX)
    return er__append_path_with_path(result, &er__app_get_binary_path, "../Resources/assets");
#else
    return er__append_path_with_path(result, &er__app_get_binary_path, "assets");
#endif
}

ERAPI er_app_get_path(er_path_type path, er_path_result *result)
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
        case ER_PATH_ASSETS:
            return er__app_get_assets_path(result);
        case ER_PATH_TEXTURES:
            return er__append_path_with_path(result, &er__app_get_assets_path, "textures");
        case ER_PATH_SHADERS:
            return er__append_path_with_path(result, &er__app_get_assets_path, "shaders");
        case ER_PATH_FONTS:
            return er__append_path_with_path(result, &er__app_get_assets_path, "fonts");
        case ER_PATH_SOUNDS:
            return er__append_path_with_path(result, &er__app_get_assets_path, "sounds");
        case ER_PATH_MUSIC:
            return er__append_path_with_path(result, &er__app_get_assets_path, "music");
    }
    return ERR_INVALID_PATH;
}

ERAPI er_app_cleanup_path_result(er_path_result *target)
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

ERAPI er_set_current_scene(er_scene *scene)
{
    if (current_scene != NULL && *current_scene != NULL) {
        if ((*current_scene)->on_destroy != NULL) {
            (*current_scene)->on_destroy((*scene)->entity);
        }
        er_entity_remove_from_parent(&(*scene)->entity);
    }
    current_scene = scene;
    if (scene != NULL && *scene != NULL) {
        if ((*scene)->on_create != NULL) {
            (*scene)->on_create((*scene)->entity);
        }
        er_entity_add_child(&g_rootscene->entity, &(*scene)->entity);
    }
    return ERR_OK;
}
