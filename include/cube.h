#ifndef __CUBE_H__
#define __CUBE_H__

#include "entity.h"

/**
 * @brief Create a new block entity to populate the world
 * @param position where to spawn the block at 
 * @param rotation direction cube is facing
 * @return NULL on error, or a block entity pointer on success
 */
Entity* cube_new(Vector3D position, float rotation);

/**
* @brief get block's min and max axis aligned bounding box points
* @param self the entity in question
*/
void cube_get_aabb(Entity* self);

#endif