#ifndef __WORLD_H__
#define __WORLD_H__


#include "gfc_types.h"
#include "gfc_list.h"
#include "gfc_matrix.h"
#include "gf3d_vgraphics.h"
#include "gf3d_model.h"
#include "entity.h"

typedef struct
{
    Matrix4 modelMat;
    Model* worldModel;
    List* spawnList;        //entities to spawn
    List* entityList;       //entities that exist in the world
}World;

/**
 * @brief create the game world
 * @param filename JSON file that stores world info
 * @return NULL on error, or world entity on success
 */
World* world_load(char* filename);

/**
* @brief draw world model to the screen
* @param world the world to be drawn
* @param bufferFrame the current rending frame index
*/
void world_draw(World* world, Uint32 bufferFrame, VkCommandBuffer commandBuffer);

/**
* @brief free world model from memory
* @param world the world to be freed
*/
void world_delete(World* world);

#endif