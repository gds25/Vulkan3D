#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"

#include "entity.h"
#include "agumon.h"
#include "player.h"
#include "monster.h"
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
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
    Model *model;
    Matrix4 modelMat;
    Model *model2;
    Matrix4 modelMat2;

    World* w;
    
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

    // main game loop
    slog("gf3d main loop begin");

	slog_sync();

    w = world_load("config/world.json");
    
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

    player_new(vector3d(0, 0, 0));

    monster_new(vector3d(50, 50, 0));

    monster_new(vector3d(-50, 50, 0));

    cube_new(vector3d(100, 0, 0), 0);
    //wall_new(vector3d(-100, 0, 0), 0);
    //wall_new(vector3d(0, 100, 0), M_PI/2);
    //wall_new(vector3d(0, -100, 0), M_PI/2);

    monster_new(vector3d(50, -50, 0));

    monster_new(vector3d(-50, -50, 0));

    //ireball_new(vector3d(-110, -110, 0), 0);

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
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

        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);

            //slog("before world draw");
                world_draw(w, bufferFrame, commandBuffer);

                //slog("before draw entites");
            // entity_draw_all would go here instead of model_draw //parameters bufferframe, commandbuffer
                entity_draw_all(bufferFrame, commandBuffer);
                
                //gf3d_model_draw(model,bufferFrame,commandBuffer,modelMat);
                //gf3d_model_draw(model2,bufferFrame,commandBuffer,modelMat2);
                //slog("after draw entities");
            gf3d_command_rendering_end(commandBuffer);
            
        gf3d_vgraphics_render_end(bufferFrame);

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
