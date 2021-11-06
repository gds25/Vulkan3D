#include "simple_logger.h"
#include "gf3d_camera.h"
#include "agumon.h"

Entity *agumon_new() {
    Entity *ent = NULL;
    ent = entity_new();
    if (ent) {
        ent->model = gf3d_model_load("dino");
        ent->think = entity_think;
    }
    //vector3d_copy(ent->position,position)
    return ent;
}

void agumon_think(Entity* self) { // , Uint32 bufferFrame, VkCommandBuffer commandBuffer) {
    gfc_matrix_rotate(
        self->modelMat,
        self->modelMat,
        0.002,
        vector3d(1, 0, 0));
}

/*void entity_think(Entity* self) {
    gfc_matrix_rotate(
        self->modelMat,
        self->modelMat,
        0.002,
        vector3d(1, 0, 0));
}*/