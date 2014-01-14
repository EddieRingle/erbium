#include "internal.h"

static uint64_t max_key_reserved = 0;
static const uint64_t max_key_possible = ~((uint64_t)0) - 1;

static struct er_entity *entities = NULL;

static uint64_t find_free_key(void)
{
    uint64_t key;
    struct er_entity *e = NULL;
    if (max_key_possible - 1 > max_key_reserved) {
        return max_key_reserved + 1;
    } else {
        for (key = 1; key != max_key_possible; key++) {
            HASH_FIND_INT(entities, &key, e);
            if (e == NULL) {
                return key;
            }
        }
    }
    return 0;
}

ERAPI er_entity_reserve(er_entity *entity)
{
    uint64_t key = find_free_key();
    if (key == 0) {
        return ERR_UNKNOWN;
    }
    if (entity == NULL) {
        return ERR_INVALID_ARGS;
    }
    *entity = er__malloc(sizeof(struct er_entity));
    if (*entity == NULL) {
        return ERR_MEMORY_ERROR;
    }
    (*entity)->id = key;
    (*entity)->num_children = 0;
    (*entity)->parent = NULL;
    list_head_init(&(*entity)->children);
    if (key > max_key_reserved) {
        max_key_reserved = key;
    }
    HASH_ADD_INT(entities, id, *entity);
    return ERR_OK;
}

ERAPI er_entity_release(er_entity *entity)
{
    ERR ret = ERR_OK;
    struct er_entity *e, *tmp = NULL;
    if (entity == NULL || *entity == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((*entity)->id == max_key_reserved) {
        max_key_reserved--;
    }
    er_prop_clear_all(entity);
    list_for_each_safe((&(*entity)->children), e, tmp, siblings) {
        list_del(&e->siblings);
        e->parent = NULL;
        (*entity)->num_children--;
        if ((ret = er_entity_release(&e)) != ERR_OK) {
            return ret;
        }
    }
    HASH_DEL(entities, *entity);
    er__free(*entity);
    *entity = NULL;
    return ERR_OK;
}

ERAPI er_entity_add_child(er_entity *entity, er_entity *child)
{
    if (entity == NULL || *entity == NULL || child == NULL || *child == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((*child)->parent != NULL) {
        return ERR_KIDNAPPER;
    }
    list_add_tail(&(*entity)->children, &(*child)->siblings);
    (*child)->parent = *entity;
    (*entity)->num_children++;
    return ERR_OK;
}

ERAPI er_entity_remove_from_parent(er_entity *entity)
{
    if (entity == NULL || *entity == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((*entity)->parent == NULL) {
        return ERR_NO_PARENT;
    }
    list_del(&(*entity)->siblings);
    (*entity)->parent->num_children--;
    (*entity)->parent = NULL;
    return ERR_OK;
}

ERAPI er_entity_remove_children(er_entity *entity, void (*dcon)(er_entity *child))
{
    struct er_entity *e, *tmp = NULL;
    if (entity == NULL || *entity == NULL) {
        return ERR_INVALID_ARGS;
    }
    list_for_each_safe((&(*entity)->children), e, tmp, siblings) {
        list_del(&e->siblings);
        e->parent = NULL;
        (*entity)->num_children--;
        if (dcon != NULL) {
            dcon(&e);
        }
    }
    return ERR_OK;
}

ERAPI er_entity_for_each_child(er_entity *entity, int recurse, void (*fn)(er_entity *child))
{
    struct er_entity *e, *tmp = NULL;
    if (entity == NULL || *entity == NULL) {
        return ERR_INVALID_ARGS;
    }
    if (fn != NULL) {
        list_for_each_safe(&(*entity)->children, e, tmp, siblings) {
            fn(&e);
            if (recurse) {
                er_entity_for_each_child(&e, recurse, fn);
            }
        }
    }
    return ERR_OK;
}

ERAPI er_entity_get_id(er_entity *entity, uint64_t *id)
{
    if (entity == NULL || *entity == NULL) {
        return ERR_INVALID_ARGS;
    }
    if (id != NULL) {
        *id = (*entity)->id;
    }
    return ERR_OK;
}

ERAPI er_entity_get_parent(er_entity *entity, er_entity *parent)
{
    if (entity == NULL || *entity == NULL) {
        return ERR_INVALID_ARGS;
    }
    if (parent != NULL) {
        *parent = (*entity)->parent;
    }
    return ERR_OK;
}

static JsonNode *entity_to_json(er_entity *entity, int export_children, int export_properties)
{
    struct er_entity *e;
    struct er_property_def *def = NULL, *def_tmp = NULL;
    struct er_property *prop = NULL;
    JsonNode *node = json_mkobject();
    JsonNode *properties, *children;
    JsonNode *prop_node;
    int i;
    json_append_number_member(node, "id", (*entity)->id);
    if (export_properties) {
        properties = json_mkobject();
        HASH_ITER(hh, g_property_definitions, def, def_tmp) {
            HASH_FIND_INT(def->instances, &(*entity)->id, prop);
            if (prop != NULL) {
                switch (def->type) {
                    case ER_PROP_BOOLEAN:
                        json_append_bool_member(properties, def->name, (prop->_bool)?true:false);
                        break;
                    case ER_PROP_NUMBER:
                        json_append_number_member(properties, def->name, prop->_number);
                        break;
                    case ER_PROP_STRING:
                        json_append_string_member(properties, def->name, prop->_string);
                        break;
                    case ER_PROP_BOOLEAN_ARRAY:
                        prop_node = json_mkarray();
                        for (i = 0; i < prop->array_count; i++) {
                            json_append_bool_element(prop_node, (((int*)prop->_unknown)[i])?true:false);
                        }
                        json_append_member(properties, def->name, prop_node);
                        break;
                    case ER_PROP_NUMBER_ARRAY:
                        prop_node = json_mkarray();
                        for (i = 0; i < prop->array_count; i++) {
                            json_append_number_element(prop_node, ((double*)prop->_unknown)[i]);
                        }
                        json_append_member(properties, def->name, prop_node);
                        break;
                    case ER_PROP_STRING_ARRAY:
                        prop_node = json_mkarray();
                        for (i = 0; i < prop->array_count; i++) {
                            json_append_string_element(prop_node, ((char**)prop->_unknown)[i]);
                        }
                        json_append_member(properties, def->name, prop_node);
                        break;
                    default:
                        json_append_null_member(properties, def->name);
                        break;
                }
            }
        }
        json_append_member(node, "properties", properties);
    }
    if (export_children) {
        children = json_mkarray();
        list_for_each(&(*entity)->children, e, siblings) {
            json_append_element(children, entity_to_json(&e, 1, export_properties));
        }
        json_append_member(node, "children", children);
    }
    return node;
}

ERAPI er_entity_export_json(er_entity *entity, int export_children, int export_properties, char **json)
{
    JsonNode *node;
    if (entity == NULL || *entity == NULL || json == NULL) {
        return ERR_INVALID_ARGS;
    }
    node = entity_to_json(entity, export_children, export_properties);
    *json = json_stringify(node, "  ");
    return ERR_OK;
}
