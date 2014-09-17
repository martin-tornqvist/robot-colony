#include "world.h"

#include "ent.h"

namespace World {

namespace {

int tickNr_ = 0;

} //Namespace

std::vector<Mob*> mobs;

Rigid* rigids[MAP_W][MAP_H];

void init() {
  tickNr_ = 0;

  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      rigids[x][y] = nullptr;
    }
  }

  cleanup();

  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      rigids[x][y] = new RockGround();
    }
  }
}

void cleanup() {
  tickNr_ = 0;

  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      delete rigids[x][y];
      rigids[x][y] = nullptr;
    }
  }

  for(Mob* mob : mobs) {delete mob;}
  mobs.resize(0);
}

int getTickNr() {
  return tickNr_;
}

void incrTickNr() {
  ++tickNr_;
}

Rigid* replaceRigid(Rigid* const newRigid, const P& p) {
  assert(newRigid && "New rigid is nullptr");
  delete rigids[p.x][p.y];
  rigids[p.x][p.y] = newRigid;
  return newRigid;
}

}
