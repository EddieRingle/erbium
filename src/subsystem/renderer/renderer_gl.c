#include "renderer.h"

#if defined(TARGET_OS_DESKTOP)

struct er_texture {
    GLuint id;
    char *filename;
    unsigned width;
    unsigned height;
    int is_bound;
};

static struct {
    struct er_texture *last_used_texture;
} gl_renderer;

ERAPI er__renderer_init__gl(void)
{
    gl_renderer.last_used_texture = NULL;
    return ERR_OK;
}
er__renderer_init_f er__renderer_init = &er__renderer_init__gl;

ERAPI er__renderer_quit__gl(void)
{
    return ERR_OK;
}
er__renderer_quit_f er__renderer_quit = &er__renderer_quit__gl;

ERAPI er__renderer_bind_texture__gl(er_texture *texture)
{
    if (gl_renderer.last_used_texture != NULL) {
        gl_renderer.last_used_texture->is_bound = 0;
    }
    gl_renderer.last_used_texture = (texture != NULL) ? *texture : NULL;
    if (texture == NULL || *texture == NULL) {
        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        glBindTexture(GL_TEXTURE_2D, (*texture)->id);
        (*texture)->is_bound = 1;
    }
    return ERR_OK;
}
er__renderer_bind_texture_f er__renderer_bind_texture = &er__renderer_bind_texture__gl;

ERAPI er__renderer_load_texture__gl(const char *filename, er_texture *texture)
{
    return ERR_NOT_IMPLEMENTED;
}
er__renderer_load_texture_f er__renderer_load_texture = &er__renderer_load_texture__gl;

ERAPI er__renderer_free_texture__gl(er_texture *texture)
{
    return ERR_NOT_IMPLEMENTED;
}
er__renderer_free_texture_f er__renderer_free_texture = &er__renderer_free_texture__gl;

ERAPI er__renderer_blit_texture__gl(er_texture *texture, double x, double y, double w, double h)
{
    return ERR_NOT_IMPLEMENTED;
}
er__renderer_blit_texture_f er__renderer_blit_texture = &er__renderer_blit_texture__gl;

#endif /* defined(TARGET_OS_DESKTOP) */
