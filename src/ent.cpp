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

//------------------------------------------------------------------- ROBOT
GlyphAndClr Rbt::getGlyphAndClr() const {
  Clr clr = pwrCur_ < 1 ? clrRed : (pwrCur_ < (pwrMax_ / 2) ? clrYellow : clrGreen);
  return GlyphAndClr('R', clrBlack, clr);
}

void Rbt::onTick() {
  auto* rigidHere = World::rigids[p_.x][p_.y];
  if(rigidHere->getEntType() == EntType::rechargeStation) {
    if(pwrCur_ < pwrMax_) {++pwrCur_;}
  }
}

bool Rbt::canStep() const {
  return pwrCur_ > 0;
}

void Rbt::onStepped() {
  --pwrCur_;
}

void Rbt::tryBuild(const P& p) {
  if(!hasActed_ && World::rigids[p.x][p.y]->getEntType() != EntType::rechargeStation) {
    if(Utils::isPosAdj(p_, p, true)) {
      World::replaceRigid(new RechargeStation, p);
      hasActed_ = true;
      --pwrCur_;
    } else {
      tryStepTowards(p);
    }
  }
}
