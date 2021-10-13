#include "simple_logger.h"

#include "entity.h"

Entity *agumon_new() {
    Entity *ent = NULL;
    ent = entity_new();
    if (ent) {
        ent->model = gf3d_model_load("dino");
        ent->think = entity_think(ent);
    }
    return ent;
}

void *entity_think(Entity* self) { // , Uint32 bufferFrame, VkCommandBuffer commandBuffer) {
    gfc_matrix_rotate(
        self->modelMat,
        self->modelMat,
        0.002,
        vector3d(1, 0, 0));
}