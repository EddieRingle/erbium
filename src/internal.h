#ifndef __included_er_internal_h
#define __included_er_internal_h

#include "erbium.h"

#include "cJSON.h"

typedef struct er_app_t {
    int initialized;
    int running;

    char *name;
    char *author;
} er_app_t;

struct er_app_attrs_t {
    char *name;
    char *author;
};

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

extern er_app_t *g_app;

extern er_context_t g_ctx;

#define INITCHECK() \
    {if (g_app == NULL || !g_app->initialized) { return ERR_UNINITIALIZED; }}

void *er__malloc(size_t size);
void  er__free(void *mem);

char *er__strdup(const char *str);
char *er__strtolower(const char *str);

#endif /* __included_er_internal_h */
