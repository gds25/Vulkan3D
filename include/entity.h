#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"

#include "gf3d_model.h"

typedef struct Entity_S {
	Uint8	_inuse;
	Matrix4 modelMat;

	Uint8 attackFrame; //current attack animation frame to get corresponding model
	Uint8 framesMax; //total number of  frames for a particular animation 

	Model* idleModel[3]; //to be populated with player models for when not attacking

	Model* model; //current entity model used
	Model* modelList_attack[20]; //to be populated with attack animation models
	Model* modelList_attack2[20];
	Model* modelList_attack3[20];

	Uint8 character; //for use in player file, specifies the weapon you are using

	void (*think)(struct Entity_S* self);
	void (*update)(struct Entity_S* self);
	Uint32	health; //entity stats
	Uint32  healthRegen;
	Uint32	mana;
	Uint32  manaRegen;
	Uint32  armor;
	Uint32  damage;

	Vector3D position; //vectors dealing with position and movement
	Vector3D futurePosition;
	Vector3D cameraPosition;
	Vector3D velocity;
	Vector3D acceleration;

	Uint8 isJumping; //entity states
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

	Vector3D maxAABB; //entity bounding box coordinates
	Vector3D minAABB;

	Vector3D maxWeaponAABB; //bounding box coordinates for weapons
	Vector3D minWeaponAABB;

	Uint8 isStatic;
	Uint8 isPlayer; //is entity the player ent
	Uint8 isMonster; //is entity a monster ent

	Uint8 attackedThisSwing; //check if monster was hit on player attack animation
	Uint8 hasAttacked; //check if monster hit player on attack anumation
	Uint8 isFireball; //check if entity is a fireball
	Uint8 isHealth; //check if entity is a health pack

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
void entity_free(Entity* self);

/**
* @brief free all previously created entities from memory
*/
void entity_free_all();

/**
* @brief draw entity model to the screen
* @param self the entity to be drawn
* @param bufferFrame the current rending frame index
*/
void entity_draw(Entity* self);


/**
* @brief draw all entity models to screen
* @param bufferFrame the current rending frame index
*/
void entity_draw_all();

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