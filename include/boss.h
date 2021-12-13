#ifndef __BOSS_H__
#define __BOSS_H__

#include "entity.h"
#include "monster.h"

/**
 * @brief Create a new boss entity
 * @param position where to spawn the boss at
 * @param filename JSON file that stores boss info
 * @return NULL on error, or an entity pointer on success
 */
Entity* boss_new(Vector3D position, char* filename);

/**
* @brief monster entity logic
* @param self the entity in question
*/
void boss_think(Entity* self);

/**
* @brief update monster in the world
* @param self the entity in question
*/
void boss_update(Entity* self);

/**
* @brief change attack animation frame and calculate weapon collision points
* @param self the entity in question
*/
void boss_walk(Entity* self);

/**
* @brief change attack animation frame and calculate weapon collision points
* @param self the entity in question
*/
void boss_stomp(Entity* self);

/**
* @brief change attack animation frame and calculate weapon collision points
* @param self the entity in question
*/
void boss_crawl(Entity* self);

/**
 * @brief get bounding box cooridnates of monster entity
 * @param the entity in question
 */
void boss_get_aabb(Entity* self);

/**
 * @brief get bounding box cooridnates of swipe/punch attack
 * @param the entity in question
 */
void boss_attack_get_aabb(Entity* self);

#endif
