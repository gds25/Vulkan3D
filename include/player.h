#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

/**
 * @brief Create a new player entity
 * @param position where to spawn the player at
 * @param filename JSON file that stores player info
 * @return NULL on error, or an player entity pointer on success
 */
Entity* player_new(Vector3D position, char* filename);

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
* @brief animate player rolling to the left or right
* @param self the player entity
*/
void player_roll(Entity* self);

/**
* @brief animate player on death
* @param self the player entity
*/
void player_on_death(Entity* self);

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
void player_set_aabb(Entity* self);

/**
 * @brief get bounding box cooridnates of player weapon
 * @param self the player entity
 */
void sword_set_aabb(Entity* self);

/**
 * @brief check to see if game is paused
 * @param self the player entity
 */
Uint8 player_get_pause_state(Entity* self);

/**
* @brief set pause state to see whether to draw main menu
* @param self the player entity
*/
void player_set_pause_state(Entity* self);

#endif