#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "entity.h"

/**
 * @brief Create a new monster entity
 * @param position where to spawn the monster at
 * @param filename JSON file that stores world info
 * @return NULL on error, or an monster entity pointer on success
 */
Entity* monster_new(Vector3D position, char* filename);


/**
* @brief monster entity logic
* @param self the entity in question
*/
void monster_think(Entity* self);

/**
* @brief update monster in the world
* @param self the entity in question
*/
void monster_update(Entity* self);

/**
* @brief monster should walk toward player entity "chasing" after them 
* @param self the entity in question
* @param playerPos the position of the player
*/
void monster_chase(Entity* self, Vector3D playerPos);

/**
* @brief monster should walk around aimlessly **TODO** switch rand() for a different random number generator
* @param self the entity in question
*/
void monster_pace(Entity* self);

/**
* @brief change attack animation frame and calculate weapon collision points
* @param self the entity in question
*/
void monster_attack(Entity* self);

/**
 * @brief get bounding box cooridnates of monster entity
 * @param the entity in question
 */
void monster_get_aabb(Entity* self);

/**
 * @brief get bounding box cooridnates of swipe/punch attack
 * @param the entity in question
 */
void attack_get_aabb(Entity* self);

/**
 * @brief free entity on death and drop a health_pack entity
 * @param the entity in question
 */
void monster_die(Entity* self);

#endif
