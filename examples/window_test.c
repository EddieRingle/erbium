#include "erbium.h"

er_main()
{
    er_app_attrs_t attrs;

    er_init();

    er_app_attrs_init(&attrs);
    er_app_attrs_set_name(&attrs, "Window Test");
    er_app_attrs_set_author(&attrs, "Erbium");

    er_exec(&attrs, NULL, argc, argv);

    er_app_attrs_destroy(&attrs);

    er_quit();

    er_return_main();
}