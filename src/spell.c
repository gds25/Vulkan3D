#include "simple_logger.h"
#include "gfc_types.h"

#include "monster.h"
#include "fireball.h"
#include "spell.h"

void fireball(Entity* self) {
	//Entity* fireball;
	fireball_new(self->position, self->rotation.z);
	//self->mana -= 30;
	self->mana = MAX(0, self->mana-30);
	slog("Casted fireball spell.\nMana after spell: %i", self->mana);
}
void speed_boost(Entity* self) {
	self->velocity.x = 10;
	self->velocity.y = 10;
	//self->mana -= 20;
	self->mana = MAX(0, self->mana-20);
	slog("Added speed boost for 5 seconds.\nMana after spell: %i", self->mana);
}
void health_regen(Entity* self) {
	self->healthRegen = 1;
	//self->mana -= 25;
	self->mana = MAX(0, self->mana-25);
	slog("Health will regenerate for 5 seconds.\nMana after spell: %i", self->mana);
}
void mana_regen(Entity* self) {
	self->manaRegen = 1;
	//self->mana -= 25;
	self->health = MAX(0, self->health-10);
	slog("Mana will regenerate for 5 seconds.\nHealth after spell: %i", self->health);
}
void defense_buff(Entity* self) {
	self->armor = 2;
	//self->mana -= 20;
	self->mana = MAX(0, self->mana-20);
	slog("Added defense, you now take 25 percent less damage for 5 seconds.\nMana after spell: %i", self->mana);
}
/*void heavy_attack(Entity* self) {
	//self->mana -= 30;
	self->mana = MAX(0, self->mana-30);
}*/

/*eol@eof*/