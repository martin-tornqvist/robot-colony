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
