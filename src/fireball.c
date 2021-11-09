#include "simple_logger.h"
#include "gfc_types.h"

#include "fireball.h"

void fireball_get_aabb(Entity* self) {
    self->maxAABB.x = self->position.x + 5; //-sin(self->rotation.z);
    self->maxAABB.y = self->position.y + 5; //+cos(self->rotation.z);
    self->maxAABB.z = self->position.z + 5;

    self->minAABB.x = self->futurePosition.x - 5; // +sin(self->rotation.z);
    self->minAABB.y = self->futurePosition.y - 5; // 6 - cos(self->rotation.z);
    self->minAABB.z = self->futurePosition.z - 5;
}

Entity* fireball_new(Vector3D position, float rotation)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no fireball for you!");
        return NULL;
    }

    ent->isFireball = 1;

    ent->model = gf3d_model_load("fireball");
    //slog("idle model: %i", ent->idleModel);
    //ent->model = ent->idleModel;

    // gfc_matrix_rotate(ent->modelMat, ent->modelMat, 90, vector3d(1, 0, 0));
    ent->think = fireball_think;
    ent->update = fireball_update;

    ent->position.y -= 5 * cos(ent->rotation.z);
    ent->position.x += 5 * sin(ent->rotation.z);
    ent->position.z += 5;

    ent->rotation.z = rotation;

    gfc_matrix_make_translation(ent->modelMat, ent->position);

    return ent;
}

void fireball_think(Entity* self) {
    self->futurePosition.y += (0.10 * cos(self->rotation.z));
    self->futurePosition.x -= (0.10 * sin(self->rotation.z));

    fireball_get_aabb(self);
    slog("position x = %f; position y = %f", self->position.x, self->position.y); 
}
void fireball_update(Entity* self) {
    //slog("here");
    if (self->futurePosition.x >= 295 || self->futurePosition.y >= 295 || self->futurePosition.x <= -295 || self->futurePosition.y <= -295) {
        entity_free(self);
        return;
    }
    gfc_matrix_make_translation(self->modelMat, self->position);
}
