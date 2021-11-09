#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

/**
 * @brief Create a new player entity
 * @param position where to spawn the player at
 * @return NULL on error, or an player entity pointer on success
 */
Entity* player_new(Vector3D position);

/**
* @brief player logic
* @param self the player entity
*/
void player_think(Entity* self);

/**
* @brief update entity in the world
* @param self the player entity
*/
void player_update(Entity* self);

/**
* @brief translate player position on z-axis
* @param self the player entity
*/
void player_jump(Entity* self);

/**
* @brief change attack animation frame and calculate weapon collision points
* @param self the player entity
*/
void player_attack(Entity* self);

/**
* @brief get player position
* @return player entity position
*/
Vector3D get_player_position();

/**
* @brief set player position to use in monster.c
* @param self the player entity
*/
void set_player_position(Entity* self);

/**
 * @brief get bounding box cooridnates of okayer entity
 * @param self the player entity
 */
void player_get_aabb(Entity* self);

/**
 * @brief get bounding box cooridnates of player weapon
 * @param self the player entity
 */
void sword_get_aabb(Entity* self);

#endif