#include "erbium.h"

int _cb_exit_app(er_io_keyinfo keyinfo)
{
    if (keyinfo.state == ER_KEYSTATE_RELEASE) {
        er_stop();
        return 1;
    }
    return 0;
}

er_main()
{
    er_app_attrs attrs;
    er_context_attrs ctx_attrs;
    er_context ctx;
    er_path_result path_result;

    er_app_attrs_init(&attrs);
    er_app_attrs_set_name(&attrs, "Window Test");
    er_app_attrs_set_author(&attrs, "Erbium");

    er_init(&attrs);

    er_app_get_path(ER_PATH_BINARY, &path_result);
    LOGI("Binary path:\t %s\n", path_result.path);
    er_app_get_path(ER_PATH_SUPPORT, &path_result);
    LOGI("Support path:\t %s\n", path_result.path);
    er_app_get_path(ER_PATH_ASSETS, &path_result);
    LOGI("Assets path:\t %s\n", path_result.path);
    er_app_get_path(ER_PATH_TEXTURES, &path_result);
    LOGI("Textures path:\t %s\n", path_result.path);
    er_app_cleanup_path_result(&path_result);

    er_ctx_attrs_init(&ctx_attrs);
    er_ctx_attrs_set_screen_width(&ctx_attrs, 854);
    er_ctx_attrs_set_screen_height(&ctx_attrs, 480);
    er_ctx_open(&ctx_attrs, &ctx);

    er_io_register_action("exit_app", &_cb_exit_app);
    er_io_add_trigger("exit_app", ER_KEY_ESCAPE);

    er_exec(&ctx, NULL, argc, argv);

    er_ctx_close(&ctx);
    er_ctx_attrs_destroy(&ctx_attrs);

    er_quit();

    er_app_attrs_destroy(&attrs);

    er_return_main();
}
