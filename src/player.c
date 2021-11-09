#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "spell.h"
#include "monster.h"
#include "player.h"

static Entity player = {0};

// TODO : cube/prism AABB around entities
//      : line segment hitbox on weapons
//      : animate monster attacks (models are in folder)
//      : add long sword and double dagger animations/hitboxes

void player_get_aabb(Entity* self) {
    self->maxAABB.x = self->futurePosition.x + 8; //-sin(self->rotation.z);
    self->maxAABB.y = self->futurePosition.y + 8; //+cos(self->rotation.z);
    self->maxAABB.z = self->futurePosition.z + 8;

    self->minAABB.x = self->futurePosition.x - 5; // +sin(self->rotation.z);
    self->minAABB.y = self->futurePosition.y - 5; // 6 - cos(self->rotation.z);
    self->minAABB.z = self->futurePosition.z;
}

void sword_get_aabb(Entity* self) {
    self->maxWeaponAABB.x = self->position.x - 10 * sin(self->rotation.z); //-sin(self->rotation.z);
    self->maxWeaponAABB.y = self->position.y + 10 * cos(self->rotation.z); //+cos(self->rotation.z);
    self->maxWeaponAABB.z = self->position.z + 5;

    self->minWeaponAABB.x = self->position.x; // +sin(self->rotation.z);
    self->minWeaponAABB.y = self->position.y; // 6 - cos(self->rotation.z);
    self->minWeaponAABB.z = self->position.z;
}



Entity* player_new(Vector3D position)
{
    Entity* ent = NULL;
    
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no player for you!");
        return NULL;
    }

    ent->isPlayer = 1;

    ent->idleModel[0] = gf3d_model_load("playermodel");
    ent->idleModel[1] = gf3d_model_load("playermodel_2");
    ent->idleModel[2] = gf3d_model_load("playermodel_3");
    //slog("idle model: %i", ent->idleModel);
    ent->model = ent->idleModel;
    //gfc_matrix_scalar(ent->modelMat, .1);
   // gfc_matrix_rotate(ent->modelMat, ent->modelMat, 90, vector3d(1, 0, 0));
    ent->think = player_think;
    ent->update = player_update;
    vector3d_copy(ent->position, position);

    model_list_init(ent, 15, ent->modelList_attack, "sword_shield_attack_%i");
    model_list_init(ent, 13, ent->modelList_attack2, "long_sword_attack_%i");
    model_list_init(ent, 10, ent->modelList_attack3, "dual_sword_attack_%i");
   // ent->model = ent->modelList_attack[0];
    //ent->attackModel = gf3d_model_load("sword_shield_attack_5");
   // ent->attackModel2 = gf3d_model_load("sword_shield_attack_10");

    //slog("circumference = %f", 2*M_PI*100);
    ent->rotation.z= M_PI/2;
    //ent->rotation.x= -M_PI/2;
    //slog("position x = %f, position y = %f, position z = % f", ent->position.x,ent->position.y,ent->position.z);
   // slog("rotation z = %f", ent->rotation.z);

    ent->cameraPosition.x = ent->position.x;
    ent->cameraPosition.y = ent->position.y - 30;
    //self->cameraPosition.x = self->position.x;
    //self->cameraPosition.y = self->position.y - 100;
    ent->cameraPosition.z = ent->position.z + 10;

    ent->futurePosition.x = ent->position.x;
    ent->futurePosition.y = ent->position.y;
    ent->futurePosition.z = ent->position.z;

   // ent->acceleration.z = 10;
    ent->isJumping = 0;
    ent->isAttacking = 0;
    ent->isIdle = 1;
    ent->attackFrame = 0;
    ent->framesMax = 14;

    ent->health = 50;
    ent->mana = 100;
    ent->armor = 1;

    ent->velocity.x = 1;
    ent->velocity.y = 1;

    player_get_aabb(ent);
   // gf3d_camera_look_at(ent->cameraPosition, ent->position, vector3d(0, 0, 1));
   // gfc_matrix_rotate(ent->modelMat, ent->modelMat, M_PI, vector3d(0, 1, 0)); 
   // gfc_matrix_rotate(ent->modelMat, ent->modelMat, M_PI, vector3d(1, 0, 0));
    //ent->character = 1;

    return ent;
}

void player_think(Entity* self)
{
    const Uint8* keys;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

    if (keys[SDL_SCANCODE_W] && self->isIdle) {
        //slog("here");
        self->futurePosition.y += self->velocity.y * (0.015 * cos(self->rotation.z));
        self->futurePosition.x -= self->velocity.x * (0.015 * sin(self->rotation.z));
        //slog("position x = %f; position y = %f", self->position.x, self->position.y); 
        //slog("future position x = % f; future position y = % f, future position z = % f", self->futurePosition.x, self->futurePosition.y, self->futurePosition.z);

    }
    if (keys[SDL_SCANCODE_S] && self->isIdle) {
        self->futurePosition.y -= self->velocity.y * (0.015 * cos(self->rotation.z));
        self->futurePosition.x += self->velocity.x * (0.015 * sin(self->rotation.z));
        //slog("position x = % f; position y = % f", self->position.x, self->position.y); 
        //gfc_matrix_rotate(self->modelMat, self->modelMat, self->rotation.z, vector3d(0, 0, 1));
    }
    if (keys[SDL_SCANCODE_A] && self->isIdle) {
        self->futurePosition.y -= self->velocity.y * (0.015 * sin(self->rotation.z));
        self->futurePosition.x -= self->velocity.x * (0.015 * cos(self->rotation.z));
       // slog("position x = %f; position y = %f", self->position.x, self->position.y);
    }
    if (keys[SDL_SCANCODE_D] && self->isIdle) {
        self->futurePosition.y += self->velocity.y * (0.015 * sin(self->rotation.z));
        self->futurePosition.x += self->velocity.x * (0.015 * cos(self->rotation.z));
    }
    if (keys[SDL_SCANCODE_SPACE] && !self->isJumping) {
        self->isJumping = 1;
        //self->isIdle = 0;
        self->lastTime = SDL_GetTicks();
    }
    if (keys[SDL_SCANCODE_Z] && self->isIdle) {
        self->isAttacking = 1;
        self->isIdle = 0;
        self->lastTime = SDL_GetTicks();
    }

   /* if (keys[SDL_SCANCODE_UP])self->rotation.x += 0.0010;
    if (keys[SDL_SCANCODE_DOWN])self->rotation.x -= 0.0010;
    if (keys[SDL_SCANCODE_LEFT])self->rotation.z -= 0.0010; //slog("rotation z = %f", self->rotation.z); }
    if (keys[SDL_SCANCODE_RIGHT])self->rotation.z += 0.0010;// slog("rotation z = %f", self->rotation.z); }*/

    if (keys[SDL_SCANCODE_LSHIFT] && self->poweredUp == 0) {
        if (self->mana >= 20) {
            speed_boost(self);
            self->poweredUp = 1;
            self->lastPowerUpTime = SDL_GetTicks();
        }
        else slog("Not enough mana");
    }
    if (keys[SDL_SCANCODE_C] && self->poweredUp == 0) {
        if (self->mana >= 30) {
            defense_buff(self);
            self->poweredUp = 1;
            self->lastPowerUpTime = SDL_GetTicks();
        }
        else slog("Not enough mana");
    }
    if (keys[SDL_SCANCODE_H] && self->poweredUp == 0) {
        if (self->mana >= 25) {
            health_regen(self);
            self->poweredUp = 1;
            self->lastPowerUpTime = SDL_GetTicks();
        }
        else slog("Not enough mana");
    }

    if (keys[SDL_SCANCODE_M] && self->poweredUp == 0) {
        if (self->health >= 10) {
            mana_regen(self);
            self->poweredUp = 1;
            self->lastPowerUpTime = SDL_GetTicks();
        }
        else slog("Not enough health");
    }

    if (keys[SDL_SCANCODE_F] && self->poweredUp == 0) {
        if (self->mana >= 30) {
            self->poweredUp = 1;
            self->lastPowerUpTime = SDL_GetTicks();
            fireball(self);
        }
        else slog("Not enough mana");
    }

    if (keys[SDL_SCANCODE_1] && self->isIdle) {
        self->character = 0;
        self->framesMax = 14;
    }
    if (keys[SDL_SCANCODE_2] && self->isIdle) {
        self->character = 1;
        self->framesMax = 12;
    }
    if (keys[SDL_SCANCODE_3] && self->isIdle) {
        self->character = 2;
        self->framesMax = 9;
    }

    if (self->isJumping)player_jump(self);
    if (self->isAttacking) {
        //("attackFrame: %i", self->attackFrame);
        player_attack(self);

        //slog("here");
        //player_attack(self, self->modelList_attack, self->attackFrame);
    }
    else {
        self->model = self->idleModel[self->character];
    }

    if (self->poweredUp) {
        self->powerUpTime = SDL_GetTicks();
        //slog("powerup time: %i, last powerup time: %i", self->powerUpTime, self->lastPowerUpTime);

        if (self->powerUpTime - self->lastPowerUpTime > 5000) {
            slog("powerUp over");
            self->velocity.x = 1;
            self->velocity.y = 1;
            self->armor = 1;
            self->healthRegen = 0;
            self->manaRegen = 0;
            self->poweredUp = 0;
        }
        else if ((self->powerUpTime - self->lastPowerUpTime) % 1000 == 0) {
            self->health = MIN(100, self->health + self->healthRegen);
            self->mana = MIN(100, self->mana + self->manaRegen);
            slog("health: %i, mana: %i", self->health, self->mana);
        }
    }

    //self->health = MIN(100, self->health + self->healthRegen);

    set_player_position(self); 

    int x, y;
    SDL_GetMouseState(&x, &y);

   // SDL_Log("x and y of mouse: %i, %i", x, y);
   // slog("in player think");
    player_get_aabb(self);

    //slog("PLAYER max AABB x, y, z: %f, %f, %f, min AABB x, y, z: %f, %f, %f", self->maxAABB.x, self->maxAABB.y, self->maxAABB.z, self->minAABB.x, self->minAABB.y, self->minAABB.z);

    //slog("position x = %f; position y = %f", self->position.x, self->position.y);

    self->rotation.z = -(2*x)/(1200/M_PI);
    //self->rotation.x = (-(2 * y) / (700 / M_PI));
      //  MAX(M_PI/3, (MIN((2*M_PI/3), (-(2 * y) / (700 / M_PI)))));

    //slog("rotation x = %f", self->rotation.x);
    //slog("orig x: %f", (-(2 * y) / (700 / M_PI)));
}

void player_update(Entity* self)
{
    if (!self)return;
    //slog("position x = % f; position y = % f, position z = % f", self->position.x, self->position.y, self->position.z);
    //slog("future position x = % f; future position y = % f, future position z = % f", self->futurePosition.x, self->futurePosition.y, self->futurePosition.z);
    self->cameraPosition.x = self->position.x + (40*sin(self->rotation.z));
    self->cameraPosition.y = self->position.y - (40*cos(self->rotation.z));
    //self->cameraPosition.x = self->position.x;
    //self->cameraPosition.y = self->position.y - 100;
    self->cameraPosition.z = self->position.z + 10; // (100 * sin(self->rotation.x));


    gf3d_camera_set_position(self->cameraPosition);
    gf3d_camera_set_rotation(self->rotation);

    gfc_matrix_make_translation(self->modelMat, self->position);
   // gfc_matrix_translate(self->modelMat, self->position);
    //slog("update");
    gfc_matrix_rotate(self->modelMat, self->modelMat, self->rotation.z, vector3d(0, 0, 1));
   // gfc_matrix_rotate(self->modelMat, self->modelMat, M_PI, vector3d(0, 1, 0));
    //gfc_matrix_rotate(self->modelMat, self->modelMat, M_PI, vector3d(1, 0, 0));

    gf3d_camera_look_at(self->cameraPosition, self->position, vector3d(0, 0, 1));
    //slog("update2");
}


void player_jump(Entity* self) {
    self->currentTime = SDL_GetTicks();
    
    if (self->currentTime - self->lastTime < 1000) {
        if (self->currentTime - self->lastTime < 500) {
            self->futurePosition.z += 0.005;
        }
        else self->futurePosition.z -= 0.005;
    }
    else {
        self->position.z = 0.0;
        self->isJumping = 0;
        //self->isIdle = 1;
    }
    //isJumping = 0;
}

void player_attack(Entity *self) {
    //slog("attackFrame: %i", attackFrame);
    //slog("PLAYER max AABB x, y, z: %f, %f, %f, min AABB x, y, z: %f, %f, %f", self->maxAABB.x, self->maxAABB.y, self->maxAABB.z, self->minAABB.x, self->minAABB.y, self->minAABB.z);
    //slog("SWORD max AABB x, y, z: %f, %f, %f, min AABB x, y, z: %f, %f, %f", self->maxWeaponAABB.x, self->maxWeaponAABB.y, self->maxWeaponAABB.z, self->minWeaponAABB.x, self->minWeaponAABB.y, self->minWeaponAABB.z);
     sword_get_aabb(self);
     self->currentTime = SDL_GetTicks();
     if (self->currentTime - self->lastTime > 50) {
         if (self->attackFrame > self->framesMax) {
             self->attackFrame = 0;
             self->isIdle = 1;
             self->isAttacking = 0;
             self->model = self->idleModel[self->character];
             //gf3d_model_free(self->attackModel);
         }
         else {
             if (self->character == 0) {
                 self->model = self->modelList_attack[self->attackFrame];
             }if (self->character == 1) {
                 self->model = self->modelList_attack2[self->attackFrame];
             }if (self->character == 2) {
                 self->model = self->modelList_attack3[self->attackFrame];
             }
             self->attackFrame++;
         }
         self->lastTime = SDL_GetTicks();
     }
}

Vector3D get_player_position() {
    return player.position;
}

void set_player_position(Entity* self) {
    player.position = self->position;
}

/*eol@eof*/