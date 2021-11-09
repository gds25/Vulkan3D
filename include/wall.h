#ifndef __WALL_H__
#define __WALL_H__

#include "entity.h"

// NOT USED


/**
 * @brief Create a new block entity to populate the world
 * @param position where to spawn the block at
 * @param rotation direction cube is facing
 * @return NULL on error, or a block entity pointer on success
 */
Entity* wall_new(Vector3D position, float rotation);

/**
* @brief get block's min and max axis aligned bounding box points
* @param self the entity in question
*/
void wall_get_aabb(Entity* self);

#endif