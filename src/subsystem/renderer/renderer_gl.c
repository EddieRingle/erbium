#include "renderer.h"

#if defined(TARGET_OS_DESKTOP)

struct er_shader_program {
    GLuint vshader_id;
    GLuint fshader_id;
    GLuint program_id;

    int is_bound;
};

struct er_vbuffer {
    GLuint vbo_id;
    GLuint ebo_id;
    unsigned size;

    int is_bound;
};

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
    struct er_shader_program *last_used_program;
    struct er_vbuffer *last_used_vbuffer;
    struct er_texture *last_used_texture;
} gl_renderer;

ERAPI er__renderer_init__gl(void)
{
    GLuint vao;
    gl_renderer.last_used_vbuffer = NULL;
    gl_renderer.last_used_texture = NULL;
    gl_renderer.last_used_program = NULL;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    return ERR_OK;
}
er__renderer_init_f er__renderer_init = &er__renderer_init__gl;

ERAPI er__renderer_quit__gl(void)
{
    return ERR_OK;
}
er__renderer_quit_f er__renderer_quit = &er__renderer_quit__gl;

#define SHADER_TYPE_VERTEX 1
#define SHADER_TYPE_FRAGMENT 2

static ERAPI read_shader(const char *shader_file, int type, char **out, size_t *len)
{
    ERR ret;
    er_file fp;
    char fullname[256];
    size_t slen = 0;
    if (type == SHADER_TYPE_VERTEX) {
        sprintf(fullname, "glsl/150/%s.v.glsl", shader_file);
    } else if (type == SHADER_TYPE_FRAGMENT) {
        sprintf(fullname, "glsl/150/%s.f.glsl", shader_file);
    }
    if ((ret = er_fs_fopen(ER_PATH_SHADERS, fullname, "rb", &fp)) != ERR_OK) {
        LOGE("Error opening shader '%s'\n", shader_file);
        return ret;
    }
    if ((ret = er_fs_flength(&fp, &slen)) != ERR_OK) {
        er_fs_ffree(&fp);
        return ret;
    }
    *out = er__malloc(sizeof(char) * slen + 1);
    if (*out == NULL) {
        er_fs_ffree(&fp);
        return ERR_MEMORY_ERROR;
    }
    if ((ret = er_fs_fread(&fp, *out, slen, len)) != ERR_OK) {
        LOGE("Error reading shader file '%s'\n", shader_file);
        er_fs_ffree(&fp);
        return ret;
    }
    (*out)[slen] = '\0';
    if ((ret = er_fs_ffree(&fp)) != ERR_OK) {
        return ret;
    }
    return ERR_OK;
}

void print_program_inputs(GLuint program)
{
    GLint numActiveAttribs = 0, numActiveUniforms = 0;
    GLint maxAttribNameLength = 0;
    GLchar *buffer;
    GLint arraySize;
    GLenum type;
    GLsizei actualLength;
    int i;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
    LOGI("Loaded program has %i active attributes and %i active uniforms\n", numActiveAttribs, numActiveUniforms);
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);
    buffer = er__malloc((size_t)maxAttribNameLength);
    for (i = 0; i < numActiveAttribs; ++i) {
        arraySize = 0;
        type = 0;
        actualLength = 0;
        glGetActiveAttrib(program, i, maxAttribNameLength, &actualLength, &arraySize, &type, buffer);
        LOGI("Attribute \"%s\":\n", buffer);
    }
    er__free(buffer);
    buffer = er__malloc(sizeof(char) * 256);
    for (i = 0; i < numActiveUniforms; ++i) {
        arraySize = 0;
        type = 0;
        actualLength = 0;
        glGetActiveUniform(program, i, sizeof(char) * 256, &actualLength, &arraySize, &type, buffer);
        LOGI("Uniform \"%s\":\n", buffer);
    }
    er__free(buffer);
}

ERAPI er__renderer_load_program__gl(const char *vshader_file, const char *fshader_file, er_shader_program *program)
{
    ERR ret;
    GLint status;
    char *shader_source;
    char gl_error_buffer[512];
    if (vshader_file == NULL || fshader_file == NULL || program == NULL) {
        return ERR_INVALID_ARGS;
    }
    *program = er__malloc(sizeof(struct er_shader_program));
    if (*program == NULL) {
        return ERR_MEMORY_ERROR;
    }
    if ((ret = read_shader(vshader_file, SHADER_TYPE_VERTEX, &shader_source, NULL)) != ERR_OK) {
        LOGE("Error reading vertex shader\n");
        return ret;
    }
    (*program)->vshader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource((*program)->vshader_id, 1, &shader_source, NULL);
    glCompileShader((*program)->vshader_id);
    glGetShaderiv((*program)->vshader_id, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog((*program)->vshader_id, sizeof gl_error_buffer, NULL, gl_error_buffer);
        LOGE("Error compiling vertex shader '%s':\n%s\n", vshader_file, gl_error_buffer);
        LOGE("\nShader source:\n%s\n", shader_source);
        return ERR_UNKNOWN;
    }
    er__free(shader_source);
    if ((ret = read_shader(fshader_file, SHADER_TYPE_FRAGMENT, &shader_source, NULL)) != ERR_OK) {
        LOGE("Error reading fragment shader\n");
        return ret;
    }
    (*program)->fshader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource((*program)->fshader_id, 1, &shader_source, NULL);
    glCompileShader((*program)->fshader_id);
    glGetShaderiv((*program)->fshader_id, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog((*program)->fshader_id, sizeof gl_error_buffer, NULL, gl_error_buffer);
        LOGE("Error compiling fragment shader '%s':\n%s\n", fshader_file, gl_error_buffer);
        LOGE("\nShader source:\n%s\n", shader_source);
        return ERR_UNKNOWN;
    }
    er__free(shader_source);
    (*program)->program_id = glCreateProgram();
    glAttachShader((*program)->program_id, (*program)->vshader_id);
    glAttachShader((*program)->program_id, (*program)->fshader_id);
    glLinkProgram((*program)->program_id);
    glGetProgramiv((*program)->program_id, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        glGetProgramInfoLog((*program)->program_id, sizeof gl_error_buffer, NULL, gl_error_buffer);
        LOGE("Error linking shader program (%s, %s):\n%s\n", vshader_file, fshader_file, gl_error_buffer);
        return ERR_UNKNOWN;
    }
    print_program_inputs((*program)->program_id);
    (*program)->is_bound = 0;
    return ERR_OK;
}
er__renderer_load_program_f er__renderer_load_program = &er__renderer_load_program__gl;

ERAPI er__renderer_bind_program__gl(er_shader_program *program)
{
    if (gl_renderer.last_used_program != NULL) {
        gl_renderer.last_used_program->is_bound = 0;
    }
    gl_renderer.last_used_program = (program != NULL) ? *program : NULL;
    if (program == NULL || *program == NULL) {
        glUseProgram(0);
    } else {
        glUseProgram((*program)->program_id);
        (*program)->is_bound = 1;
    }
    return ERR_OK;
}
er__renderer_bind_program_f er__renderer_bind_program = &er__renderer_bind_program__gl;

ERAPI er__renderer_free_program__gl(er_shader_program *program)
{
    if (program == NULL || *program == NULL) {
        return ERR_INVALID_ARGS;
    }
    if (gl_renderer.last_used_program == *program) {
        er__renderer_bind_program(NULL);
    }
    glDeleteShader((*program)->vshader_id);
    glDeleteShader((*program)->fshader_id);
    glDeleteProgram((*program)->program_id);
    er__free(*program);
    *program = NULL;
    return ERR_OK;
}
er__renderer_free_program_f er__renderer_free_program = &er__renderer_free_program__gl;

ERAPI er__renderer_bind_buffer__gl(er_vbuffer *buffer)
{
    if (gl_renderer.last_used_vbuffer != NULL) {
        gl_renderer.last_used_vbuffer->is_bound = 0;
    }
    gl_renderer.last_used_vbuffer = (buffer != NULL) ? *buffer : NULL;
    if (buffer == NULL || *buffer == NULL) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, (*buffer)->vbo_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*buffer)->ebo_id);
        (*buffer)->is_bound = 1;
    }
    return ERR_OK;
}
er__renderer_bind_buffer_f er__renderer_bind_buffer = &er__renderer_bind_buffer__gl;

ERAPI er__renderer_make_buffer__gl(er_vbuffer *buffer)
{
    if (buffer == NULL) {
        return ERR_INVALID_ARGS;
    }
    *buffer = er__malloc(sizeof(struct er_vbuffer));
    if (*buffer == NULL) {
        return ERR_MEMORY_ERROR;
    }
    glGenBuffers(1, &(*buffer)->vbo_id);
    glGenBuffers(1, &(*buffer)->ebo_id);
    (*buffer)->size = 0;
    (*buffer)->is_bound = 0;
    return ERR_OK;
}
er__renderer_make_buffer_f er__renderer_make_buffer = &er__renderer_make_buffer__gl;

ERAPI er__renderer_fill_buffer__gl(er_vbuffer *buffer, size_t data_size, void *data, size_t elements_size, void *elements)
{
    if (buffer == NULL || *buffer == NULL) {
        return ERR_INVALID_ARGS;
    }
    if (gl_renderer.last_used_vbuffer != *buffer) {
        er__renderer_bind_buffer(buffer);
    }
    (*buffer)->size = elements_size / sizeof(unsigned int);
    glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_size, elements, GL_STATIC_DRAW);
    return ERR_OK;
}
er__renderer_fill_buffer_f er__renderer_fill_buffer = &er__renderer_fill_buffer__gl;

ERAPI er__renderer_free_buffer__gl(er_vbuffer *buffer)
{
    if (buffer == NULL || *buffer == NULL) {
        return ERR_INVALID_ARGS;
    }
    if (gl_renderer.last_used_vbuffer == *buffer) {
        er__renderer_bind_buffer(NULL);
    }
    glDeleteBuffers(1, &(*buffer)->vbo_id);
    glDeleteBuffers(1, &(*buffer)->ebo_id);
    er__free(*buffer);
    *buffer = NULL;
    return ERR_OK;
}
er__renderer_free_buffer_f er__renderer_free_buffer = &er__renderer_free_buffer__gl;

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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

#endif /* defined(TARGET_OS_DESKTOP) */
