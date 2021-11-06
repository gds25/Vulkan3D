#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "player.h"

static Entity player = {0};

//void player_update_view(Entity* self);

Entity* player_new(Vector3D position)
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
    ent->think = player_think;
    ent->update = player_update;
    vector3d_copy(ent->position, position);
    slog("circumference = %f", 2*M_PI*100);
    ent->rotation.z=  M_PI;
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
   // gf3d_camera_look_at(ent->cameraPosition, ent->position, vector3d(0, 0, 1));
    //gfc_matrix_rotate(self->modelMat, self->modelMat, self->rotation.z, vector3d(0, 0, 1));
    return ent;
}

void player_think(Entity* self)
{
    const Uint8* keys;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

    if (keys[SDL_SCANCODE_W]) { 
        self->position.y += (0.10 * cos(self->rotation.z)); 
        self->position.x -= (0.10 * sin(self->rotation.z)); 
        //slog("position x = %f; position y = %f", self->position.x, self->position.y); 
        
    }
    if (keys[SDL_SCANCODE_S]) {
        self->position.y -= (0.10 * cos(self->rotation.z)); 
        self->position.x += (0.10 * sin(self->rotation.z)); 
        //slog("position x = % f; position y = % f", self->position.x, self->position.y); 
        //gfc_matrix_rotate(self->modelMat, self->modelMat, self->rotation.z, vector3d(0, 0, 1));
    }
    if (keys[SDL_SCANCODE_A]) {
        self->position.y -= (0.10 * sin(self->rotation.z));
        self->position.x -= (0.10 * cos(self->rotation.z)); 
       // slog("position x = %f; position y = %f", self->position.x, self->position.y);
    }
    if (keys[SDL_SCANCODE_D]) {
        self->position.y += (0.10 * sin(self->rotation.z));
        self->position.x += (0.10 * cos(self->rotation.z));
    }
    if (keys[SDL_SCANCODE_SPACE]&& self->isJumping == 0) {
        self->isJumping = 1;
        self->lastTime = SDL_GetTicks();
    }
    if (keys[SDL_SCANCODE_Z]) self->position.z -= 0.10; 

    if (keys[SDL_SCANCODE_UP])self->rotation.x += 0.0010;
    if (keys[SDL_SCANCODE_DOWN])self->rotation.x -= 0.0010;
    if (keys[SDL_SCANCODE_LEFT])self->rotation.z -= 0.0010; //slog("rotation z = %f", self->rotation.z); }
    if (keys[SDL_SCANCODE_RIGHT])self->rotation.z += 0.0010;// slog("rotation z = %f", self->rotation.z); }


    if (self->isJumping)player_jump(self);

    set_player_position(self); 

    int x, y;
    SDL_GetMouseState(&x, &y);

   // SDL_Log("x and y of mouse: %i, %i", x, y);
   // slog("in player think");
    self->rotation.z = (2*x)/(1200/M_PI);
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
    self->cameraPosition.z = self->position.z + 50; // (100 * sin(self->rotation.x));


    gf3d_camera_set_position(self->cameraPosition);
    gf3d_camera_set_rotation(self->rotation);

    gfc_matrix_make_translation(self->modelMat, self->position);
    gfc_matrix_rotate(self->modelMat, self->modelMat, self->rotation.z, vector3d(0, 0, 1));
}


void player_jump(Entity* self) {
    self->currentTime = SDL_GetTicks();
    
    if (self->currentTime-self->lastTime < 1500) {
        if (self->currentTime - self->lastTime < 750) {
            self->position.z += 0.05;
        }
        else self->position.z -= 0.05;
    }
    else {
        self->position.z = 0.0;
        self->isJumping = 0;
    }
    //isJumping = 0;
}

Vector3D get_player_position() {
    return player.position;
}

Vector3D set_player_position(Entity* self) {
    player.position = self->position;
}

/*eol@eof*/