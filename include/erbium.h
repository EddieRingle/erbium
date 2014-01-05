#ifndef __included_erbium_h
#define __included_erbium_h

#include "platform_detect.h"

#if defined(TARGET_OS_ANDROID)
#   define TARGET_OS_MOBILE
#else
#   define TARGET_OS_DESKTOP
#endif

#include <stdlib.h>
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
#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>

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

#if defined(TARGET_COMPILER_VC)
#   define ERCALL __cdecl
#   pragma section(".CRT$XCU",read)
#   define ER_INITIALIZER(f) \
    static void __cdecl f(void); \
    __declspec(allocate(".CRT$XCU")) void (__cdecl*f##_)(void) = f; \
    static void __cdecl f(void)
#elif defined(__GNUC__)
#   define ERCALL
#   define ER_INITIALIZER(f) \
    static void f(void) __attribute__((constructor)); \
    static void f(void)
#else
#   error "Compiler does not support __attribute__((constructor))"
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

typedef enum {
    ER_PATH_BINARY,
    ER_PATH_SUPPORT
} er_path_e;

typedef struct er_path_result_t {
    char *path;
    size_t len;
} er_path_result_t;

typedef struct er_app_attrs_t * er_app_attrs_t;

typedef struct er_context_t * er_context_t;

typedef struct er_context_attrs_t * er_context_attrs_t;

#if defined(TARGET_OS_ANDROID)
ERAPI er_exec_android(er_app_attrs_t *attrs, er_context_t *ctx, struct android_app *state);
#define er_exec(attrsptr, ctxptr, state) er_exec_android(attrsptr, ctxptr, state)
#else
ERAPI er_exec_cli(er_app_attrs_t *attrs, er_context_t *ctx, int argc, char **argv);
#define er_exec(attrsptr, ctxptr, argc, argv) er_exec_cli(attrsptr, ctxptr, argc, argv)
#endif

ERAPI er_init(void);
ERAPI er_quit(void);

ERAPI er_app_attrs_init(er_app_attrs_t *attrs);
ERAPI er_app_attrs_set_name(er_app_attrs_t *attrs, const char *name);
ERAPI er_app_attrs_set_author(er_app_attrs_t *attrs, const char *author);
ERAPI er_app_attrs_destroy(er_app_attrs_t *attrs);

ERAPI er_app_get_path(er_path_e path, er_path_result_t *result);
ERAPI er_app_cleanup_path_result(er_path_result_t *target);

ERAPI er_ctx_attrs_init(er_context_attrs_t *attrs);
ERAPI er_ctx_attrs_set_window_title(er_context_attrs_t *attrs, const char *title);
ERAPI er_ctx_attrs_set_screen_width(er_context_attrs_t *attrs, unsigned width);
ERAPI er_ctx_attrs_set_screen_height(er_context_attrs_t *attrs, unsigned height);
ERAPI er_ctx_attrs_set_fullscreen(er_context_attrs_t *attrs, int fullscreen);
ERAPI er_ctx_attrs_set_decorated(er_context_attrs_t *attrs, int decorated);
ERAPI er_ctx_attrs_destroy(er_context_attrs_t *attrs);
ERAPI er_ctx_open(er_context_attrs_t *attrs, er_context_t *ctx);
ERAPI er_ctx_close(er_context_t *ctx);

#endif /* __included_erbium_h */
