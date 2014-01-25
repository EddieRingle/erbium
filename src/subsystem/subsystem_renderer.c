#include "subsystem_defaults.h"

#include "renderer/renderer.h"

static er_matrix_stack proj_stack;
static er_matrix_stack view_stack;
static er_matrix_stack model_stack;

void er__ssys_renderer_init_cb(void)
{
    er_matrix *mat;
    er__renderer_init();

    er_mstack_init(&proj_stack);
    er_mstack_push(&proj_stack);
    er_mstack_peek(&proj_stack, &mat);
    er_matrix_identity(mat);

    er_mstack_init(&view_stack);
    er_mstack_push(&view_stack);
    er_mstack_peek(&view_stack, &mat);
    er_matrix_identity(mat);

    er_mstack_init(&model_stack);
    er_mstack_push(&model_stack);
    er_mstack_peek(&model_stack, &mat);
    er_matrix_identity(mat);
}

void er__ssys_renderer_update_cb(er_entity root, double delta)
{
    glClearColor(0.5f, 0.6f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void er__ssys_renderer_quit_cb(void)
{
    er_mstack_destroy(&model_stack);
    er_mstack_destroy(&view_stack);
    er_mstack_destroy(&proj_stack);
    er__renderer_quit();
}

void er__subsystem_register_default_renderer(void)
{
    er_subsystem_attrs attrs;

    er_subsystem_attrs_init(&attrs);
    er_subsystem_attrs_set_name(&attrs, "renderer");
    er_subsystem_attrs_set_init_cb(&attrs, &er__ssys_renderer_init_cb);
    er_subsystem_attrs_set_update_cb(&attrs, &er__ssys_renderer_update_cb);
    er_subsystem_attrs_set_quit_cb(&attrs, &er__ssys_renderer_quit_cb);
    er_subsystem_register(&attrs, NULL);
    er_subsystem_attrs_destroy(&attrs);
}
