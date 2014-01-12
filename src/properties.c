#include "internal.h"

struct er_property_def *definitions = NULL;

ERAPI er_prop_get_type(const char *key, er_prop_type *type)
{
    struct er_property_def *prop_def = NULL;
    if (key == NULL || type == NULL) {
        return ERR_INVALID_ARGS;
    }
    HASH_FIND_STR(definitions, key, prop_def);
    if (prop_def == NULL) {
        return ERR_NOT_FOUND;
    }
    *type = prop_def->type;
    return ERR_OK;
}

static ERAPI get_prop_def(const char *key, struct er_property_def **out)
{
    struct er_property_def *prop_def = NULL;
    HASH_FIND_STR(definitions, key, prop_def);
    if (prop_def == NULL) {
        return ERR_NOT_FOUND;
    }
    if (out != NULL) {
        *out = prop_def;
    }
    return ERR_OK;
}

static ERAPI get_prop(er_entity *entity, const char *key, struct er_property **out)
{
    ERR ret;
    struct er_property *prop = NULL;
    struct er_property_def *def = NULL;
    if ((ret = get_prop_def(key, &def)) != ERR_OK) {
        return ret;
    }
    HASH_FIND_INT(def->instances, &(*entity)->id, prop);
    if (prop == NULL) {
        return ERR_NOT_FOUND;
    }
    if (out != NULL) {
        *out = prop;
    }
    return ERR_OK;
}

ERAPI er_prop_get_boolean(er_entity *entity, const char *key, int *out)
{
    ERR ret;
    struct er_property *prop = NULL;
    if ((ret = get_prop(entity, key, &prop)) != ERR_OK) {
        return ret;
    }
    if (prop->definition->type != ER_PROP_BOOLEAN) {
        return ERR_PROP_TYPE_MISMATCH;
    }
    if (out != NULL) {
        *out = prop->_bool;
    }
    return ERR_OK;
}

ERAPI er_prop_get_number(er_entity *entity, const char *key, double *out)
{
    ERR ret;
    struct er_property *prop = NULL;
    if ((ret = get_prop(entity, key, &prop)) != ERR_OK) {
        return ret;
    }
    if (prop->definition->type != ER_PROP_NUMBER) {
        return ERR_PROP_TYPE_MISMATCH;
    }
    if (out != NULL) {
        *out = prop->_number;
    }
    return ERR_OK;
}

ERAPI er_prop_get_string(er_entity *entity, const char *key, char **out)
{
    ERR ret;
    struct er_property *prop = NULL;
    if ((ret = get_prop(entity, key, &prop)) != ERR_OK) {
        return ret;
    }
    if (prop->definition->type != ER_PROP_STRING) {
        return ERR_PROP_TYPE_MISMATCH;
    }
    if (out != NULL) {
        *out = er__strdup(prop->_string);
        if (*out == NULL) {
            return ERR_MEMORY_ERROR;
        }
    }
    return ERR_OK;
}

ERAPI er_prop_get_boolean_array(er_entity *entity, const char *key, int **out, size_t *count)
{
    ERR ret;
    struct er_property *prop = NULL;
    if ((ret = get_prop(entity, key, &prop)) != ERR_OK) {
        return ret;
    }
    if (prop->definition->type != ER_PROP_BOOLEAN_ARRAY || prop->array_count < 1) {
        return ERR_PROP_TYPE_MISMATCH;
    }
    if (out != NULL) {
        *out = er__malloc(sizeof(int) * prop->array_count);
        memcpy(*out, prop->_unknown, sizeof(int) * prop->array_count);
    }
    if (count != NULL) {
        *count = prop->array_count;
    }
    return ERR_OK;
}

ERAPI er_prop_get_number_array(er_entity *entity, const char *key, double **out, size_t *count)
{
    ERR ret;
    struct er_property *prop = NULL;
    if ((ret = get_prop(entity, key, &prop)) != ERR_OK) {
        return ret;
    }
    if (prop->definition->type != ER_PROP_NUMBER_ARRAY || prop->array_count < 1) {
        return ERR_PROP_TYPE_MISMATCH;
    }
    if (out != NULL) {
        *out = er__malloc(sizeof(double) * prop->array_count);
        memcpy(*out, prop->_unknown, sizeof(double) * prop->array_count);
    }
    if (count != NULL) {
        *count = prop->array_count;
    }
    return ERR_OK;
}

ERAPI er_prop_get_string_array(er_entity *entity, const char *key, char ***out, size_t *count)
{
    ERR ret;
    struct er_property *prop = NULL;
    int i;
    if ((ret = get_prop(entity, key, &prop)) != ERR_OK) {
        return ret;
    }
    if (prop->definition->type != ER_PROP_STRING_ARRAY || prop->array_count < 1) {
        return ERR_PROP_TYPE_MISMATCH;
    }
    if (out != NULL) {
        *out = er__malloc(sizeof(char *) * prop->array_count);
        for (i = 0; i < prop->array_count; i++) {
            (*out)[i] = er__strdup(((char**)prop->_unknown)[i]);
        }
    }
    if (count != NULL) {
        *count = prop->array_count;
    }
    return ERR_OK;
}
