#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "player.h"
#include "cube.h"
#include "health.h"
#include "monster.h"


void monster_die(Entity* self) {
    health_new(self->position);
    entity_free(self);
}

Entity* monster_new(Vector3D position, char* filename)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no player for you!");
        return NULL;
    }

    ent->isMonster = 1; 

    SJson* json, * pjson;
    char* modelName = NULL;

    json = sj_load(filename);
    if (!json)
    {
        slog("failed to load json file (%s) for the world data", filename);
        free(ent);
        return NULL;
    }
    pjson = sj_object_get_value(json, "monster");
    if (!pjson)
    {
        slog("failed to find moster object in %s monster config", filename);
        free(ent);
        sj_free(json);
        return NULL;
    }
    modelName = sj_get_string_value(sj_object_get_value(pjson, "model"));

    if (modelName)
    {
        ent->idleModel[0] = gf3d_model_load(modelName);
    }
    else
    {
        slog("monster data (%s) has no model", filename);
    }
    modelName = sj_get_string_value(sj_object_get_value(pjson, "modellist"));
    if (modelName)
    {
        model_list_init(ent, 16, ent->modelList_attack, modelName);
    }
    else
    {
        slog("monster data (%s) has no model", filename);
    }

    ent->health = sj_get_integer_value(sj_object_get_value(pjson, "health"), NULL);
    ent->damage = sj_get_integer_value(sj_object_get_value(pjson, "damage"), NULL);

    sj_free(json);

    //slog("idle model: %i", ent->idleModel);
    ent->model = ent->idleModel[0];
    // gfc_matrix_rotate(ent->modelMat, ent->modelMat, 90, vector3d(1, 0, 0));
    ent->think = monster_think;
    ent->update = monster_update;
    vector3d_copy(ent->position, position);

    ent->futurePosition.x = ent->position.x;
    ent->futurePosition.y = ent->position.y;
    ent->futurePosition.z = ent->position.z;

    monster_get_aabb(ent);

    ent->rotation.z = -M_PI;

    //ent->health = 100;
    //ent->damage = 10;

    return ent;
}

void monster_think(Entity* self)
{
    if (self->health <= 0) {
        monster_die(self);
        return;
    }
    Vector3D playerPos;
    playerPos = get_player_position();
    //slog("x = %f, y = %f, z = %f", playerPos.x,playerPos.y,playerPos.z);
    float maxChaseDist = 50;
    float maxAttackDist = 20;
    float d = (((self->futurePosition.x - playerPos.x) * (self->futurePosition.x - playerPos.x)) + ((self->futurePosition.y - playerPos.y) * (self->futurePosition.y - playerPos.y)) + ((self->futurePosition.z - playerPos.z) * (self->futurePosition.z - playerPos.z))); //get_distance_from_entity_squared(self, playerPos);

    //slog("distance = %f", d);
    //slog("hol up = %f", playerPos.x + playerPos.y + playerPos.z);

    if (d < maxAttackDist * maxAttackDist) {
        monster_attack(self);
    }
    else {
        self->attackFrame = 0;
        self->model = self->idleModel[0];
        if (d < maxChaseDist * maxChaseDist) {
            monster_chase(self, playerPos); 
        }
        else {
            monster_pace(self);
        }
    }
    
    monster_get_aabb(self);
    //slog("MONSTER max AABB x, y, z: %f, %f, %f, min AABB x, y, z: %f, %f, %f", self->maxAABB.x, self->maxAABB.y, self->maxAABB.z, self->minAABB.x, self->minAABB.y, self->minAABB.z);

}

void monster_update(Entity* self)
{
    if (!self)return;
    gfc_matrix_make_translation(self->modelMat, self->position);
    gfc_matrix_rotate(self->modelMat, self->modelMat, (2*M_PI)-self->rotation.z, vector3d(0, 0, 1));
    //gfc_matrix_scale(self->modelMat, vector3d(2, 2, 2));

}

float get_distance_from_entity_squared(Entity *self, Vector3D pos) {
    return pos.x + pos.y + pos.z;
    //return (((self->position.x - pos.x) * (self->position.x - pos.x)) + ((self->position.y - pos.y) * (self->position.y - pos.y)) + ((self->position.z - pos.z) * (self->position.z - pos.z)));
}

void monster_chase(Entity* self, Vector3D playerPos) {
    //slog("here");
    self->rotation.z = -atan((self->futurePosition.x - playerPos.x) / (self->futurePosition.y - playerPos.y));

    //self->futurePosition.x -= playerPos.x;
    //self->futurePosition.y -= playerPos.y;

    self->futurePosition.x -= (0.005 * sin(self->rotation.z));
    self->futurePosition.y += (0.005 * cos(self->rotation.z));
}

void monster_pace(Entity* self) {
    self->rotation.z += 0.0001;
   
    self->futurePosition.x -= (0.005 * sin(self->rotation.z));
    self->futurePosition.y += (0.005 * cos(self->rotation.z));
}

void monster_get_aabb(Entity* self) {
    self->maxAABB.x = self->futurePosition.x + 8; //-sin(self->rotation.z);
    self->maxAABB.y = self->futurePosition.y + 8; //+cos(self->rotation.z);
    self->maxAABB.z = self->futurePosition.z + 8;

    self->minAABB.x = self->futurePosition.x-8; // +sin(self->rotation.z);
    self->minAABB.y = self->futurePosition.y-8; // 6 - cos(self->rotation.z);
    self->minAABB.z = self->futurePosition.z-8;
}

void attack_get_aabb(Entity* self) {
    self->maxWeaponAABB.x = self->position.x - 10 * sin(self->rotation.z); //-sin(self->rotation.z);
    self->maxWeaponAABB.y = self->position.y + 10 * cos(self->rotation.z); //+cos(self->rotation.z);
    self->maxWeaponAABB.z = self->position.z + 5;

    self->minWeaponAABB.x = self->position.x; // +sin(self->rotation.z);
    self->minWeaponAABB.y = self->position.y; // 6 - cos(self->rotation.z);
    self->minWeaponAABB.z = self->position.z;
}

void monster_attack(Entity* self) {
    attack_get_aabb(self);
    //slog("attackFrame: %i", attackFrame);
    //slog("has attacked: %i", self->hasAttacked);
    self->jumpTime = SDL_GetTicks();
    if (self->jumpTime - self->lastJumpTime > 50) {
        if (self->attackFrame > 15) {
            self->attackFrame = 0;
            self->isIdle = 1;
            self->isAttacking = 0;
            self->hasAttacked = 0;
            //self->model = self->idleModel;
            //gf3d_model_free(self->attackModel);
        }
        else {
            self->model = self->modelList_attack[self->attackFrame];
            self->attackFrame++;
        }
        self->lastJumpTime = SDL_GetTicks();
    }
}

