#include "time.h"

#include "cmnData.h"
#include "ent.h"
#include "world.h"

using namespace std;

namespace {
int tickNr_ = 0;
}

namespace Time {

void tick() {
  ++tickNr_;

  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      World::rigids[x][y]->onTick();
    }
  }

  //Allow mobs to act again
  for(Mob* mob : World::mobs) {
    mob->hasActed_ = false;
    mob->onTick();
  }
}

int getTickNr() {
  return tickNr_;
}

} //Time
