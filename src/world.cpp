#include "world.h"

namespace World {

std::vector<Mob*> mobs;

void init() {
  cleanup();
}

void cleanup() {
  for(Mob* mob : mobs) {delete mob;}
  mobs.resize(0);
}

}
