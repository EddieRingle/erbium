#ifndef __included_er_internal_h
#define __included_er_internal_h

#include "erbium.h"

#include "ccan/json.h"
#include "ccan/list.h"
#include "uthash.h"

typedef struct er_app {
    int initialized;
    int running;

    char *name;
    char *author;
} er_app;

struct er_app_attrs {
    char *name;
    char *author;
};

struct er_context {
    er_context_attrs attrs;

#if defined(TARGET_OS_DESKTOP)
    GLFWwindow *window;
#endif
};

struct er_context_attrs {
    char *window_title;
    unsigned screen_width;
    unsigned screen_height;
    int fullscreen;
    int decorated;
};

typedef struct er_gfx {
    int i;
} er_gfx;

struct er_io_action {
    char action[64];
    int (*callback)(er_io_keyinfo keyinfo);

    UT_hash_handle hh;
};

struct er_io_triggermap {
    int trigger;
    struct er_io_action *action;

    UT_hash_handle hh;
};

typedef struct er_io {
    struct er_io_action *action_map;
    struct er_io_triggermap *trigger_map;
} er_io;

struct er_property_def {
    char name[64];
    er_prop_type type;
    struct er_property *instances;

    UT_hash_handle hh;
};

struct er_property {
    uint64_t key;
    struct er_property_def *definition;
    uint32_t array_count;
    union {
        int _bool;
        double _number;
        char *_string;
        void *_unknown;
    };

    UT_hash_handle hh;
};

struct er_entity {
    uint64_t id;
    uint32_t num_children;
    struct er_entity *parent;
    struct list_head children;
    struct list_node siblings;

    UT_hash_handle hh;
};

struct er_scene {
    struct er_entity *entity;

    void (*on_create)(er_entity root);
    void (*on_destroy)(er_entity root);
};

extern er_app *g_app;

extern er_context g_ctx;

extern er_gfx *g_gfx;

extern er_io *g_io;

extern er_scene g_rootscene;

extern struct er_property_def *g_property_definitions;

#define INITCHECK() \
    {if (g_app == NULL || !g_app->initialized) { return ERR_UNINITIALIZED; }}

void *er__malloc(size_t size);
void  er__free(void *mem);

void *er__memdup(const void *mem, size_t sz);

char *er__strdup(const char *str);
char *er__strtolower(const char *str);

typedef ERAPI (*er__ctx_open_f)(er_context *ctx);
typedef ERAPI (*er__ctx_close_f)(er_context *ctx);

typedef ERAPI (*er__gfx_init_f)(er_context *ctx);
typedef ERAPI (*er__gfx_quit_f)(er_context *ctx);
typedef ERAPI (*er__gfx_draw_f)(void);

typedef ERAPI (*er__io_init_f)(er_context *ctx);
typedef ERAPI (*er__io_quit_f)(er_context *ctx);
typedef ERAPI (*er__io_poll_f)(void);

extern er__ctx_open_f er__ctx_open;
extern er__ctx_close_f er__ctx_close;

extern er__gfx_init_f er__gfx_init;
extern er__gfx_quit_f er__gfx_quit;
extern er__gfx_draw_f er__gfx_draw;

extern er__io_init_f er__io_init;
extern er__io_quit_f er__io_quit;
extern er__io_poll_f er__io_poll;

#endif /* __included_er_internal_h */
