#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "cmnData.h"

class Mob;
class Ground;

namespace World {

extern std::vector<Mob*> mobs;

extern Ground* ground[MAP_W][MAP_H];

void init();

void cleanup();

}

#endif
