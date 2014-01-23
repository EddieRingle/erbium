#ifndef __included_er_renderer_h
#define __included_er_renderer_h

#include "../../internal.h"

typedef struct er_vbuffer * er_vbuffer;

typedef struct er_texture * er_texture;

typedef ERAPI (*er__renderer_init_f)(void);
typedef ERAPI (*er__renderer_quit_f)(void);

typedef ERAPI (*er__renderer_bind_buffer_f)(er_vbuffer *buffer);
typedef ERAPI (*er__renderer_make_buffer_f)(er_vbuffer *buffer);
typedef ERAPI (*er__renderer_fill_buffer_f)(er_vbuffer *buffer, size_t size, void *data);
typedef ERAPI (*er__renderer_free_buffer_f)(er_vbuffer *buffer);

typedef ERAPI (*er__renderer_bind_texture_f)(er_texture *texture);
typedef ERAPI (*er__renderer_load_texture_f)(const char *filename, er_texture *out);
typedef ERAPI (*er__renderer_free_texture_f)(er_texture *texture);
typedef ERAPI (*er__renderer_blit_texture_f)(er_texture *texture, double x, double y, double w, double h);

extern er__renderer_init_f er__renderer_init;
extern er__renderer_quit_f er__renderer_quit;

extern er__renderer_bind_texture_f er__renderer_bind_texture;
extern er__renderer_load_texture_f er__renderer_load_texture;
extern er__renderer_free_texture_f er__renderer_free_texture;
extern er__renderer_blit_texture_f er__renderer_blit_texture;

#endif /* __included_er_renderer_h */
