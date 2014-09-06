#include "world.h"

#include "ent.h"

namespace World {

std::vector<Mob*> mobs;

Ground* ground[MAP_W][MAP_H];

void init() {
  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      ground[x][y] = nullptr;
    }
  }

  cleanup();

  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      ground[x][y] = new RockGround();
    }
  }
}

void cleanup() {
  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      delete ground[x][y];
      ground[x][y] = nullptr;
    }
  }

  for(Mob* mob : mobs) {delete mob;}
  mobs.resize(0);
}

}
