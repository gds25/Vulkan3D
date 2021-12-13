#ifndef __ANGRYMONSTER_H__
#define __ANGRYMONSTER_H__

#include "entity.h"
#include "monster.h"

/**
 * @brief Create a new monster entity
 * @param position where to spawn the monster at
 * @param filename JSON file that stores world info
 * @return NULL on error, or an monster entity pointer on success
 */
Entity* angry_monster_new(Vector3D position, char* filename);

/**
* @brief monster entity logic
* @param self the entity in question
*/
void angry_monster_think(Entity* self);

/**
* @brief update monster in the world
* @param self the entity in question
*/
void angry_monster_update(Entity* self);

/**
* @brief change attack animation frame and calculate weapon collision points
* @param self the entity in question
*/
void angry_monster_attack(Entity* self);

/**
 * @brief get bounding box cooridnates of monster entity
 * @param the entity in question
 */
void angry_monster_get_aabb(Entity* self);

/**
 * @brief get bounding box cooridnates of swipe/punch attack
 * @param the entity in question
 */
void jump_attack_get_aabb(Entity* self);

#endif
