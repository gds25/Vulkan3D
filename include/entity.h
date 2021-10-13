#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"

#include "gf3d_model.h"

typedef struct Entity_S {
	Uint8	_inuse;
	Matrix4 modelMat;
	Model* model;
	void (*think)(struct Entity_S* self);
	Uint32	health;
} Entity;

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
void *entity_think(Entity* self); // , Uint32 bufferFrame, VkCommandBuffer commandBuffer);

void* entity_think_all(); // , Uint32 bufferFrame, VkCommandBuffer commandBuffer);

#endif