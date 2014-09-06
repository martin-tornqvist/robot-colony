#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "ent.h"

namespace World {

extern std::vector<Mob*> mobs;

void init();

void cleanup();

}

#endif
