#include "internal.h"

struct er_file {
    FILE *fp;
};

ERAPI er_fs_fopen(er_path_type root, const char *path, const char *mode, er_file *fp)
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
    memset(full_path, 0, sizeof full_path);
    sprintf(full_path, "%s/%s", root_result.path, path);
    *fp = er__malloc(sizeof(struct er_file));
    if (*fp == NULL) {
        return ERR_MEMORY_ERROR;
    }
    (*fp)->fp = fopen(full_path, mode);
    if ((*fp)->fp == NULL) {
        return ERR_NOT_FOUND;
    }
    er_app_cleanup_path_result(&root_result);
    return ERR_OK;
}

ERAPI er_fs_fread(er_file *fp, char *buffer, size_t count, size_t *bytes_read)
{
    size_t read_bytes = 0;
    if (fp == NULL || *fp == NULL || buffer == NULL) {
        return ERR_INVALID_ARGS;
    }
    read_bytes = fread(buffer, sizeof(char), count, (*fp)->fp);
    if (bytes_read != NULL) {
        *bytes_read = read_bytes;
    }
    return ERR_OK;
}

ERAPI er_fs_fseek(er_file *fp, off_t offset, int whence)
{
    if (fp == NULL || *fp == NULL || (*fp)->fp == NULL) {
        return ERR_INVALID_ARGS;
    }
    if (fseeko((*fp)->fp, offset, whence)) {
        return ERR_UNKNOWN;
    }
    return ERR_OK;
}

ERAPI er_fs_flength(er_file *fp, size_t *len)
{
    if (fp == NULL || *fp == NULL || (*fp)->fp == NULL) {
        return ERR_INVALID_ARGS;
    }
    fseek((*fp)->fp, 0, SEEK_END);
    *len = (size_t) ftell((*fp)->fp);
    rewind((*fp)->fp);
    return ERR_OK;
}

ERAPI er_fs_ftell(er_file *fp, long int *pos)
{
    if (fp == NULL || *fp == NULL || (*fp)->fp == NULL) {
        return ERR_INVALID_ARGS;
    }
    *pos = ftell((*fp)->fp);
    if (*pos <= -1L) {
        return ERR_UNKNOWN;
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
