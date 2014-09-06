#include "ent.h"

#include "mapParsing.h"
#include "utils.h"
#include "rendering.h"
#include "world.h"

using namespace std;

//------------------------------------------------------------------- MOBILE
void Mob::tryStepTowards(const P& p) {
  if(!hasActed_ && p != p_ && Utils::isPosInMap(p, true) && canStep()) {

    bool blocked[MAP_W][MAP_H];

    for(int y = 0; y < MAP_H; ++y) {
      for(int x = 0; x < MAP_W; ++x) {
        blocked[x][y] = World::rigids[x][y]->isBlocking();
      }
    }

    vector<P> path;

    PathFind::run(p_, p, blocked, path);

    if(!path.empty()) {
      p_ = path.back();
      onStepped();
    }
  }

  hasActed_ = true;
}

//------------------------------------------------------------------- ROCK GROUND
GlyphAndClr RockGround::getGlyphAndClr() const {
  return GlyphAndClr('.', clrBrownXDrk);
}

//------------------------------------------------------------------- ROCK WALL
GlyphAndClr RockWall::getGlyphAndClr() const {
  return GlyphAndClr('^', clrBrownXDrk, clrGray);
}

//------------------------------------------------------------------- ROBOT
GlyphAndClr Rbt::getGlyphAndClr() const {
  Clr clr = pwrCur_ < 1 ? clrRed : (pwrCur_ < (pwrMax_ / 3) ? clrYellow : clrGreen);
  return GlyphAndClr('R', clrBlack, clr);
}

bool Rbt::canStep() const {
  return pwrCur_ > 0;
}

void Rbt::onStepped() {
  if(pwrCur_ > 0) {--pwrCur_;}
}
