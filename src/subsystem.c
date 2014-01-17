#include "internal.h"

struct er_subsystem *g_subsystems = NULL;

ERAPI er_subsystem_attrs_init(er_subsystem_attrs *attrs)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    *attrs = er__malloc(sizeof(struct er_subsystem_attrs));
    if (*attrs == NULL) {
        return ERR_MEMORY_ERROR;
    }
    memset(*attrs, 0, sizeof(struct er_subsystem_attrs));
    return ERR_OK;
}

ERAPI er_subsystem_attrs_set_name(er_subsystem_attrs *attrs, const char *name)
{
    if (attrs == NULL || name == NULL) {
        return ERR_INVALID_ARGS;
    }
    memset((*attrs)->name, 0, sizeof (*attrs)->name);
    strncpy((*attrs)->name, name, (sizeof (*attrs)->name) / sizeof(char));
    return ERR_OK;
}

ERAPI er_subsystem_attrs_set_init_cb(er_subsystem_attrs *attrs, void (*cb)(void))
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*attrs)->init = cb;
    return ERR_OK;
}

ERAPI er_subsystem_attrs_set_update_cb(er_subsystem_attrs *attrs, void (*cb)(er_entity root, double delta))
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*attrs)->update = cb;
    return ERR_OK;
}

ERAPI er_subsystem_attrs_set_quit_cb(er_subsystem_attrs *attrs, void (*cb)(void))
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    (*attrs)->quit = cb;
    return ERR_OK;
}

ERAPI er_subsystem_attrs_destroy(er_subsystem_attrs *attrs)
{
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    er__free(*attrs);
    *attrs = NULL;
    return ERR_OK;
}

ERAPI er_subsystem_register(er_subsystem_attrs *attrs, er_subsystem *subsystem)
{
    struct er_subsystem *system, *tmp = NULL;
    INITCHECK();
    if (attrs == NULL) {
        return ERR_INVALID_ARGS;
    }
    system = er__malloc(sizeof(struct er_subsystem));
    if (system == NULL) {
        return ERR_MEMORY_ERROR;
    }
    memcpy(&system->attrs, (*attrs), sizeof(struct er_subsystem_attrs));
    HASH_FIND_STR(g_subsystems, system->attrs.name, tmp);
    if (tmp != NULL) {
        LOGE("A subsystem is already registered with the name '%s'\n", system->attrs.name);
        er__free(system);
        return ERR_UNKNOWN;
    }
    HASH_ADD_STR(g_subsystems, attrs.name, system);
    if (system->attrs.init != NULL) {
        system->attrs.init();
    }
    if (subsystem != NULL) {
        *subsystem = system;
    }
    return ERR_OK;
}

ERAPI er_subsystem_unregister(er_subsystem *subsystem)
{
    if (subsystem == NULL || *subsystem == NULL) {
        return ERR_INVALID_ARGS;
    }
    if ((*subsystem)->attrs.quit != NULL) {
        (*subsystem)->attrs.quit();
    }
    HASH_DEL(g_subsystems, *subsystem);
    er__free(*subsystem);
    *subsystem = NULL;
    return ERR_OK;
}
