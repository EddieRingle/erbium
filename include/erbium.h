#ifndef __included_erbium_h
#define __included_erbium_h

#include "platform_detect.h"

#if defined(TARGET_OS_ANDROID)
#   define TARGET_OS_MOBILE
#else
#   define TARGET_OS_DESKTOP
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#if defined(TARGET_OS_WINDOWS)
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   include <shlobj.h>
#else
#   include <sys/stat.h>
#   include <sys/types.h>
#   include <unistd.h>
#   include <signal.h>
#endif

/*
 * Freetype
 */
#include <ft2build.h>
#include FT_FREETYPE_H

/*
 * GLEW
 */
#define GLEW_STATIC
#include <GL/glew.h>

/*
 * GLFW
 */
#include <GLFW/glfw3.h>

/*
 * zlib & libpng
 */
#include <zlib.h>
#include <png.h>

/*
 * Lua
 */
#include <lua5.1/lua.h>
#include <lua5.1/luaconf.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>

/*
 * OpenAL Soft
 */
#if defined(TARGET_OS_MACOSX)
#   include <OpenAL/al.h>
#   include <OpenAL/alc.h>
#else
#   define AL_LIBTYPE_STATIC
#   include <AL/al.h>
#   include <AL/alc.h>
#endif

/*
 * Ogg
 */
#if defined(TARGET_LITTLE_ENDIAN)
#   define OGG_ENDIAN 0
#elif defined(TARGET_BIG_ENDIAN)
#   define OGG_ENDIAN 1
#endif
#include <ogg/ogg.h>

/*
 * Vorbis
 */
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

#include "erbium_version.h"

#define LOGI(...) (fprintf(stdout, __VA_ARGS__))
#define LOGE(...) (fprintf(stderr, __VA_ARGS__))

#if defined(TARGET_OS_ANDROID)
#   define er_main() void android_main(struct android_app *state)
#   define er_return_main()
#else
#   define er_main() int main(int argc, char **argv)
#   define er_return_main() return 0
#endif

typedef enum {
    ERR_OK = 0,

    ERR_UNINITIALIZED,
    ERR_MEMORY_ERROR,
    ERR_ALREADY_INITIALIZED,
    ERR_NOT_IMPLEMENTED,
    ERR_UNREACHABLE_RESULT,
    ERR_INVALID_PATH,
    ERR_INVALID_ARGS,
    ERR_KIDNAPPER,
    ERR_NO_PARENT,
    ERR_NOT_FOUND,
    ERR_PROP_TYPE_MISMATCH,
    ERR_UNKNOWN
} ERR;

#define ERAPI ERR

/*
 * Key mappings borrowed from GLFW
 */
/* The unknown key */
#define ER_KEY_UNKNOWN            -1

/* Printable keys */
#define ER_KEY_SPACE              32
#define ER_KEY_APOSTROPHE         39  /* ' */
#define ER_KEY_COMMA              44  /* , */
#define ER_KEY_MINUS              45  /* - */
#define ER_KEY_PERIOD             46  /* . */
#define ER_KEY_SLASH              47  /* / */
#define ER_KEY_0                  48
#define ER_KEY_1                  49
#define ER_KEY_2                  50
#define ER_KEY_3                  51
#define ER_KEY_4                  52
#define ER_KEY_5                  53
#define ER_KEY_6                  54
#define ER_KEY_7                  55
#define ER_KEY_8                  56
#define ER_KEY_9                  57
#define ER_KEY_SEMICOLON          59  /* ; */
#define ER_KEY_EQUAL              61  /* = */
#define ER_KEY_A                  65
#define ER_KEY_B                  66
#define ER_KEY_C                  67
#define ER_KEY_D                  68
#define ER_KEY_E                  69
#define ER_KEY_F                  70
#define ER_KEY_G                  71
#define ER_KEY_H                  72
#define ER_KEY_I                  73
#define ER_KEY_J                  74
#define ER_KEY_K                  75
#define ER_KEY_L                  76
#define ER_KEY_M                  77
#define ER_KEY_N                  78
#define ER_KEY_O                  79
#define ER_KEY_P                  80
#define ER_KEY_Q                  81
#define ER_KEY_R                  82
#define ER_KEY_S                  83
#define ER_KEY_T                  84
#define ER_KEY_U                  85
#define ER_KEY_V                  86
#define ER_KEY_W                  87
#define ER_KEY_X                  88
#define ER_KEY_Y                  89
#define ER_KEY_Z                  90
#define ER_KEY_LEFT_BRACKET       91  /* [ */
#define ER_KEY_BACKSLASH          92  /* \ */
#define ER_KEY_RIGHT_BRACKET      93  /* ] */
#define ER_KEY_GRAVE_ACCENT       96  /* ` */
#define ER_KEY_WORLD_1            161 /* non-US #1 */
#define ER_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define ER_KEY_ESCAPE             256
#define ER_KEY_ENTER              257
#define ER_KEY_TAB                258
#define ER_KEY_BACKSPACE          259
#define ER_KEY_INSERT             260
#define ER_KEY_DELETE             261
#define ER_KEY_RIGHT              262
#define ER_KEY_LEFT               263
#define ER_KEY_DOWN               264
#define ER_KEY_UP                 265
#define ER_KEY_PAGE_UP            266
#define ER_KEY_PAGE_DOWN          267
#define ER_KEY_HOME               268
#define ER_KEY_END                269
#define ER_KEY_CAPS_LOCK          280
#define ER_KEY_SCROLL_LOCK        281
#define ER_KEY_NUM_LOCK           282
#define ER_KEY_PRINT_SCREEN       283
#define ER_KEY_PAUSE              284
#define ER_KEY_F1                 290
#define ER_KEY_F2                 291
#define ER_KEY_F3                 292
#define ER_KEY_F4                 293
#define ER_KEY_F5                 294
#define ER_KEY_F6                 295
#define ER_KEY_F7                 296
#define ER_KEY_F8                 297
#define ER_KEY_F9                 298
#define ER_KEY_F10                299
#define ER_KEY_F11                300
#define ER_KEY_F12                301
#define ER_KEY_F13                302
#define ER_KEY_F14                303
#define ER_KEY_F15                304
#define ER_KEY_F16                305
#define ER_KEY_F17                306
#define ER_KEY_F18                307
#define ER_KEY_F19                308
#define ER_KEY_F20                309
#define ER_KEY_F21                310
#define ER_KEY_F22                311
#define ER_KEY_F23                312
#define ER_KEY_F24                313
#define ER_KEY_F25                314
#define ER_KEY_KP_0               320
#define ER_KEY_KP_1               321
#define ER_KEY_KP_2               322
#define ER_KEY_KP_3               323
#define ER_KEY_KP_4               324
#define ER_KEY_KP_5               325
#define ER_KEY_KP_6               326
#define ER_KEY_KP_7               327
#define ER_KEY_KP_8               328
#define ER_KEY_KP_9               329
#define ER_KEY_KP_DECIMAL         330
#define ER_KEY_KP_DIVIDE          331
#define ER_KEY_KP_MULTIPLY        332
#define ER_KEY_KP_SUBTRACT        333
#define ER_KEY_KP_ADD             334
#define ER_KEY_KP_ENTER           335
#define ER_KEY_KP_EQUAL           336
#define ER_KEY_LEFT_SHIFT         340
#define ER_KEY_LEFT_CONTROL       341
#define ER_KEY_LEFT_ALT           342
#define ER_KEY_LEFT_SUPER         343
#define ER_KEY_RIGHT_SHIFT        344
#define ER_KEY_RIGHT_CONTROL      345
#define ER_KEY_RIGHT_ALT          346
#define ER_KEY_RIGHT_SUPER        347
#define ER_KEY_MENU               348
#define ER_KEY_LAST               ER_KEY_MENU

#define ER_MOD_SHIFT              0x0001
#define ER_MOD_CONTROL            0x0002
#define ER_MOD_ALT                0x0004
#define ER_MOD_SUPER              0x0008

#define ER_MOUSE_BUTTON_1         0
#define ER_MOUSE_BUTTON_2         1
#define ER_MOUSE_BUTTON_3         2
#define ER_MOUSE_BUTTON_4         3
#define ER_MOUSE_BUTTON_5         4
#define ER_MOUSE_BUTTON_6         5
#define ER_MOUSE_BUTTON_7         6
#define ER_MOUSE_BUTTON_8         7
#define ER_MOUSE_BUTTON_LAST      ER_MOUSE_BUTTON_8
#define ER_MOUSE_BUTTON_LEFT      ER_MOUSE_BUTTON_1
#define ER_MOUSE_BUTTON_RIGHT     ER_MOUSE_BUTTON_2
#define ER_MOUSE_BUTTON_MIDDLE    ER_MOUSE_BUTTON_3

#define ER_JOYSTICK_1             0
#define ER_JOYSTICK_2             1
#define ER_JOYSTICK_3             2
#define ER_JOYSTICK_4             3
#define ER_JOYSTICK_5             4
#define ER_JOYSTICK_6             5
#define ER_JOYSTICK_7             6
#define ER_JOYSTICK_8             7
#define ER_JOYSTICK_9             8
#define ER_JOYSTICK_10            9
#define ER_JOYSTICK_11            10
#define ER_JOYSTICK_12            11
#define ER_JOYSTICK_13            12
#define ER_JOYSTICK_14            13
#define ER_JOYSTICK_15            14
#define ER_JOYSTICK_16            15
#define ER_JOYSTICK_LAST          ER_JOYSTICK_16

#if defined(TARGET_OS_DESKTOP)
#   define ER_KEYSTATE_PRESS GLFW_PRESS
#   define ER_KEYSTATE_RELEASE GLFW_RELEASE
#   define ER_KEYSTATE_REPEAT GLFW_REPEAT
#else
#   define ER_KEYSTATE_PRESS 1
#   define ER_KEYSTATE_RELEASE 2
#   define ER_KEYSTATE_REPEAT 3
#endif

#define ER_ST_FLOAT GL_FLOAT
#define ER_ST_FLOAT_VEC2 GL_FLOAT_VEC2
#define ER_ST_FLOAT_VEC3 GL_FLOAT_VEC3
#define ER_ST_FLOAT_VEC4 GL_FLOAT_VEC4
#define ER_ST_INT GL_INT
#define ER_ST_INT_VEC2 GL_INT_VEC2
#define ER_ST_INT_VEC3 GL_INT_VEC3
#define ER_ST_INT_VEC4 GL_INT_VEC4
#define ER_ST_UNSIGNED_INT GL_UNSIGNED_INT
#define ER_ST_UNSIGNED_INT_VEC2 GL_UNSIGNED_INT_VEC2
#define ER_ST_UNSIGNED_INT_VEC3 GL_UNSIGNED_INT_VEC3
#define ER_ST_UNSIGNED_INT_VEC4 GL_UNSIGNED_INT_VEC4
#define ER_ST_BOOL GL_BOOL
#define ER_ST_BOOL_VEC2 GL_BOOL_VEC2
#define ER_ST_BOOL_VEC3 GL_BOOL_VEC3
#define ER_ST_BOOL_VEC4 GL_BOOL_VEC4
#define ER_ST_FLOAT_MAT2 GL_FLOAT_MAT2
#define ER_ST_FLOAT_MAT3 GL_FLOAT_MAT3
#define ER_ST_FLOAT_MAT4 GL_FLOAT_MAT4
#define ER_ST_FLOAT_MAT2x3 GL_FLOAT_MAT2x3
#define ER_ST_FLOAT_MAT2x4 GL_FLOAT_MAT2x4
#define ER_ST_FLOAT_MAT3x2 GL_FLOAT_MAT3x2
#define ER_ST_FLOAT_MAT3x4 GL_FLOAT_MAT3x4
#define ER_ST_FLOAT_MAT4x2 GL_FLOAT_MAT4x2
#define ER_ST_FLOAT_MAT4x3 GL_FLOAT_MAT4x3
#define ER_ST_SAMPLER_1D GL_SAMPLER_1D
#define ER_ST_SAMPLER_2D GL_SAMPLER_2D
#define ER_ST_SAMPLER_3D GL_SAMPLER_3D
#define ER_ST_SAMPLER_CUBE GL_SAMPLER_CUBE
#define ER_ST_SAMPLER_1D_SHADOW GL_SAMPLER_1D_SHADOW
#define ER_ST_SAMPLER_2D_SHADOW GL_SAMPLER_2D_SHADOW
#define ER_ST_SAMPLER_1D_ARRAY GL_SAMPLER_1D_ARRAY
#define ER_ST_SAMPLER_2D_ARRAY GL_SAMPLER_2D_ARRAY
#define ER_ST_SAMPLER_1D_ARRAY_SHADOW GL_SAMPLER_1D_ARRAY_SHADOW
#define ER_ST_SAMPLER_2D_ARRAY_SHADOW GL_SAMPLER_2D_ARRAY_SHADOW
#define ER_ST_SAMPLER_2D_MULTISAMPLE GL_SAMPLER_2D_MULTISAMPLE
#define ER_ST_SAMPLER_2D_MULTISAMPLE_ARRAY GL_SAMPLER_2D_MULTISAMPLE_ARRAY
#define ER_ST_SAMPLER_CUBE_SHADOW GL_SAMPLER_CUBE_SHADOW
#define ER_ST_SAMPLER_BUFFER GL_SAMPLER_BUFFER
#define ER_ST_SAMPLER_2D_RECT GL_SAMPLER_2D_RECT
#define ER_ST_SAMPLER_2D_RECT_SHADOW GL_SAMPLER_2D_RECT_SHADOW
#define ER_ST_INT_SAMPLER_1D GL_INT_SAMPLER_1D
#define ER_ST_INT_SAMPLER_2D GL_INT_SAMPLER_2D
#define ER_ST_INT_SAMPLER_3D GL_INT_SAMPLER_3D
#define ER_ST_INT_SAMPLER_CUBE GL_INT_SAMPLER_CUBE
#define ER_ST_INT_SAMPLER_1D_ARRAY GL_INT_SAMPLER_1D_ARRAY
#define ER_ST_INT_SAMPLER_2D_ARRAY GL_INT_SAMPLER_2D_ARRAY
#define ER_ST_INT_SAMPLER_2D_MULTISAMPLE GL_INT_SAMPLER_2D_MULTISAMPLE
#define ER_ST_INT_SAMPLER_2D_MULTISAMPLE_ARRAY GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY
#define ER_ST_INT_SAMPLER_BUFFER GL_INT_SAMPLER_BUFFER
#define ER_ST_INT_SAMPLER_2D_RECT GL_INT_SAMPLER_2D_RECT
#define ER_ST_UNSIGNED_INT_SAMPLER_1D GL_UNSIGNED_INT_SAMPLER_1D
#define ER_ST_UNSIGNED_INT_SAMPLER_2D GL_UNSIGNED_INT_SAMPLER_2D
#define ER_ST_UNSIGNED_INT_SAMPLER_3D GL_UNSIGNED_INT_SAMPLER_3D
#define ER_ST_UNSIGNED_INT_SAMPLER_CUBE GL_UNSIGNED_INT_SAMPLER_CUBE
#define ER_ST_UNSIGNED_INT_SAMPLER_1D_ARRAY GL_UNSIGNED_INT_SAMPLER_1D_ARRAY
#define ER_ST_UNSIGNED_INT_SAMPLER_2D_ARRAY GL_UNSIGNED_INT_SAMPLER_2D_ARRAY
#define ER_ST_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE
#define ER_ST_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY
#define ER_ST_UNSIGNED_INT_SAMPLER_BUFFER GL_UNSIGNED_INT_SAMPLER_BUFFER
#define ER_ST_UNSIGNED_INT_SAMPLER_2D_RECT GL_UNSIGNED_INT_SAMPLER_2D_RECT

#define PI 3.14159265359
#define TO_RADS(d) (d*(PI/180.0f))
#define TO_DEGS(r) (r*(180.0f/PI))

typedef enum {
    /*
     * Path to the binary being executed
     */
    ER_PATH_BINARY,

    /*
     * Path to the application's support directory
     * - On OS X, this is ~/Library/Application Support/{author}/{name}
     * - On Windows, this is ~/{APPDATA}/{author}/{name}
     * - On Linux, this is ~/.local/share/{author}/{name}
     */
    ER_PATH_SUPPORT,

    /*
     * Path to game assets; Most of the time this is relative to ER_PATH_BINARY
     */
    ER_PATH_ASSETS,

    /*
     * Path to textures; Relative to ER_PATH_ASSETS
     */
    ER_PATH_TEXTURES,

    /*
     * Path to shaders; Relative to ER_PATH_ASSETS
     */
    ER_PATH_SHADERS,

    /*
     * Path to fonts; Relative to ER_PATH_ASSETS
     */
    ER_PATH_FONTS,

    /*
     * Path to sound effects; Relative to ER_PATH_ASSETS
     */
    ER_PATH_SOUNDS,

    /*
     * Path to music files; Relative to ER_PATH_ASSETS
     */
    ER_PATH_MUSIC

} er_path_type;

typedef struct er_path_result {
    char *path;
    size_t len;
} er_path_result;

typedef struct er_io_keyinfo {
    int sym;
    int state;
    double x;
    double y;
} er_io_keyinfo;

typedef enum {
    ER_PROP_UNKNOWN = 0,
    ER_PROP_BOOLEAN,
    ER_PROP_NUMBER,
    ER_PROP_STRING,
    ER_PROP_BOOLEAN_ARRAY,
    ER_PROP_NUMBER_ARRAY,
    ER_PROP_STRING_ARRAY
} er_prop_type;

typedef struct er_app_attrs * er_app_attrs;

typedef struct er_context * er_context;

typedef struct er_context_attrs * er_context_attrs;

typedef struct er_property_def * er_property_def;

typedef struct er_property * er_property;

typedef struct er_entity * er_entity;

typedef struct er_scene * er_scene;

typedef struct er_subsystem_attrs * er_subsystem_attrs;

typedef struct er_subsystem * er_subsystem;

typedef struct er_vector er_vector;

typedef union er_matrix er_matrix;

typedef struct er_matrix_stack * er_matrix_stack;

typedef struct er_file * er_file;

#if defined(TARGET_OS_ANDROID)
ERAPI er_exec_android(er_context *ctx, er_scene *scene, struct android_app *state);
#define er_exec(ctxptr, scene, state) er_exec_android(ctxptr, scene, state)
#else
ERAPI er_exec_cli(er_context *ctx, er_scene *scene, int argc, char **argv);
#define er_exec(ctxptr, scene, argc, argv) er_exec_cli(ctxptr, scene, argc, argv)
#endif

ERAPI er_init(er_app_attrs *attrs);
ERAPI er_quit(void);

ERAPI er_stop(void);

ERAPI er_time(double *time);

ERAPI er_app_attrs_init(er_app_attrs *attrs);
ERAPI er_app_attrs_set_name(er_app_attrs *attrs, const char *name);
ERAPI er_app_attrs_set_author(er_app_attrs *attrs, const char *author);
ERAPI er_app_attrs_destroy(er_app_attrs *attrs);

ERAPI er_app_get_path(er_path_type path, er_path_result *result);
ERAPI er_app_cleanup_path_result(er_path_result *target);

ERAPI er_fs_fopen(er_path_type root, const char *path, const char *mode, er_file *fp);
ERAPI er_fs_fread(er_file *fp, char *buffer, size_t count, size_t *bytes_read);
ERAPI er_fs_fseek(er_file *fp, off_t offset, int whence);
ERAPI er_fs_flength(er_file *fp, size_t *len);
ERAPI er_fs_ftell(er_file *fp, long int *pos);
ERAPI er_fs_ffree(er_file *fp);

ERAPI er_ctx_attrs_init(er_context_attrs *attrs);
ERAPI er_ctx_attrs_set_window_title(er_context_attrs *attrs, const char *title);
ERAPI er_ctx_attrs_set_screen_width(er_context_attrs *attrs, unsigned width);
ERAPI er_ctx_attrs_set_screen_height(er_context_attrs *attrs, unsigned height);
ERAPI er_ctx_attrs_set_fullscreen(er_context_attrs *attrs, int fullscreen);
ERAPI er_ctx_attrs_set_decorated(er_context_attrs *attrs, int decorated);
ERAPI er_ctx_attrs_set_vsync(er_context_attrs *attrs, int vsync);
ERAPI er_ctx_attrs_destroy(er_context_attrs *attrs);
ERAPI er_ctx_open(er_context_attrs *attrs, er_context *ctx);
ERAPI er_ctx_close(er_context *ctx);

ERAPI er_entity_reserve(er_entity *entity);
ERAPI er_entity_release(er_entity *entity);

ERAPI er_entity_add_child(er_entity *entity, er_entity *child);
ERAPI er_entity_remove_from_parent(er_entity *entity);
ERAPI er_entity_remove_children(er_entity *entity, void (*dcon)(er_entity *child));
ERAPI er_entity_for_each_child(er_entity *entity, int recurse, void (*fn)(er_entity *child));
ERAPI er_entity_get_id(er_entity *entity, uint64_t *id);
ERAPI er_entity_get_parent(er_entity *entity, er_entity *parent);
ERAPI er_entity_export_json(er_entity *entity, int export_children, int export_properties, char **json);
ERAPI er_entity_import_json(er_entity *entity, const char *json);

ERAPI er_prop_get_type(const char *key, er_prop_type *type);

ERAPI er_prop_get_boolean(er_entity *entity, const char *key, int *out);
ERAPI er_prop_get_number(er_entity *entity, const char *key, double *out);
ERAPI er_prop_get_string(er_entity *entity, const char *key, char **out);
ERAPI er_prop_get_boolean_array(er_entity *entity, const char *key, int **out, size_t *count);
ERAPI er_prop_get_number_array(er_entity *entity, const char *key, double **out, size_t *count);
ERAPI er_prop_get_string_array(er_entity *entity, const char *key, char ***out, size_t *count);

ERAPI er_prop_set_boolean(er_entity *entity, const char *key, int in);
ERAPI er_prop_set_number(er_entity *entity, const char *key, double in);
ERAPI er_prop_set_string(er_entity *entity, const char *key, const char *in);
ERAPI er_prop_set_boolean_array(er_entity *entity, const char *key, int *in, size_t count);
ERAPI er_prop_set_number_array(er_entity *entity, const char *key, double *in, size_t count);
ERAPI er_prop_set_string_array(er_entity *entity, const char *key, char **in, size_t count);

ERAPI er_prop_remove(er_entity *entity, const char *key);
ERAPI er_prop_clear_all(er_entity *entity);

ERAPI er_scene_create(er_scene *scene);
ERAPI er_scene_destroy(er_scene *scene);

ERAPI er_scene_set_on_create_cb(er_scene *scene, void (*cb)(er_entity root));
ERAPI er_scene_set_on_destroy_cb(er_scene *scene, void (*cb)(er_entity root));

ERAPI er_set_current_scene(er_scene *scene);

ERAPI er_io_register_action(const char *action_name, int (*action_cb)(er_io_keyinfo keyinfo));
ERAPI er_io_unregister_action(const char *action_name);

ERAPI er_io_add_trigger(const char *action_name, int trigger);
ERAPI er_io_clear_action(const char *action_name);
ERAPI er_io_clear_trigger(int trigger);

ERAPI er_subsystem_attrs_init(er_subsystem_attrs *attrs);
ERAPI er_subsystem_attrs_set_name(er_subsystem_attrs *attrs, const char *name);
ERAPI er_subsystem_attrs_set_init_cb(er_subsystem_attrs *attrs, void (*cb)(void));
ERAPI er_subsystem_attrs_set_update_cb(er_subsystem_attrs *attrs, void (*cb)(er_entity root, double delta));
ERAPI er_subsystem_attrs_set_quit_cb(er_subsystem_attrs *attrs, void (*cb)(void));
ERAPI er_subsystem_attrs_destroy(er_subsystem_attrs *attrs);
ERAPI er_subsystem_register(er_subsystem_attrs *attrs, er_subsystem *subsystem);
ERAPI er_subsystem_unregister(er_subsystem *subsystem);

ERAPI er_vector_addv(er_vector *v1, er_vector *v2);
ERAPI er_vector_adds(er_vector *v, double s);
ERAPI er_vector_muls(er_vector *v, double s);
ERAPI er_vector_dot(er_vector *v1, er_vector *v2, double *out);
ERAPI er_vector_cross(er_vector *v1, er_vector *v2, er_vector *out);
ERAPI er_vector_mag(er_vector *v, double *out);
ERAPI er_vector_normalize(er_vector *v);

ERAPI er_matrix_addm(er_matrix *m1, er_matrix *m2);
ERAPI er_matrix_adds(er_matrix *m, double s);
ERAPI er_matrix_mulm(er_matrix *m1, er_matrix *m2, er_matrix *out);
ERAPI er_matrix_muls(er_matrix *m, double s);
ERAPI er_matrix_identity(er_matrix *m);
ERAPI er_matrix_determinant(er_matrix *m, double *out);
ERAPI er_matrix_inverse(er_matrix *m);
ERAPI er_matrix_translates(er_matrix *m, double tx, double ty, double tz);
ERAPI er_matrix_translatev(er_matrix *m, er_vector *v);
ERAPI er_matrix_rotate(er_matrix *m, double angle, double x, double y, double z);
ERAPI er_matrix_scale(er_matrix *m, double sx, double sy, double sz);
ERAPI er_matrix_ortho(er_matrix *m, double left, double right, double bottom, double top, double near, double far);
ERAPI er_matrix_orthoa(er_matrix *m, double aspect, double near, double far);
ERAPI er_matrix_perspective(er_matrix *m, double fovy, double aspect, double near, double far);
ERAPI er_matrix_lookat(er_vector *eye, er_vector *target, er_vector *up, er_matrix *out);
ERAPI er_matrix_lookatyp(er_vector *eye, double pitch, double yaw, er_matrix *out);
ERAPI er_matrix_print(er_matrix *m);
ERAPI er_mstack_init(er_matrix_stack *stack);
ERAPI er_mstack_destroy(er_matrix_stack *stack);
ERAPI er_mstack_pop(er_matrix_stack *stack);
ERAPI er_mstack_push(er_matrix_stack *stack);
ERAPI er_mstack_peek(er_matrix_stack *stack, er_matrix **top);

#endif /* __included_erbium_h */
