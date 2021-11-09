#include "simple_logger.h"
#include "gfc_types.h"

#include "health.h"

void health_get_aabb(Entity* self) {
    self->maxAABB.x = self->position.x + 5; //-sin(self->rotation.z);
    self->maxAABB.y = self->position.y + 5; //+cos(self->rotation.z);
    self->maxAABB.z = self->position.z + 3;

    self->minAABB.x = self->position.x; // +sin(self->rotation.z);
    self->minAABB.y = self->position.y; // 6 - cos(self->rotation.z);
    self->minAABB.z = self->position.z;
}

Entity* health_new(Vector3D position)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no fireball for you!");
        return NULL;
    }

    ent->isHealth = 1;

    ent->model = gf3d_model_load("health_pack");

    vector3d_copy(ent->position, position);
    vector3d_copy(ent->futurePosition, position);

    ent->position.z += 5;

    gfc_matrix_make_translation(ent->modelMat, ent->position);

    health_get_aabb(ent);

    return ent;
}

