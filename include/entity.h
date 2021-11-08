#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"

#include "gf3d_model.h"

typedef struct Entity_S {
	Uint8	_inuse;
	Matrix4 modelMat;
	Uint8 attackFrame;
	Model* idleModel;
	Model* model;
	Model* modelList_attack[100];
	void (*think)(struct Entity_S* self);
	void (*update)(struct Entity_S* self);
	Uint32	health;
	Uint32  healthRegen;
	Uint32	mana;
	Uint32  armor;
	Uint32  damage;

	Vector3D position;
	Vector3D futurePosition;
	Vector3D cameraPosition;
	Vector3D velocity;
	Vector3D acceleration;

	Uint8 isJumping;
	Uint8 isAttacking;
	Uint8 isIdle;
	Uint8 poweredUp;

	Uint32 currentTime;
	Uint32 lastTime;

	Uint32 jumpTime;
	Uint32 lastJumpTime;

	Uint32 powerUpTime;
	Uint32 lastPowerUpTime;

	Vector3D scale;
	Vector3D rotation;

	Vector3D maxAABB;
	Vector3D minAABB;

	Vector3D maxWeaponAABB;
	Vector3D minWeaponAABB;

	Uint8 isStatic;
	Uint8 isPlayer;
	Uint8 isMonster;
	Uint8 attackedThisSwing;

} Entity;

/**
* @brief entity collision checker
*/
void check_collisions();

/**
* @brief populate a list of models for keyframed animation
* @param self entity to store models for
* @param max maximum amount of models in the list
* @param modelList[] list to populate
* @param prefix name of model files, all should have the same name with a number at the end
*/
void model_list_init(Entity* self, Uint32 max, Model* modelList[], char* prefix);

/**
* @brief initialize entity subsystem
* @param max maximum amount of entities that can exist at once
*/
void entity_system_init(Uint32 max);

/**
* @brief create new entity
*/
Entity* entity_new();

/**
* @brief free previously created entity from memory
* @param self the entity to be freed
*/
void entity_free(Entity* self); // , Uint32 bufferFrame);

/**
* @brief free all previously created entities from memory
*/
void entity_free_all();

/**
* @brief free previously created entity from memory
* @param self the entity to be drawn
* @param bufferFrame the current rending frame index
*/
void entity_draw(Entity* self, Uint32 bufferFrame, VkCommandBuffer commandBuffer);


/**
* @brief free previously created entity from memory
* @param self the entity to be freed
* @param bufferFrame the current rending frame index
*/
void entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer);

/**
* @brief entity logic
* @param self the entity in question
*/
void entity_think(Entity* self); // , Uint32 bufferFrame, VkCommandBuffer commandBuffer);

/**
* @brief loop through think function of every entity
*/
void entity_think_all(); 

/**
* @brief update entity in the world
* @param self the entity in question
*/
void entity_update(Entity* self);

/**
* @brief loop through update function of every entity
*/
void entity_update_all();

#endif