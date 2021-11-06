#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "entity.h"

/**
 * @brief Create a new player entity
 * @param position where to spawn the player at
 * @return NULL on error, or an player entity pointer on success
 */
Entity* monster_new(Vector3D position);
void monster_think(Entity* self);
void monster_update(Entity* self);
void monster_chase(Entity* self, Vector3D playerPos);
void monster_pace(Entity* self);
void monster_attack();

#endif
