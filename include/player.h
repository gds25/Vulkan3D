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
void player_update(Entity* self);

void player_jump(Entity* self);
void player_attack(Entity* self);

Vector3D get_player_position();
Vector3D set_player_position(Entity* self);

#endif