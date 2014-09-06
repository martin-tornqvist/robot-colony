#include "time.h"

#include "cmnData.h"
#include "ent.h"
#include "world.h"

using namespace std;

namespace Time {

void tick() {
  //Allow mobs to act again
  for(Mob* mob : World::mobs) {mob->hasActed_ = false;}
}

} //Time
