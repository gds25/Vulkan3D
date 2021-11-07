#ifndef __SPELL_H__
#define __SPELL_H__

#include "entity.h"

/**
 * @brief Create a new player entity
 * @param position where to spawn the player at
 * @return NULL on error, or an player entity pointer on success
 */

void fireball(Entity* self);
void speed_boost(Entity* self);
void health_regen(Entity* self);
void defense_buff(Entity* self);
void heavy_attack(Entity* self);


#endif