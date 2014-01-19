#include "renderer.h"

#if defined(TARGET_OS_DESKTOP)

struct er_texture {
    GLuint id;
    char *filename;
    unsigned width;
    unsigned height;
    int is_bound;
};

ERAPI er__renderer_init__gl(void)
{
    return ERR_OK;
}
er__renderer_init_f er__renderer_init = &er__renderer_init__gl;

ERAPI er__renderer_quit__gl(void)
{
    return ERR_OK;
}
er__renderer_quit_f er__renderer_quit = &er__renderer_quit__gl;

ERAPI gl__renderer_bind_texture(er_texture *texture)
{
    return ERR_NOT_IMPLEMENTED;
}

ERAPI gl__renderer_load_texture(const char *filename, er_texture *texture)
{
    return ERR_NOT_IMPLEMENTED;
}

ERAPI gl__renderer_free_texture(er_texture *texture)
{
    return ERR_NOT_IMPLEMENTED;
}

ERAPI gl__renderer_blit_texture(er_texture *texture, double x, double y, double w, double h)
{
    return ERR_NOT_IMPLEMENTED;
}

er__renderer_bind_texture_f er__renderer_bind_texture = &gl__renderer_bind_texture;
er__renderer_load_texture_f er__renderer_load_texture = &gl__renderer_load_texture;
er__renderer_free_texture_f er__renderer_free_texture = &gl__renderer_free_texture;
er__renderer_blit_texture_f er__renderer_blit_texture = &gl__renderer_blit_texture;

#endif /* defined(TARGET_OS_DESKTOP) */
