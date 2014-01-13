#include "internal.h"

struct er_property_def *g_property_definitions = NULL;

ERAPI er_prop_get_type(const char *key, er_prop_type *type)
{
    struct er_property_def *prop_def = NULL;
    if (key == NULL || type == NULL) {
        return ERR_INVALID_ARGS;
    }
    HASH_FIND_STR(g_property_definitions, key, prop_def);
    if (prop_def == NULL) {
        return ERR_NOT_FOUND;
    }
    *type = prop_def->type;
    return ERR_OK;
}

static ERAPI get_prop_def(const char *key, struct er_property_def **out)
{
    struct er_property_def *prop_def = NULL;
    HASH_FIND_STR(g_property_definitions, key, prop_def);
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
    if (entity == NULL || *entity == NULL || key == NULL) {
        return ERR_INVALID_ARGS;
    }
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
    if (entity == NULL || *entity == NULL || key == NULL) {
        return ERR_INVALID_ARGS;
    }
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
    if (entity == NULL || *entity == NULL || key == NULL) {
        return ERR_INVALID_ARGS;
    }
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
    if (entity == NULL || *entity == NULL || key == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((ret = get_prop(entity, key, &prop)) != ERR_OK) {
        return ret;
    }
    if (prop->definition->type != ER_PROP_BOOLEAN_ARRAY || prop->array_count < 1) {
        return ERR_PROP_TYPE_MISMATCH;
    }
    if (out != NULL) {
        *out = er__memdup(prop->_unknown, sizeof(int) * prop->array_count);
        if (*out == NULL) {
            return ERR_MEMORY_ERROR;
        }
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
    if (entity == NULL || *entity == NULL || key == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((ret = get_prop(entity, key, &prop)) != ERR_OK) {
        return ret;
    }
    if (prop->definition->type != ER_PROP_NUMBER_ARRAY || prop->array_count < 1) {
        return ERR_PROP_TYPE_MISMATCH;
    }
    if (out != NULL) {
        *out = er__memdup(prop->_unknown, sizeof(double) * prop->array_count);
        if (*out == NULL) {
            return ERR_MEMORY_ERROR;
        }
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
    if (entity == NULL || *entity == NULL || key == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((ret = get_prop(entity, key, &prop)) != ERR_OK) {
        return ret;
    }
    if (prop->definition->type != ER_PROP_STRING_ARRAY || prop->array_count < 1) {
        return ERR_PROP_TYPE_MISMATCH;
    }
    if (out != NULL) {
        *out = er__malloc(sizeof(char *) * prop->array_count);
        if (*out == NULL) {
            return ERR_MEMORY_ERROR;
        }
        for (i = 0; i < prop->array_count; i++) {
            (*out)[i] = er__strdup(((char**)prop->_unknown)[i]);
            if ((*out)[i] == NULL) {
                for (i = i - 1; i >= 0; i--) {
                    er__free((*out)[i]);
                }
                er__free(*out);
                return ERR_MEMORY_ERROR;
            }
        }
    }
    if (count != NULL) {
        *count = prop->array_count;
    }
    return ERR_OK;
}

static ERAPI get_or_create_prop_def(const char *key, er_prop_type type, struct er_property_def **out)
{
    ERR ret;
    struct er_property_def *def = NULL;
    if ((ret = get_prop_def(key, &def)) != ERR_OK) {
        if (ret == ERR_NOT_FOUND) {
            def = er__malloc(sizeof(struct er_property_def));
            if (def == NULL) {
                return ERR_MEMORY_ERROR;
            }
            strncpy(def->name, key, sizeof def->name);
            def->type = type;
            def->instances = NULL;
            HASH_ADD_STR(g_property_definitions, name, def);
            if (out != NULL) {
                *out = def;
            }
            return ERR_OK;
        } else {
            return ret;
        }
    } else if (def != NULL) {
        if (def->type != type) {
            return ERR_PROP_TYPE_MISMATCH;
        }
        if (out != NULL) {
            *out = def;
        }
        return ERR_OK;
    }
    return ERR_UNKNOWN;
}

ERAPI er_prop_set_boolean(er_entity *entity, const char *key, int in)
{
    ERR ret;
    struct er_property_def *def = NULL;
    struct er_property *prop = NULL;
    if (entity == NULL || *entity == NULL || key == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((ret = get_prop(entity, key, &prop)) == ERR_OK) {
        if (prop->definition->type != ER_PROP_BOOLEAN) {
            return ERR_PROP_TYPE_MISMATCH;
        }
        prop->_bool = in;
        return ERR_OK;
    }
    if (ret != ERR_NOT_FOUND) {
        return ret;
    }
    if ((ret = get_or_create_prop_def(key, ER_PROP_BOOLEAN, &def)) != ERR_OK) {
        return ret;
    }
    prop = er__malloc(sizeof(struct er_property));
    if (prop == NULL) {
        return ERR_MEMORY_ERROR;
    }
    prop->key = (*entity)->id;
    prop->definition = def;
    prop->array_count = 0;
    prop->_bool = in;
    HASH_ADD_INT(def->instances, key, prop);
    return ERR_OK;
}

ERAPI er_prop_set_number(er_entity *entity, const char *key, double in)
{
    ERR ret;
    struct er_property_def *def = NULL;
    struct er_property *prop = NULL;
    if (entity == NULL || *entity == NULL || key == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((ret = get_prop(entity, key, &prop)) == ERR_OK) {
        if (prop->definition->type != ER_PROP_NUMBER) {
            return ERR_PROP_TYPE_MISMATCH;
        }
        prop->_number = in;
        return ERR_OK;
    }
    if (ret != ERR_NOT_FOUND) {
        return ret;
    }
    if ((ret = get_or_create_prop_def(key, ER_PROP_NUMBER, &def)) != ERR_OK) {
        return ret;
    }
    prop = er__malloc(sizeof(struct er_property));
    if (prop == NULL) {
        return ERR_MEMORY_ERROR;
    }
    prop->key = (*entity)->id;
    prop->definition = def;
    prop->array_count = 0;
    prop->_number = in;
    HASH_ADD_INT(def->instances, key, prop);
    return ERR_OK;
}

ERAPI er_prop_set_string(er_entity *entity, const char *key, const char *in)
{
    ERR ret;
    struct er_property_def *def = NULL;
    struct er_property *prop = NULL;
    if (entity == NULL || *entity == NULL || key == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((ret = get_prop(entity, key, &prop)) == ERR_OK) {
        if (prop->definition->type != ER_PROP_STRING) {
            return ERR_PROP_TYPE_MISMATCH;
        }
        if (prop->_string != NULL) {
            er__free(prop->_string);
        }
        prop->_string = er__strdup(in);
        if (prop->_string == NULL) {
            return ERR_MEMORY_ERROR;
        }
        return ERR_OK;
    }
    if (ret != ERR_NOT_FOUND) {
        return ret;
    }
    if ((ret = get_or_create_prop_def(key, ER_PROP_STRING, &def)) != ERR_OK) {
        return ret;
    }
    prop = er__malloc(sizeof(struct er_property));
    if (prop == NULL) {
        return ERR_MEMORY_ERROR;
    }
    prop->key = (*entity)->id;
    prop->definition = def;
    prop->array_count = 0;
    prop->_string = er__strdup(in);
    if (prop->_string == NULL) {
        er__free(prop);
        return ERR_MEMORY_ERROR;
    }
    HASH_ADD_INT(def->instances, key, prop);
    return ERR_OK;
}

ERAPI er_prop_set_boolean_array(er_entity *entity, const char *key, int *in, size_t count)
{
    ERR ret;
    struct er_property_def *def = NULL;
    struct er_property *prop = NULL;
    if (entity == NULL || *entity == NULL || key == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((ret = get_prop(entity, key, &prop)) == ERR_OK) {
        if (prop->definition->type != ER_PROP_BOOLEAN_ARRAY) {
            return ERR_PROP_TYPE_MISMATCH;
        }
        if (prop->_unknown != NULL) {
            er__free(prop->_unknown);
        }
        prop->_unknown = er__memdup(in, sizeof(int) * count);
        if (prop->_unknown == NULL) {
            return ERR_MEMORY_ERROR;
        }
        prop->array_count = count;
        return ERR_OK;
    }
    if (ret != ERR_NOT_FOUND) {
        return ret;
    }
    if ((ret = get_or_create_prop_def(key, ER_PROP_BOOLEAN_ARRAY, &def)) != ERR_OK) {
        return ret;
    }
    prop = er__malloc(sizeof(struct er_property));
    if (prop == NULL) {
        return ERR_MEMORY_ERROR;
    }
    prop->key = (*entity)->id;
    prop->definition = def;
    prop->array_count = count;
    prop->_unknown = er__memdup(in, sizeof(int) * count);
    if (prop->_unknown == NULL) {
        er__free(prop);
        return ERR_MEMORY_ERROR;
    }
    HASH_ADD_INT(def->instances, key, prop);
    return ERR_OK;
}

ERAPI er_prop_set_number_array(er_entity *entity, const char *key, double *in, size_t count)
{
    ERR ret;
    struct er_property_def *def = NULL;
    struct er_property *prop = NULL;
    if (entity == NULL || *entity == NULL || key == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((ret = get_prop(entity, key, &prop)) == ERR_OK) {
        if (prop->definition->type != ER_PROP_NUMBER_ARRAY) {
            return ERR_PROP_TYPE_MISMATCH;
        }
        if (prop->_unknown != NULL) {
            er__free(prop->_unknown);
        }
        prop->_unknown = er__memdup(in, sizeof(double) * count);
        if (prop->_unknown == NULL) {
            return ERR_MEMORY_ERROR;
        }
        prop->array_count = count;
        return ERR_OK;
    }
    if (ret != ERR_NOT_FOUND) {
        return ret;
    }
    if ((ret = get_or_create_prop_def(key, ER_PROP_NUMBER_ARRAY, &def)) != ERR_OK) {
        return ret;
    }
    prop = er__malloc(sizeof(struct er_property));
    if (prop == NULL) {
        return ERR_MEMORY_ERROR;
    }
    prop->key = (*entity)->id;
    prop->definition = def;
    prop->array_count = count;
    prop->_unknown = er__memdup(in, sizeof(double) * count);
    if (prop->_unknown == NULL) {
        er__free(prop);
        return ERR_MEMORY_ERROR;
    }
    HASH_ADD_INT(def->instances, key, prop);
    return ERR_OK;
}

ERAPI er_prop_set_string_array(er_entity *entity, const char *key, char **in, size_t count)
{
    ERR ret;
    struct er_property_def *def = NULL;
    struct er_property *prop = NULL;
    int i;
    if (entity == NULL || *entity == NULL || key == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((ret = get_prop(entity, key, &prop)) == ERR_OK) {
        if (prop->definition->type != ER_PROP_STRING_ARRAY) {
            return ERR_PROP_TYPE_MISMATCH;
        }
        if (prop->_unknown != NULL) {
            for (i = 0; i < prop->array_count; i++) {
                er__free(((char**)prop->_unknown)[i]);
            }
            er__free(prop->_unknown);
        }
        prop->_unknown = er__malloc(sizeof(char *) * count);
        if (prop->_unknown == NULL) {
            return ERR_MEMORY_ERROR;
        }
        for (i = 0; i < count; i++) {
            ((char **)prop->_unknown)[i] = er__strdup(in[i]);
            if (((char **)prop->_unknown)[i] == NULL) {
                for (i = i - 1; i >= 0; i--) {
                    er__free(((char **)prop->_unknown)[i]);
                }
                er__free(prop->_unknown);
                return ERR_MEMORY_ERROR;
            }
        }
        prop->array_count = count;
        return ERR_OK;
    }
    if (ret != ERR_NOT_FOUND) {
        return ret;
    }
    if ((ret = get_or_create_prop_def(key, ER_PROP_STRING_ARRAY, &def)) != ERR_OK) {
        return ret;
    }
    prop = er__malloc(sizeof(struct er_property));
    if (prop == NULL) {
        return ERR_MEMORY_ERROR;
    }
    prop->key = (*entity)->id;
    prop->definition = def;
    prop->array_count = count;
    prop->_unknown = er__malloc(sizeof(char *) * count);
    if (prop->_unknown == NULL) {
        er__free(prop);
        return ERR_MEMORY_ERROR;
    }
    for (i = 0; i < count; i++) {
        ((char **)prop->_unknown)[i] = er__strdup(in[i]);
        if (((char **)prop->_unknown)[i] == NULL) {
            for (i = i - 1; i >= 0; i--) {
                er__free(((char **)prop->_unknown)[i]);
            }
            er__free(prop->_unknown);
            er__free(prop);
            return ERR_MEMORY_ERROR;
        }
    }
    HASH_ADD_INT(def->instances, key, prop);
    return ERR_OK;
}

ERAPI er_prop_remove(er_entity *entity, const char *key)
{
    ERR ret;
    struct er_property *prop = NULL;
    int i;
    if (entity == NULL || *entity == NULL || key == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((ret = get_prop(entity, key, &prop)) != ERR_OK) {
        return ret;
    }
    HASH_DEL(prop->definition->instances, prop);
    switch (prop->definition->type) {
        case ER_PROP_STRING_ARRAY:
            if (prop->_unknown != NULL) {
                for (i = 0; i < prop->array_count; i++) {
                    if (((char **)prop->_unknown)[i] != NULL) {
                        er__free(((char **)prop->_unknown)[i]);
                    }
                }
                er__free(prop->_unknown);
            }
            er__free(prop);
            break;
        case ER_PROP_NUMBER_ARRAY:
        case ER_PROP_BOOLEAN_ARRAY:
            if (prop->_unknown != NULL) {
                er__free(prop->_unknown);
            }
            er__free(prop);
            break;
        case ER_PROP_STRING:
            if (prop->_string != NULL) {
                er__free(prop->_string);
            }
        case ER_PROP_NUMBER:
        case ER_PROP_BOOLEAN:
        default:
            er__free(prop);
            break;
    }
    return ERR_OK;
}
