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

  auto&       mobs  = World::mobs;
  auto* const rbt   = mobs[0];

  rbt->onTick();                                                //Tick robot
  while(!rbt->hasActed_) {}                                     //Wait for robot to act
  for(size_t i = 1; i < mobs.size(); ++i) {mobs[i]->onTick();}  //Tick other mobs
}

int getTickNr() {
  return tickNr_;
}

} //Time
