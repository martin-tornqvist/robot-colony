#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "cmnData.h"

class Mob;
class Rigid;
struct P;

namespace World {

extern std::vector<Mob*> mobs;

extern Rigid* rigids[MAP_W][MAP_H];

void init();

void cleanup();

Rigid* replaceRigid(Rigid* const newRigid, const P& p);

}

#endif
