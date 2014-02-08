#include "internal.h"

struct er_file {
    FILE *fp;
};

ERAPI er_fs_fopen(er_path_type root, const char *path, er_file *fp)
{
    ERR ret;
    char full_path[2048];
    er_path_result root_result = {0};
    if (path == NULL || fp == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((ret = er_app_get_path(root, &root_result)) != ERR_OK) {
        return ret;
    }
    sprintf(full_path, "%s/%s", root_result.path, path);
    *fp = er__malloc(sizeof(struct er_file));
    if (*fp == NULL) {
        return ERR_MEMORY_ERROR;
    }
    (*fp)->fp = fopen(full_path, "rb");
    if ((*fp)->fp == NULL) {
        return ERR_NOT_FOUND;
    }
    er_app_cleanup_path_result(&root_result);
    return ERR_OK;
}

ERAPI er_fs_fread(er_file *fp, char **out, size_t *len)
{
    size_t flen;
    if (fp == NULL || *fp == NULL) {
        return ERR_INVALID_ARGS;
    }
    fseek((*fp)->fp, 0, SEEK_END);
    flen = (size_t)ftell((*fp)->fp);
    rewind((*fp)->fp);
    if (len != NULL) {
        *len = flen;
    }
    if (out != NULL) {
        *out = er__malloc(flen + 1);
        fread(*out, flen, 1, (*fp)->fp);
        (*out)[flen] = '\0';
    }
    return ERR_OK;
}

ERAPI er_fs_ffree(er_file *fp)
{
    if (fclose((*fp)->fp)) {
        return ERR_UNKNOWN;
    }
    er__free(*fp);
    *fp = NULL;
    return ERR_OK;
}
