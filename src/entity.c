#include "simple_logger.h"
#include "gfc_matrix.h"
#include "entity.h"

typedef struct {
	Entity* entity_list;
	Uint32  entity_count;
}EntityManager;

static EntityManager entity_manager = { 0 };

void entity_system_close() {
	entity_free_all();
	free(entity_manager.entity_list);
	memset(&entity_manager, 0, sizeof(EntityManager));
	slog("Entity system closed.");
}

void entity_system_init(Uint32 max) {
	entity_manager.entity_list = gfc_allocate_array(sizeof(Entity), max);
	if (entity_manager.entity_list == NULL) {
		slog("Entity system initialization error: cannot allocate 0 entities.");
		return;
	}
	entity_manager.entity_count = max;
	slog("----- ------ ------Entity system initialized.");
	atexit(entity_system_close);
}

Entity* entity_new() {
	for (int i = 0; i < entity_manager.entity_count; i++) {
		if (!entity_manager.entity_list[i]._inuse) {
			entity_manager.entity_list[i]._inuse = 1;
			gfc_matrix_identity(entity_manager.entity_list[i].modelMat);
			return &entity_manager.entity_list[i];
		}
	}
	slog("Entity creation error: no free space in entity list.");
	return NULL;
}

/**
* @brief free previously created entity from memory
* @param self the entity to be freed
* @param bufferFrame the current rending frame index
*/
void entity_free(Entity* self) { // , Uint32 bufferFrame) {
	if (!self)
		return;
	memset(self, 0, sizeof(Entity));
	gf3d_model_free(self->model);
}

void entity_free_all() {
	for (int i = 0; i < entity_manager.entity_count; i++) {
		entity_free(&entity_manager.entity_list[i]);
	}
}

void entity_draw(Entity* self, Uint32 bufferFrame, VkCommandBuffer commandBuffer) {
	if (!self)
		return;
	//slog("model: %i", self->model);
	gf3d_model_draw(self->model, bufferFrame, commandBuffer, self->modelMat);
}

void entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer) {
	for (int i = 0; i < entity_manager.entity_count; i++) {
		if (!entity_manager.entity_list[i]._inuse)
			continue;
		entity_draw(&entity_manager.entity_list[i], bufferFrame, commandBuffer);
	}
}

void entity_think(Entity* self) {
	if (!self)return;
	if (self->think)self->think(self);
} // , Uint32 bufferFrame, VkCommandBuffer commandBuffer);

void entity_think_all() {
	for (int i = 0; i < entity_manager.entity_count; i++) {
		if (!entity_manager.entity_list[i]._inuse)
			continue;
		if (!entity_manager.entity_list[i].think) {
			slog("Entity (%s) does not have a think function.", i);
			return;
			//entity_manager.entity_list[i].think;
		}
		//entity_think;
		entity_think(&entity_manager.entity_list[i]);
	}
}

void entity_update(Entity* self) {
	if (!self)return;
	if (self->update)self->update(self);
} // {
	//if (!self)return;
	// HANDLE ALL COMMON UPDATE STUFF

	//vector3d_add(self->position, self->position, self->velocity);
	//vector3d_add(self->velocity, self->acceleration, self->velocity);

	//gfc_matrix_identity(self->modelMat);

    /*gfc_matrix_scale(self->modelMat, self->scale);

	gfc_matrix_rotate(self->modelMat, self->modelMat, self->rotation.z, vector3d(0, 0, 1));
	gfc_matrix_rotate(self->modelMat, self->modelMat, self->rotation.y, vector3d(0, 1, 0));
	gfc_matrix_rotate(self->modelMat, self->modelMat, self->rotation.x, vector3d(1, 0, 0));

	gfc_matrix_translate(self->modelMat, self->position);*/

	//if (self->update)self->update(self);
//}
// vector3d_add(position,position,velocity)s
void entity_update_all() {
	int i;
	for (i = 0; i < entity_manager.entity_count; i++)
	{
		if (!entity_manager.entity_list[i]._inuse)// not used yet
		{
			continue;// skip this iteration of the loop
		}
		//entity_update;
		entity_update(&entity_manager.entity_list[i]);
	}
}