#include "simple_logger.h"
#include "gfc_types.h"

#include "cube.h"

Entity* cube_new(Vector3D position, float rotation)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no block for you!");
        return NULL;
    }

    ent->model = gf3d_model_load("cube");
    gfc_matrix_identity(ent->modelMat);
  
    //ent->think = cube_get_aabb;
    // gfc_matrix_rotate(ent->modelMat, ent->modelMat, 90, vector3d(1, 0, 0));

    vector3d_copy(ent->position, position);

    ent->rotation.z = rotation;

    ent->isStatic = 1;

    gfc_matrix_rotate(ent->modelMat, ent->modelMat, ent->rotation.z, vector3d(0, 0, 1));

    cube_get_aabb(ent);

    return ent;
}

void cube_get_aabb(Entity* self) {
    //slog("CUBE max AABB x, y, z: %f, %f, %f, min AABB x, y, z: %f, %f, %f", self->maxAABB.x, self->maxAABB.y, self->maxAABB.z, self->minAABB.x, self->minAABB.y, self->minAABB.z);
    self->maxAABB.x = self->position.x + 20; //-sin(self->rotation.z);
    self->maxAABB.y = self->position.y + 20; //+cos(self->rotation.z);
    self->maxAABB.z = self->position.z + 20;

    self->minAABB.x = self->position.x - 20; // +sin(self->rotation.z);
    self->minAABB.y = self->position.y - 20; // 6 - cos(self->rotation.z);
    self->minAABB.z = self->position.z - 20;
}
