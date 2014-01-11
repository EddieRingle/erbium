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

ERAPI er_entity_for_each_child(er_entity *entity, void (*fn)(er_entity *child))
{
    struct er_entity *e, *tmp = NULL;
    if (entity == NULL || *entity == NULL) {
        return ERR_INVALID_ARGS;
    }
    if (fn != NULL) {
        list_for_each_safe(&(*entity)->children, e, tmp, siblings) {
            fn(&e);
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
