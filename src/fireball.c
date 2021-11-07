#include "simple_logger.h"
#include "gfc_types.h"

#include "fireball.h"

Entity* fireball_new(Vector3D position, float rotation)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no fireball for you!");
        return NULL;
    }

    ent->model = gf3d_model_load("fireball");

    // gfc_matrix_rotate(ent->modelMat, ent->modelMat, 90, vector3d(1, 0, 0));
    ent->think = fireball_think;
    ent->update = fireball_update;
    vector3d_copy(ent->position, position);

    ent->rotation.z = rotation;

    // ent->acceleration.z = 10;
    ent->damage = 20;

    ent->velocity.x = 1;
    ent->velocity.y = 1;

    return ent;
}

void fireball_think(Entity* self) {
    self->position.y += self->velocity.y * (0.10 * cos(self->rotation.z));
    self->position.x -= self->velocity.x * (0.10 * sin(self->rotation.z));
}
void fireball_update(Entity* self) {
    gfc_matrix_make_translation(self->modelMat, self->position);
}