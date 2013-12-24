#include "internal.h"

#if defined(TARGET_OS_MACOSX)
extern char **NXArgv;
#endif

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

ERAPI er__app_get_binary_path(er_path_result_t *result)
{
    char temp_path[2048];
#if defined(TARGET_OS_LINUX) || defined(TARGET_OS_MACOSX)
    const char *home_path;
    char *str;
#endif
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
    char *ptr;
#   if defined(TARGET_OS_LINUX)
    char *str;
#   endif
#endif

    memset(temp_path, 0, sizeof temp_path);
#if defined(TARGET_OS_WINDOWS)
    SHGetFolderPathA(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, temp_path);
    if (g_app->author != NULL && g_app->name != NULL) {
        sprintf(temp_path, "%s\\%s\\%s\\", temp_path, g_app->author, g_app->name);
    } else if (g_app->author != NULL) {
        sprintf(temp_path, "%s\\%s\\erbiumApp\\", temp_path, g_app->author);
    } else if (g_app->name != NULL) {
        sprintf(temp_path, "%s\\erbium\\%s", temp_path, g_app->name);
    }
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
