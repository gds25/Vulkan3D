#ifndef __AGUMON_H__
#define __AGUMON_H__

#include "gfc_types.h"

#include "gf3d_model.h"
#include "entity.h"

Entity* agumon_new();

void agumon_think(Entity* self);

#endif