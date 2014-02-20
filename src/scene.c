#include "internal.h"

ERAPI er_scene_create(er_scene *scene)
{
    ERR ret;
    INITCHECK();
    if (scene == NULL) {
        return ERR_INVALID_ARGS;
    }
    *scene = er__malloc(sizeof(struct er_scene));
    if (*scene == NULL) {
        return ERR_MEMORY_ERROR;
    }
    if ((ret = er_entity_reserve(&(*scene)->entity)) != ERR_OK) {
        er__free(*scene);
        *scene = NULL;
        return ret;
    }
    if ((ret = er_prop_set_boolean(&(*scene)->entity, "scene", 1)) != ERR_OK) {
        er_entity_release(&(*scene)->entity);
        er__free(*scene);
        *scene = NULL;
        return ret;
    }
    (*scene)->on_create = NULL;
    (*scene)->on_destroy = NULL;
    return ERR_OK;
}

ERAPI er_scene_destroy(er_scene *scene)
{
    ERR ret;
    INITCHECK();
    if (scene == NULL || *scene == NULL) {
        return ERR_INVALID_ARGS;
    }
    ret = er_entity_release(&(*scene)->entity);
    er__free(*scene);
    *scene = NULL;
    return ret;
}

ERAPI er_scene_set_on_create_cb(er_scene *scene, void (*cb)(er_entity root))
{
    if (scene == NULL || *scene == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*scene)->on_create = cb;
    return ERR_OK;
}

ERAPI er_scene_set_on_destroy_cb(er_scene *scene, void (*cb)(er_entity root))
{
    if (scene == NULL || *scene == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*scene)->on_destroy = cb;
    return ERR_OK;
}
