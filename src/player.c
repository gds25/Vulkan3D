#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "spell.h"
#include "player.h"

static Entity player = {0};

// TODO : cube/prism AABB around entities
//      : line segment hitbox on weapons
//      : animate monster attacks (models are in folder)
//      : add long sword and double dagger animations/hitboxes


void model_list_init(Entity* self, Uint32 max) {
    //self->modelList_attack = gfc_allocate_array(sizeof(Model*), max);
    if (self->modelList_attack == NULL) {
        //slog("Model system initialization error: cannot allocate 0 entities.");
        return;
    }
    for (int i = 0; i < max; i++) {
        TextLine modelName;
        snprintf(modelName, GFCLINELEN, "sword_shield_attack_%i", i);
        slog(modelName);
       // Model modelInsert = gf3d_model_load(modelName);
        //memcpy(&self->modelList_attack[i], gf3d_model_load(modelName), sizeof(Model*));
        self->modelList_attack[i] = gf3d_model_load(modelName);
        slog("model: %i", self->modelList_attack[i]);
    }
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

    ent->idleModel = gf3d_model_load("cube");
    slog("idle model: %i", ent->idleModel);
    ent->model = ent->idleModel;
    gfc_matrix_scalar(ent->modelMat, .1);
   // gfc_matrix_rotate(ent->modelMat, ent->modelMat, 90, vector3d(1, 0, 0));
    ent->think = player_think;
    ent->update = player_update;
    vector3d_copy(ent->position, position);

    model_list_init(ent, 15);
   // ent->model = ent->modelList_attack[0];
    //ent->attackModel = gf3d_model_load("sword_shield_attack_5");
   // ent->attackModel2 = gf3d_model_load("sword_shield_attack_10");

    slog("circumference = %f", 2*M_PI*100);
    ent->rotation.z= M_PI/2;
    //ent->rotation.x= -M_PI/2;
    slog("position x = %f, position y = %f, position z = % f", ent->position.x,ent->position.y,ent->position.z);
    slog("rotation z = %f", ent->rotation.z);

    ent->cameraPosition.x = ent->position.x;
    ent->cameraPosition.y = ent->position.y - 100;
    //self->cameraPosition.x = self->position.x;
    //self->cameraPosition.y = self->position.y - 100;
    ent->cameraPosition.z = ent->position.z + 50;

   // ent->acceleration.z = 10;
    ent->isJumping = 0;
    ent->isAttacking = 0;
    ent->isIdle = 1;
    ent->attackFrame = 0;

    ent->health = 100;
    ent->mana = 100;

    ent->velocity.x = 1;
    ent->velocity.y = 1;
   // gf3d_camera_look_at(ent->cameraPosition, ent->position, vector3d(0, 0, 1));
    gfc_matrix_rotate(ent->modelMat, ent->modelMat, M_PI, vector3d(0, 1, 0)); 
    gfc_matrix_rotate(ent->modelMat, ent->modelMat, M_PI, vector3d(1, 0, 0));
    return ent;
}

void player_think(Entity* self)
{
    const Uint8* keys;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

    if (keys[SDL_SCANCODE_W] && self->isIdle) {
        self->position.y += self->velocity.y * (0.10 * cos(self->rotation.z));
        self->position.x -= self->velocity.x * (0.10 * sin(self->rotation.z));
        slog("position x = %f; position y = %f", self->position.x, self->position.y); 
        
    }
    if (keys[SDL_SCANCODE_S] && self->isIdle) {
        self->position.y -= self->velocity.y *(0.10 * cos(self->rotation.z)); 
        self->position.x += self->velocity.x *(0.10 * sin(self->rotation.z));
        //slog("position x = % f; position y = % f", self->position.x, self->position.y); 
        //gfc_matrix_rotate(self->modelMat, self->modelMat, self->rotation.z, vector3d(0, 0, 1));
    }
    if (keys[SDL_SCANCODE_A] && self->isIdle) {
        self->position.y -= self->velocity.y * (0.10 * sin(self->rotation.z));
        self->position.x -= self->velocity.x * (0.10 * cos(self->rotation.z));
       // slog("position x = %f; position y = %f", self->position.x, self->position.y);
    }
    if (keys[SDL_SCANCODE_D] && self->isIdle) {
        self->position.y += self->velocity.y * (0.10 * sin(self->rotation.z));
        self->position.x += self->velocity.x * (0.10 * cos(self->rotation.z));
    }
    if (keys[SDL_SCANCODE_SPACE]&& self->isIdle) {
        self->isJumping = 1;
        self->isIdle = 0;
        self->lastJumpTime = SDL_GetTicks();
    }
    if (keys[SDL_SCANCODE_Z] && self->isIdle) {
        self->isAttacking = 1;
        self->isIdle = 0;
    }

    if (keys[SDL_SCANCODE_UP])self->rotation.x += 0.0010;
    if (keys[SDL_SCANCODE_DOWN])self->rotation.x -= 0.0010;
    if (keys[SDL_SCANCODE_LEFT])self->rotation.z -= 0.0010; //slog("rotation z = %f", self->rotation.z); }
    if (keys[SDL_SCANCODE_RIGHT])self->rotation.z += 0.0010;// slog("rotation z = %f", self->rotation.z); }

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

    if (keys[SDL_SCANCODE_F]) {
        if (self->mana >= 30) {
            fireball(self);
        }
        else slog("Not enough mana");
    }

    if (self->isJumping)player_jump(self);
    if (self->isAttacking) {
        //("attackFrame: %i", self->attackFrame);
        self->model = self->modelList_attack[self->attackFrame];

        //slog("here");
        //player_attack(self, self->modelList_attack, self->attackFrame);
        self->attackFrame++;
        if (self->attackFrame > 14) {
            self->attackFrame = 0;
            self->isIdle = 1;
            self->isAttacking = 0;
            //self->model = gf3d_model_load("playermodel");
            //gf3d_model_free(self->attackModel);
        }
    }
    else {
        self->model = self->idleModel;
    }

    if (self->poweredUp) {
        self->powerUpTime = SDL_GetTicks();
        slog("powerup time: %i, last powerup time: %i", self->powerUpTime, self->lastPowerUpTime);

        if (self->powerUpTime - self->lastPowerUpTime > 5000) {
            slog("powerUp over");
            self->velocity.x = 1;
            self->velocity.y = 1;
            self->armor = 1;
            self->healthRegen = 0;
            self->poweredUp = 0;
        }
        else if (self->powerUpTime - self->lastPowerUpTime % 1000 == 0) {
            self->health = MIN(100, self->health + self->healthRegen);
        }
    }

    //self->health = MIN(100, self->health + self->healthRegen);

    set_player_position(self); 

    int x, y;
    SDL_GetMouseState(&x, &y);

   // SDL_Log("x and y of mouse: %i, %i", x, y);
   // slog("in player think");
    self->rotation.z = -(2*x)/(1200/M_PI);
    //self->rotation.x = (-(2 * y) / (700 / M_PI));
      //  MAX(M_PI/3, (MIN((2*M_PI/3), (-(2 * y) / (700 / M_PI)))));

    //slog("rotation x = %f", self->rotation.x);
    //slog("orig x: %f", (-(2 * y) / (700 / M_PI)));
}

void player_update(Entity* self)
{
    if (!self)return;
    self->cameraPosition.x = self->position.x + (100*sin(self->rotation.z));
    self->cameraPosition.y = self->position.y - (100*cos(self->rotation.z));
    //self->cameraPosition.x = self->position.x;
    //self->cameraPosition.y = self->position.y - 100;
    self->cameraPosition.z = self->position.z + 20; // (100 * sin(self->rotation.x));


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
    self->jumpTime = SDL_GetTicks();
    
    if (self->jumpTime - self->lastJumpTime < 1400) {
        if (self->jumpTime - self->lastJumpTime < 700) {
            self->position.z += 0.05;
        }
        else self->position.z -= 0.05;
    }
    else {
        self->position.z = 0.0;
        self->isJumping = 0;
        self->isIdle = 1;
    }
    //isJumping = 0;
}

void player_attack(Entity *self, Uint8 attackFrame) {
    //slog("attackFrame: %i", attackFrame);

     self->model = self->modelList_attack[attackFrame];
}

Vector3D get_player_position() {
    return player.position;
}

Vector3D set_player_position(Entity* self) {
    player.position = self->position;
}

/*eol@eof*/