#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "player.h"
#include "cube.h"
#include "health.h"
#include "monster.h"
#include "boss.h"

Entity* boss_new(Vector3D position, char* filename)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no player for you!");
        return NULL;
    }

    ent->isMonster = 1;

    SJson* json, * pjson, * mjson;
    char* modelName1 = NULL;
    char* modelName2 = NULL;
    char* modelName3 = NULL;

    json = sj_load(filename);
    if (!json)
    {
        slog("failed to load json file (%s) for the world data", filename);
        free(ent);
        return NULL;
    }
    pjson = sj_object_get_value(json, "boss");
    if (!pjson)
    {
        slog("failed to find moster object in %s monster config", filename);
        free(ent);
        sj_free(json);
        return NULL;
    }
    modelName1 = sj_get_string_value(sj_object_get_value(pjson, "model"));

    if (modelName1)
    {
        ent->idleModel[0] = gf3d_model_load(modelName1);
    }
    else
    {
        slog("monster data (%s) has no model", filename);
    }
    mjson = sj_object_get_value(pjson, "modellist");
    slog("here");
    modelName1 = sj_get_string_value(sj_object_get_value(mjson, "walk"));
    modelName2 = sj_get_string_value(sj_object_get_value(mjson, "stomp"));
    //modelName3 = sj_get_string_value(sj_object_get_value(mjson, "crawl"));

    if (modelName1)
    {
        model_list_init(ent, 19, ent->modelList_attack, modelName1);
    }
    else
    {
        slog("player data (%s) has no model", filename);
    }
    if (modelName2)
    {
        model_list_init(ent, 29, ent->modelList_attack2, modelName2);
    }
    else
    {
        slog("player data (%s) has no model", filename);
    }
    /*if (modelName3)
    {
        model_list_init(ent, 10, ent->modelList_attack3, modelName3);
    }
    else
    {
        slog("player data (%s) has no model", filename);
    }*/

    ent->health = sj_get_integer_value(sj_object_get_value(pjson, "health"), NULL);
    ent->mana = sj_get_integer_value(sj_object_get_value(pjson, "mana"), NULL);
    ent->damage = sj_get_integer_value(sj_object_get_value(pjson, "damage"), NULL);
    ent->armor = sj_get_integer_value(sj_object_get_value(pjson, "armor"), NULL);

    sj_free(json);

    //slog("idle model: %i", ent->idleModel);
    ent->model = ent->idleModel[0];
    // gfc_matrix_rotate(ent->modelMat, ent->modelMat, 90, vector3d(1, 0, 0));
    ent->think = boss_think;
    ent->update = boss_update;
    vector3d_copy(ent->position, position);

    ent->futurePosition.x = ent->position.x;
    ent->futurePosition.y = ent->position.y;
    ent->futurePosition.z = ent->position.z;

    ent->walkFrame = 0.0;

    boss_get_aabb(ent);

    ent->rotation.z = -M_PI;

    //ent->health = 100;
    //ent->damage = 10;

    return ent;
}

void boss_think(Entity* self)
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
        self->walkFrame = 0;
        boss_stomp(self);
    }
    else {
        if (self->attackFrame == 0)
            boss_walk(self);
        else boss_stomp(self);
    }

    boss_get_aabb(self);
    //slog("MONSTER max AABB x, y, z: %f, %f, %f, min AABB x, y, z: %f, %f, %f", self->maxAABB.x, self->maxAABB.y, self->maxAABB.z, self->minAABB.x, self->minAABB.y, self->minAABB.z);

}

void boss_update(Entity* self)
{
    if (!self)return;
    gfc_matrix_make_translation(self->modelMat, self->position);
    gfc_matrix_rotate(self->modelMat, self->modelMat, (2 * M_PI) - self->rotation.z, vector3d(0, 0, 1));
    //gfc_matrix_scale(self->modelMat, vector3d(2, 2, 2));

}

void boss_walk(Entity* self) {
    self->rotation.z += 0.0003;

    self->futurePosition.x -= (0.015 * sin(self->rotation.z));
    self->futurePosition.y += (0.015 * cos(self->rotation.z));

    self->walkFrame += 0.007;

    if (self->walkFrame >= 19)self->walkFrame = 0;
    self->model = self->modelList_attack[(Uint32)self->walkFrame];
}

void boss_get_aabb(Entity* self) {
    self->maxAABB.x = self->futurePosition.x + 16; //-sin(self->rotation.z);
    self->maxAABB.y = self->futurePosition.y + 16; //+cos(self->rotation.z);
    self->maxAABB.z = self->futurePosition.z + 16;

    self->minAABB.x = self->futurePosition.x - 16; // +sin(self->rotation.z);
    self->minAABB.y = self->futurePosition.y - 16; // 6 - cos(self->rotation.z);
    self->minAABB.z = self->futurePosition.z - 16;
}

void boss_attack_get_aabb(Entity* self) {
    self->maxWeaponAABB.x = self->position.x + 20; //-sin(self->rotation.z);
    self->maxWeaponAABB.y = self->position.y + 20; //+cos(self->rotation.z);
    self->maxWeaponAABB.z = self->position.z + 5;

    self->minWeaponAABB.x = self->position.x + 10; ; // +sin(self->rotation.z);
    self->minWeaponAABB.y = self->position.y + 10; // 6 - cos(self->rotation.z);
    self->minWeaponAABB.z = self->position.z - 5;
}

void boss_stomp(Entity* self) {
    boss_attack_get_aabb(self);
    //slog("attackFrame: %i", attackFrame);
    //slog("has attacked: %i", self->hasAttacked);
    self->jumpTime = SDL_GetTicks();
    if (self->jumpTime - self->lastJumpTime > 100) {
        if (self->attackFrame > 28) {
            self->attackFrame = 0;
            self->isIdle = 1;
            self->isAttacking = 0;
            self->hasAttacked = 0;
            //self->model = self->idleModel;
            //gf3d_model_free(self->attackModel);
        }
        else {
            self->model = self->modelList_attack2[self->attackFrame];
            self->attackFrame++;
        }
        self->lastJumpTime = SDL_GetTicks();
    }
}

