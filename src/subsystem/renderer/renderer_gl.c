#include "renderer.h"

#if defined(TARGET_OS_DESKTOP)

struct er_texture {
    GLuint id;
    char *filename;
    unsigned width;
    unsigned height;

    int bit_depth;
    int color_type;

    int blur;
    int clamp;

    int is_bound;
};

static struct {
    struct er_texture *last_used_texture;
} gl_renderer;

ERAPI er__renderer_init__gl(void)
{
    gl_renderer.last_used_texture = NULL;
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

static void read_meta_info(const char *filename, er_texture *texture)
{
    char metafilename[strlen(filename) + 8];
    char *json;
    size_t json_len;
    FILE *fp;
    JsonNode *node, *texture_node, *child;
    sprintf(metafilename, "%s.ermeta", filename);
    fp = fopen(metafilename, "rb");
    if (fp == NULL) {
        return;
    }
    fseek(fp, 0, SEEK_END);
    json_len = (size_t)ftell(fp);
    rewind(fp);
    json = er__malloc(json_len + 1);
    fread(json, json_len, 1, fp);
    fclose(fp);
    node = json_decode(json);
    texture_node = json_find_member(node, "texture");
    (*texture)->blur = 0;
    (*texture)->clamp = 0;
    if (texture_node != NULL) {
        child = json_find_member(texture_node, "blur");
        if (child != NULL && child->tag == JSON_BOOL) {
            (*texture)->blur = child->bool_;
        }
        child = json_find_member(texture_node, "clamp");
        if (child != NULL && child->tag == JSON_BOOL) {
            (*texture)->clamp = child->bool_;
        }
    }
    json_delete(node);
    er__free(json);
}

ERAPI er__renderer_load_texture__gl(const char *filename, er_texture *texture)
{
    png_byte header[8];
    FILE *fp;
    int is_png;
    png_structp png_ptr;
    png_infop info_ptr;
    png_infop end_info;
    png_uint_32 twidth, theight;
    int rowbytes;
    png_byte *image_data;
    png_bytep *row_ptrs;
    int i;

    if (filename == NULL || texture == NULL) {
        return ERR_INVALID_ARGS;
    }

    *texture = er__malloc(sizeof(struct er_texture));
    if (*texture == NULL) {
        return ERR_MEMORY_ERROR;
    }
    memset(*texture, 0, sizeof(struct er_texture));

    read_meta_info(filename, texture);

    (*texture)->filename = er__strdup(filename);
    if ((*texture)->filename == NULL) {
        er__free(*texture);
        *texture = NULL;
        return ERR_MEMORY_ERROR;
    }

    fp = fopen(filename, "rb");
    if (!fp) {
        LOGE("Error opening file handle for '%s'\n", filename);
        return ERR_UNKNOWN;
    }

    fread(&header, 8, sizeof(png_byte), fp);
    rewind(fp);

    is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png) {
        fclose(fp);
        er__free((*texture)->filename);
        er__free(*texture);
        *texture = NULL;
        LOGE("Error: '%s' is not a valid PNG file\n", filename);
        return ERR_UNKNOWN;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        er__free((*texture)->filename);
        er__free(*texture);
        *texture = NULL;
        return ERR_UNKNOWN;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        er__free((*texture)->filename);
        er__free(*texture);
        *texture = NULL;
        return ERR_UNKNOWN;
    }

    end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        er__free((*texture)->filename);
        er__free(*texture);
        *texture = NULL;
        return ERR_UNKNOWN;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        er__free((*texture)->filename);
        er__free(*texture);
        *texture = NULL;
        return ERR_UNKNOWN;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    (*texture)->bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    (*texture)->color_type = png_get_color_type(png_ptr, info_ptr);
    if ((*texture)->color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }
    if ((*texture)->color_type == PNG_COLOR_TYPE_GRAY && (*texture)->bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    }
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
    }
    if ((*texture)->bit_depth == 16) {
        png_set_strip_16(png_ptr);
    } else if ((*texture)->bit_depth < 8) {
        png_set_packing(png_ptr);
    }
    png_read_update_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &(*texture)->bit_depth, &(*texture)->color_type, NULL, NULL, NULL);
    (*texture)->width = twidth;
    (*texture)->height = theight;
    rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    image_data = er__malloc(sizeof(png_byte) * rowbytes * theight);
    if (image_data == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        er__free((*texture)->filename);
        er__free(*texture);
        *texture = NULL;
        return ERR_MEMORY_ERROR;
    }
    row_ptrs = er__malloc(sizeof(png_bytep) * theight);
    if (row_ptrs == NULL) {
        er__free(image_data);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        er__free((*texture)->filename);
        er__free(*texture);
        *texture = NULL;
        return ERR_MEMORY_ERROR;
    }
    for (i = 0; (uint32_t)i < theight; ++i) {
        row_ptrs[theight - 1 - i] = image_data + i * rowbytes;
    }
    png_read_image(png_ptr, row_ptrs);
    png_read_end(png_ptr, NULL);

    glGenTextures(1, &(*texture)->id);
    if ((*texture)->id < 1) {
        LOGE("Texture limit reached");
        er__free(row_ptrs);
        er__free(image_data);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        er__free((*texture)->filename);
        er__free(*texture);
        *texture = NULL;
        return ERR_UNKNOWN;
    }
    er__renderer_bind_texture(texture);
    if ((*texture)->clamp) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    if ((*texture)->blur) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    if ((*texture)->color_type == PNG_COLOR_TYPE_RGBA) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, theight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }
    er__free(row_ptrs);
    er__free(image_data);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(fp);
    return ERR_OK;
}
er__renderer_load_texture_f er__renderer_load_texture = &er__renderer_load_texture__gl;

ERAPI er__renderer_free_texture__gl(er_texture *texture)
{
    if (texture == NULL || *texture == NULL) {
        return ERR_INVALID_ARGS;
    }
    if (gl_renderer.last_used_texture == *texture) {
        er__renderer_bind_texture(NULL);
    }
    glDeleteTextures(1, &(*texture)->id);
    if ((*texture)->filename != NULL) {
        er__free((*texture)->filename);
    }
    er__free(*texture);
    *texture = NULL;
    return ERR_OK;
}
er__renderer_free_texture_f er__renderer_free_texture = &er__renderer_free_texture__gl;

ERAPI er__renderer_blit_texture__gl(er_texture *texture, double x, double y, double w, double h)
{
    return ERR_NOT_IMPLEMENTED;
}
er__renderer_blit_texture_f er__renderer_blit_texture = &er__renderer_blit_texture__gl;

#endif /* defined(TARGET_OS_DESKTOP) */
