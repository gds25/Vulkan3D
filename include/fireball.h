#ifndef __FIREBALL_H__
#define __FIREBALL_H__

#include "entity.h"

/**
 * @brief Create a new fireball entity
 * @param position where to spawn the fireball at (most likely at current player position)
 * @param rotation direction in which the fireball will travel (radian angle around origin)
 * @return NULL on error, or a fireball entity pointer on success
 */
Entity* fireball_new(Vector3D position, float rotation);

/**
* @brief fireball logic
* @param self the entity in question
*/
void fireball_think(Entity* self);

/**
* @brief update fireball in world space
* @param self the entity in question
*/
void fireball_update(Entity* self);

/**
 * @brief get bounding box cooridnates of fireball entity
 * @param the entity in question
 */

void fireball_get_aabb(Entity* self);


#endif