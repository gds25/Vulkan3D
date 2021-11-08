#include "simple_logger.h"
#include "gfc_matrix.h"
#include "entity.h"

typedef struct {
	Entity* entity_list;
	Uint32  entity_count;
	Uint32  current_ent_total;
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
			entity_manager.current_ent_total++;
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
			//slog("Entity (%s) does not have a think function.", i);
			continue;
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
		if (!entity_manager.entity_list[i].update) {
			//slog("Entity (%s) does not have an update function.", i);
			continue;
			//entity_manager.entity_list[i].think;
		}
		entity_update(&entity_manager.entity_list[i]);
	}
}

void model_list_init(Entity* self, Uint32 max, Model* modelList[], char* prefix) {
	//self->modelList_attack = gfc_allocate_array(sizeof(Model*), max);
	if (modelList == NULL) {
		//slog("Model system initialization error: cannot allocate 0 entities.");
		return;
	}
	for (int i = 0; i < max; i++) {
		TextLine modelName;
		snprintf(modelName, GFCLINELEN, prefix, i);
		slog(modelName);
		// Model modelInsert = gf3d_model_load(modelName);
		 //memcpy(&self->modelList_attack[i], gf3d_model_load(modelName), sizeof(Model*));
		modelList[i] = gf3d_model_load(modelName);
		//slog("model: %i", self->modelList_attack[i]);
	}
}

void check_collisions() {
	for (int i = 0; i < entity_manager.current_ent_total; i++) {
		for (int j = 0; j < entity_manager.current_ent_total; j++) {
			if (i == j) continue;
			if (entity_manager.entity_list[i].isPlayer && entity_manager.entity_list[j].isMonster) {
				//slog("here");
				if (entity_manager.entity_list[i].attackFrame > 4 && entity_manager.entity_list[i].attackFrame < 11 && !entity_manager.entity_list[j].attackedThisSwing) {
					//slog("i max AABB x, y, z: %f, %f, %f, min AABB x, y, z: %f, %f, %f", entity_manager.entity_list[i].maxWeaponAABB.x, entity_manager.entity_list[i].maxWeaponAABB.y, entity_manager.entity_list[i].maxWeaponAABB.z, entity_manager.entity_list[i].minWeaponAABB.x, entity_manager.entity_list[i].minWeaponAABB.y, entity_manager.entity_list[i].minWeaponAABB.z);
					//slog("j max AABB x, y, z: %f, %f, %f, min AABB x, y, z: %f, %f, %f", entity_manager.entity_list[j].maxAABB.x, entity_manager.entity_list[j].maxAABB.y, entity_manager.entity_list[j].maxAABB.z, entity_manager.entity_list[j].minAABB.x, entity_manager.entity_list[j].minAABB.y, entity_manager.entity_list[j].minAABB.z);
					if (entity_manager.entity_list[i].maxWeaponAABB.x >= entity_manager.entity_list[j].minAABB.x &&
						entity_manager.entity_list[i].maxWeaponAABB.y >= entity_manager.entity_list[j].minAABB.y &&
						entity_manager.entity_list[i].maxWeaponAABB.z >= entity_manager.entity_list[j].minAABB.z &&
						entity_manager.entity_list[i].minWeaponAABB.x <= entity_manager.entity_list[j].maxAABB.x &&
						entity_manager.entity_list[i].minWeaponAABB.y <= entity_manager.entity_list[j].maxAABB.y &&
						entity_manager.entity_list[i].minWeaponAABB.z <= entity_manager.entity_list[j].maxAABB.z) {
						entity_manager.entity_list[j].attackedThisSwing = 1;
						entity_manager.entity_list[j].health -= 20;
						slog("damaged enemy; enemy health: %i", entity_manager.entity_list[j].health);
					}
				}
				else if (entity_manager.entity_list[i].attackFrame > 10) entity_manager.entity_list[j].attackedThisSwing = 0;
			}
			if (entity_manager.entity_list[i].maxAABB.x >= entity_manager.entity_list[j].minAABB.x &&
				entity_manager.entity_list[i].maxAABB.y >= entity_manager.entity_list[j].minAABB.y &&
				entity_manager.entity_list[i].maxAABB.z >= entity_manager.entity_list[j].minAABB.z &&
				entity_manager.entity_list[i].minAABB.x <= entity_manager.entity_list[j].maxAABB.x &&
				entity_manager.entity_list[i].minAABB.y <= entity_manager.entity_list[j].maxAABB.y &&
				entity_manager.entity_list[i].minAABB.z <= entity_manager.entity_list[j].maxAABB.z)
			{
				//slog("hit entities %i, %i", i, j);
				//slog("i max AABB x, y, z: %f, %f, %f, min AABB x, y, z: %f, %f, %f", entity_manager.entity_list[i].maxAABB.x, entity_manager.entity_list[i].maxAABB.y, entity_manager.entity_list[i].maxAABB.z, entity_manager.entity_list[i].minAABB.x, entity_manager.entity_list[i].minAABB.y, entity_manager.entity_list[i].minAABB.z);
				//slog("j max AABB x, y, z: %f, %f, %f, min AABB x, y, z: %f, %f, %f", entity_manager.entity_list[j].maxAABB.x, entity_manager.entity_list[j].maxAABB.y, entity_manager.entity_list[j].maxAABB.z, entity_manager.entity_list[j].minAABB.x, entity_manager.entity_list[j].minAABB.y, entity_manager.entity_list[j].minAABB.z);

				if (!entity_manager.entity_list[i].isStatic) {
					//entity_manager.entity_list[i].futurePosition.x = entity_manager.entity_list[i].position.x - (1 * cos(entity_manager.entity_list[i].rotation.z));
					//entity_manager.entity_list[i].futurePosition.y = entity_manager.entity_list[i].position.y - (1 * sin(entity_manager.entity_list[i].rotation.z));
					//entity_manager.entity_list[i].futurePosition.z = entity_manager.entity_list[i].position.z;
				}

				//entity_manager.entity_list[i].position.x -= entity_manager.entity_list[i].velocity.x * (0.10 * sin(entity_manager.entity_list[i].rotation.z));
				//entity_manager.entity_list[i].position.y += entity_manager.entity_list[i].velocity.y * (0.10 * cos(entity_manager.entity_list[i].rotation.z));
				//entity_manager.entity_list[i].position.z;
			}
			else {
				entity_manager.entity_list[i].position.x = entity_manager.entity_list[i].futurePosition.x;
				entity_manager.entity_list[i].position.y = entity_manager.entity_list[i].futurePosition.y;
				entity_manager.entity_list[i].position.z = entity_manager.entity_list[i].futurePosition.z;
			}
		}
	}
}