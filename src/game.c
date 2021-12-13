#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "gfc_audio.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_sprite.h"

#include "entity.h"
#include "agumon.h"
#include "player.h"
#include "monster.h"
#include "angrymonster.h"
#include "boss.h"
#include "fireball.h"
#include "cube.h"
#include "wall.h"
#include "world.h"

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 0;
    const Uint8 * keys;
    Model *model;
    Matrix4 modelMat;
    Model *model2;
    Matrix4 modelMat2;

    Sprite* mouse = NULL;
    int mousex, mousey;
    float mouseFrame = 0;

    Sprite* mainMenu = NULL;
    Sprite* healthbar = NULL;
    Sprite* manabar = NULL; 
    Sprite* healthbg = NULL;
    Sprite* manabg = NULL;
    Sprite* deathScreen = NULL;
    World* w;

    Entity* player;

    Sound* menuTheme = NULL;
    Sound* deathSound = NULL;
    
    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }
    
    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
	slog_sync();

    // --- initialize entity system
    entity_system_init(1024);

    // --- initialize sound system
    gfc_audio_init(
        1024,
        2,
        2,
        1024,
        1,
        1);

    // main game loop
    slog("gf3d main loop begin");

	slog_sync();

    mouse = gf3d_sprite_load("images/pointer.png", 32, 32, 16);
    mainMenu = gf3d_sprite_load("images/mainmenu.png", -1, -1, 1);
    healthbar = gf3d_sprite_load("images/healthbar.png", -1, -1, 1);
    manabar = gf3d_sprite_load("images/manabar.png", -1, -1, 1);
    healthbg = gf3d_sprite_load("images/healthbg.png", -1, -1, 1);
    manabg = gf3d_sprite_load("images/manabg.png", -1, -1, 1);
    deathScreen = gf3d_sprite_load("images/deathscreen.png", -1, -1, 1);

    menuTheme = gfc_sound_load("sounds/menuTheme.wav", 1.0, 1);
    deathSound = gfc_sound_load("sounds/deathSound.wav", 1.0, 1);
    //gf3d_sprite_draw(mouse, vector2d(100, 100), vector2d(1, 1), (Uint32)mouseFrame);
    //gf3d_sprite_draw(mainMenu, vector2d(0, 0), vector2d(1, 1), 0);

    w = world_load("config/world.json");

    //gfc_sound_play(menuTheme, -1, 1.0, -1, -1);
    //Entity *agumon = agumon_new();

	/*
    model = gf3d_model_load("dino");
	gfc_matrix_identity(modelMat);
	model2 = gf3d_model_load("dino");
    gfc_matrix_identity(modelMat2);
    gfc_matrix_make_translation(
            modelMat2,
            vector3d(10,0,0)
        );
    */

    gf3d_camera_set_scale(vector3d(1, 1, 1));

    
  //  wall_new(vector3d(-100, 0, 0), 0);
  //  wall_new(vector3d(100, 0, 0), M_PI);

    //ireball_new(vector3d(-110, -110, 0), 0);

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

        SDL_GetMouseState(&mousex, &mousey);

        mouseFrame += 0.01;
        if (mouseFrame >= 16)mouseFrame = 0;

        //update game things here
       
        /*
        gf3d_vgraphics_rotate_camera(0.001);
        gfc_matrix_rotate(
            modelMat,
            modelMat,
            0.002,
            vector3d(1,0,0));
        gfc_matrix_rotate(
            modelMat2,
            modelMat2,
            0.002,
            vector3d(0,0,1));
        */

        entity_think_all();
        check_collisions();
        entity_update_all();
        

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        //slog("before camera");
        //gf3d_camera_update_view();
        gf3d_camera_get_view_mat4(gf3d_vgraphics_get_view_matrix());

            gf3d_vgraphics_render_start();
            //slog("before world draw");
            if (!player->isPaused) {
                world_draw(w);

                //slog("before draw entites");
            // entity_draw_all would go here instead of model_draw //parameters bufferframe, commandbuffer
                entity_draw_all();
                

                
                //gf3d_sprite_draw(healthbg, vector2d(900, 75), vector2d(1, 1), 0); 
                //gf3d_sprite_draw(healthbg, vector2d(900, 75), vector2d(1, 1), 0); 
                gf3d_sprite_draw(manabg, vector2d(900, 125), vector2d(1, 1), 0);
                gf3d_sprite_draw(healthbg, vector2d(900, 75), vector2d(1, 1), 0);
                gf3d_sprite_draw(healthbar, vector2d(1200 - (3*player->health), 75), vector2d(1, 1), 0);
                gf3d_sprite_draw(manabar, vector2d(1200 - (3*player->mana), 125), vector2d(1, 1), 0);

                if (player->isDead) {
                    gf3d_sprite_draw(deathScreen, vector2d(0, 0), vector2d(1, 1), 0);
                    if (player->attackFrame == 0) gfc_sound_play(deathSound, 0, 1.0, -1, -1);
                }
            }
                
                //gf3d_model_draw(model,bufferFrame,commandBuffer,modelMat);
                //gf3d_model_draw(model2,bufferFrame,commandBuffer,modelMat2);
                //slog("after draw entities");
            else {
                gf3d_sprite_draw(mainMenu, vector2d(0, 0), vector2d(1, 1), 0);
            }
                gf3d_sprite_draw(mouse, vector2d(mousex, mousey), vector2d(1, 1), (Uint32)mouseFrame);
        gf3d_vgraphics_render_end();

        if (player->isPaused & SDL_GetMouseState(&mousex, &mousey) & SDL_BUTTON_LMASK != 0
            & mousex >= 315 & mousex <= 871 & mousey >= 269 & mousey <= 575) {

            if (mousey <= 395) {
                player->isPaused = 0;
            }
            else if (mousey >= 450) {
                done = 1;
            }
            slog("mouse click: x = %i, y = %i", mousex, mousey);
        }

        //if (player->health <= 0) gfc_sound_play(deathSound, 0, 1.0, -1, -1);
        if ((player->isPaused & keys[SDL_SCANCODE_9]) || (player->isDead && SDL_GetMouseState(&mousex, &mousey) && SDL_BUTTON_LMASK != 0
            & mousex >= 315 & mousex <= 871 && mousey >= 269 && mousey <= 395)) {
            gfc_sound_play(deathSound, 0, 1.0, -1, -1);
            //world_delete(w);
            entity_system_close();
            gf3d_model_manager_close();

            gf3d_model_manager_init(1024, gf3d_swapchain_get_swap_image_count(), gf3d_vgraphics_get_default_logical_device());
            w = world_load("config/world.json");
            entity_system_init(1024);
            player = player_new(vector3d(250, 250, 0), "config/player.json");

            for (int i = 5; i < 300; i = i + 150) {
                monster_new(vector3d(i, i, 0), "config/monster.json");
                angry_monster_new(vector3d(i + 50, i + 50, 0), "config/angrymonster.json");
                monster_new(vector3d(i - 300, i - 300, 0), "config/monster.json");
                angry_monster_new(vector3d(i - 250, i - 250, 0), "config/angrymonster.json");
                monster_new(vector3d(i - 300, i, 0), "config/monster.json");
                angry_monster_new(vector3d(i - 250, i, 0), "config/angrymonster.json");
            }

            boss_new(vector3d(200, -200, 0), "config/boss.json");

        }
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    

    world_delete(w);
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
} 

/*eol@eof*/
