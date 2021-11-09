#ifndef __HEALTH_H__
#define __HEALTH_H__

#include "entity.h"

/**
 * @brief Create a new health_pack entity
 * @param position where to spawn the player at
 * @return NULL on error, or a health_pack entity pointer on success
 */
Entity* health_new(Vector3D position);

/**
 * @brief get bounding box cooridnates of health_pack entity
 * @param the entity in question
 */
void health_get_aabb(Entity* self);

#endif