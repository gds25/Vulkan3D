#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "player.h"
#include "monster.h"

Entity* monster_new(Vector3D position)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no player for you!");
        return NULL;
    }

    ent->model = gf3d_model_load("dino");
    // gfc_matrix_rotate(ent->modelMat, ent->modelMat, 90, vector3d(1, 0, 0));
    ent->think = monster_think;
    ent->update = monster_update;
    vector3d_copy(ent->position, position);
    ent->rotation.z = -M_PI;

    return ent;
}

void monster_think(Entity* self)
{
    Vector3D playerPos;
    playerPos = get_player_position();
    //slog("x = %f, y = %f, z = %f", playerPos.x,playerPos.y,playerPos.z);
    float maxChaseDist = 100;
    float d = (((self->position.x - playerPos.x) * (self->position.x - playerPos.x)) + ((self->position.y - playerPos.y) * (self->position.y - playerPos.y)) + ((self->position.z - playerPos.z) * (self->position.z - playerPos.z))); //get_distance_from_entity_squared(self, playerPos);

    //slog("distance = %f", d);
    //slog("hol up = %f", playerPos.x + playerPos.y + playerPos.z);


    if (d < maxChaseDist * maxChaseDist) {
        if (d < 10 * 10) monster_chase(self, playerPos);
        else {
            monster_chase(self, playerPos);
        }
    }
    else {
        monster_pace(self);
    }
}

void monster_update(Entity* self)
{
    if (!self)return;
    gfc_matrix_make_translation(self->modelMat, self->position);
    gfc_matrix_rotate(self->modelMat, self->modelMat, (2*M_PI)-self->rotation.z, vector3d(0, 0, 1));

}

float get_distance_from_entity_squared(Entity *self, Vector3D pos) {
    return pos.x + pos.y + pos.z;
    //return (((self->position.x - pos.x) * (self->position.x - pos.x)) + ((self->position.y - pos.y) * (self->position.y - pos.y)) + ((self->position.z - pos.z) * (self->position.z - pos.z)));
}

void monster_chase(Entity* self, Vector3D playerPos) {
    slog("here");
    self->rotation.z = atan(playerPos.y/playerPos.x);

    self->position.x += (0.03 * sin(self->rotation.z));
    self->position.y -= (0.03 * cos(self->rotation.z));
}


void monster_pace(Entity* self) {
    self->currentTime = SDL_GetTicks();
    if (self->currentTime > self->lastTime + 5000) {
        self->lastTime = self->currentTime;
        self->rotation.z = rand() * (2/M_PI);
    }
    self->position.x -= (0.01 * sin(self->rotation.z));
    self->position.y -= (0.01 * cos(self->rotation.z));
}
