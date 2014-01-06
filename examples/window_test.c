#include "erbium.h"

int _cb_exit_app(er_io_keyinfo_t keyinfo)
{
    if (keyinfo.state == ER_KEYSTATE_RELEASE) {
        er_stop();
        return 1;
    }
    return 0;
}

er_main()
{
    er_app_attrs_t attrs;
    er_context_attrs_t ctx_attrs;
    er_context_t ctx;

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

    er_exec(&ctx, argc, argv);

    er_ctx_close(&ctx);
    er_ctx_attrs_destroy(&ctx_attrs);

    er_quit();

    er_app_attrs_destroy(&attrs);

    er_return_main();
}