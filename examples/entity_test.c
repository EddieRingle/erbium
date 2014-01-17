#include "erbium.h"

int _cb_exit_app(er_io_keyinfo keyinfo)
{
    if (keyinfo.state == ER_KEYSTATE_RELEASE) {
        er_stop();
        return 1;
    }
    return 0;
}

void en_fn(er_entity *e)
{
    uint64_t id;
    er_entity_get_id(e, &id);
    printf(" |- Entity #%"PRId64"\n |\n", id);
}

void entities(void)
{
    int i;
    int booltest;
    double numbertest;
    const char *teststring = "This is a test string.";
    char *stringtest;
    uint64_t id;
    er_entity e1, e2, e3;
    char *json;

    er_entity_reserve(&e1);
    for (i = 0; i < 5; i++) {
        er_entity_reserve(&e2);
        er_entity_add_child(&e1, &e2);
        if (i == 3) {
            er_entity_reserve(&e3);
            er_entity_add_child(&e2, &e3);
        }
    }

    booltest = 1;
    er_prop_set_boolean(&e1, "booltest", booltest);
    booltest = 0;
    er_prop_get_boolean(&e1, "booltest", &booltest);
    printf("booltest: %d\n", booltest);

    numbertest = 3.14;
    er_prop_set_number(&e1, "numbertest", numbertest);
    numbertest = 0;
    er_prop_get_number(&e1, "numbertest", &numbertest);
    printf("numbertest: %f\n", numbertest);

    er_prop_set_string(&e1, "stringtest", teststring);
    er_prop_get_string(&e1, "stringtest", &stringtest);
    printf("stringtest: %s\n", stringtest);

    er_entity_get_id(&e1, &id);
    printf("Entity #%"PRId64"\n |\n", id);
    er_entity_for_each_child(&e1, 1, &en_fn);

    er_entity_export_json(&e1, 1, 1, &json);
    printf("JSON:\n%s\n", json);

    er_entity_import_json(&e3, json);
    numbertest = 0;
    er_prop_get_number(&e3, "numbertest", &numbertest);
    printf("numbertest, post-import: %f\n", numbertest);

    er_entity_release(&e1);
    er_entity_release(&e3);
}

er_main()
{
    er_app_attrs attrs;
    er_context_attrs ctx_attrs;
    er_context ctx;

    er_app_attrs_init(&attrs);
    er_app_attrs_set_name(&attrs, "Window Test");
    er_app_attrs_set_author(&attrs, "Erbium");

    er_init(&attrs);

    er_ctx_attrs_init(&ctx_attrs);
    er_ctx_attrs_set_screen_width(&ctx_attrs, 854);
    er_ctx_attrs_set_screen_height(&ctx_attrs, 480);
    er_ctx_open(&ctx_attrs, &ctx);

    er_io_register_action("exit_app", &_cb_exit_app);
    er_io_add_trigger("exit_app", ER_KEY_ESCAPE);

    entities();

    er_exec(&ctx, NULL, argc, argv);

    er_ctx_close(&ctx);
    er_ctx_attrs_destroy(&ctx_attrs);

    er_quit();

    er_app_attrs_destroy(&attrs);

    er_return_main();
}
