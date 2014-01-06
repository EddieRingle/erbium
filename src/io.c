#include "internal.h"

ERAPI er_io_register_action(const char *action_name, int (*action_cb)(er_io_keyinfo_t keyinfo))
{
    struct er_io_action_t *io_action = NULL;
    size_t len;

    INITCHECK();
    if (action_name == NULL || action_cb == NULL) {
        return ERR_INVALID_ARGS;
    }
    io_action = er__malloc(sizeof(struct er_io_action_t));
    memset(io_action, 0, sizeof(struct er_io_action_t));
    len = strlen(action_name);
    if (len > sizeof(io_action->action)) {
        LOGE("Action name must not exceed %zu characters\n", sizeof(io_action->action) / sizeof(char));
        return ERR_INVALID_ARGS;
    }
    strncpy(io_action->action, action_name, len);
    io_action->callback = action_cb;
    HASH_ADD_STR(g_io->action_map, action, io_action);

    return ERR_OK;
}

ERAPI er_io_unregister_action(const char *action_name)
{
    struct er_io_action_t *io_action = NULL;

    INITCHECK();
    if (action_name == NULL) {
        return ERR_INVALID_ARGS;
    }
    HASH_FIND_STR(g_io->action_map, action_name, io_action);
    if (io_action == NULL) {
        LOGE("Can't find action called '%s' to unregister\n", action_name);
        return ERR_UNKNOWN;
    }
    HASH_DEL(g_io->action_map, io_action);
    er__free(io_action);

    return ERR_OK;
}

ERAPI er_io_add_trigger(const char *action_name, int trigger)
{
    struct er_io_action_t *io_action = NULL;
    struct er_io_triggermap_t *io_trigger = NULL;

    INITCHECK();
    if (action_name == NULL) {
        return ERR_INVALID_ARGS;
    }
    io_trigger = er__malloc(sizeof(struct er_io_triggermap_t));
    HASH_FIND_STR(g_io->action_map, action_name, io_action);
    if (io_action == NULL) {
        LOGI("Can't add trigger to action: No action registered with name '%s'\n", action_name);
        return ERR_UNKNOWN;
    }
    io_trigger->trigger = trigger;
    io_trigger->action = io_action;
    HASH_ADD_INT(g_io->trigger_map, trigger, io_trigger);

    return ERR_OK;
}

ERAPI er_io_clear_action(const char *action_name)
{
    struct er_io_action_t *io_action = NULL;
    struct er_io_triggermap_t *io_trigger = NULL, *tmp_trigger = NULL;

    INITCHECK();
    if (action_name == NULL) {
        return ERR_INVALID_ARGS;
    }
    HASH_FIND_STR(g_io->action_map, action_name, io_action);
    if (io_action == NULL) {
        LOGI("Can't clear triggers from action: No action registered with name '%s'\n", action_name);
        return ERR_UNKNOWN;
    }
    HASH_ITER(hh, g_io->trigger_map, io_trigger, tmp_trigger) {
        if (io_action == io_trigger->action) {
            HASH_DEL(g_io->trigger_map, io_trigger);
            er__free(io_trigger);
        }
    }

    return ERR_OK;
}

ERAPI er_io_clear_trigger(int trigger)
{
    struct er_io_triggermap_t *io_trigger = NULL, *tmp_trigger = NULL;

    INITCHECK();
    HASH_FIND_INT(g_io->trigger_map, &trigger, io_trigger);
    if (io_trigger == NULL) {
        return ERR_UNKNOWN;
    }
    HASH_DEL(g_io->trigger_map, io_trigger);
    er__free(io_trigger);

    return ERR_OK;
}
