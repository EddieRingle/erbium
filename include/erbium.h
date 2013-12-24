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
    ERR_INVALID_ARGS
} ERR;

#define ERAPI ERR

typedef struct er_app_attrs_t {
    char *name;
    char *author;
} er_app_attrs_t;

typedef enum {
    ER_PATH_BINARY,
    ER_PATH_SUPPORT
} er_path_e;

typedef struct er_path_result_t {
    char *path;
    size_t len;
} er_path_result_t;

#if defined(TARGET_OS_ANDROID)
ERAPI er_exec_android(er_app_attrs_t *attrs, struct android_app *state);
#define er_exec(attrsptr) er_exec_android(attrsptr, state)
#else
ERAPI er_exec_cli(er_app_attrs_t *attrs, int argc, char **argv);
#define er_exec(attrsptr) er_exec_cli(attrsptr, argc, argv)
#endif

ERAPI er_init(void);
ERAPI er_quit(void);

ERAPI er_app_get_path(er_path_e path, er_path_result_t *result);
ERAPI er_app_cleanup_path_result(er_path_result_t *target);

#endif /* __included_erbium_h */
