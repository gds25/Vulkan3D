#ifndef __SPELL_H__
#define __SPELL_H__

#include "entity.h"

/**
 * @brief cast fireball spell
 * @param self the entity casting the spell (player)
 */
void fireball(Entity* self);

/**
 * @brief add speedboost to player
 * @param self the entity casting the spell (player)
 */
void speed_boost(Entity* self);

/**
 * @brief use mana to regen health
 * @param self the entity casting the spell (player)
 */
void health_regen(Entity* self);

/**
 * @brief use health to regen mana
 * @param self the entity casting the spell (player)
 */
void mana_regen(Entity* self);

/**
 * @brief increase player armor/defense
 * @param self the entity casting the spell (player)
 */
void defense_buff(Entity* self);


//void heavy_attack(Entity* self);


#endif