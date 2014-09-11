#include "ent.h"

#include "mapParsing.h"
#include "utils.h"
#include "rendering.h"
#include "world.h"
#include "time.h"

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

//------------------------------------------------------------------- ASSEMBLY
void Asm::tickBuild() {
  if(ticksBuilt_ < getNrTicksToBuild()) {
    ++ticksBuilt_;
    isBeingBuiltThisTick_ = true;
  }
}

GlyphAndClr Asm::getGlyphAndClr() const {
  const auto def = getGlyphAndClr_();
  if(isFinished()) {
    return def;
  } else {
    //"Animation"
    if(!isBeingBuiltThisTick_ || Time::getTickNr() % 2) {
      return GlyphAndClr('%', def.clr, def.clrBg);
    } else {
      return GlyphAndClr('%', def.clrBg, def.clr);
    }
  }
}

//------------------------------------------------------------------- ROBOT
GlyphAndClr Rbt::getGlyphAndClr() const {
  Clr clr = pwrCur_ < 1 ? clrRed : (pwrCur_ < (pwrMax_ / 2) ? clrYellow : clrGreen);
  return GlyphAndClr('R', clrBlack, clr);
}

void Rbt::onTick() {
  auto* rigidHere = World::rigids[p_.x][p_.y];
  if(rigidHere->getEntType() == EntType::assembly) {
    if(static_cast<Asm*>(rigidHere)->isFinished()) {
      if(pwrCur_ < pwrMax_) {++pwrCur_;}
    }
  }
}

bool Rbt::canStep() const {
  return pwrCur_ > 0;
}

void Rbt::onStepped() {
  --pwrCur_;
}

void Rbt::tryBuild(const P& p) {
  auto* const rigidHere   = World::rigids[p.x][p.y];
  const auto  entTypeHere = rigidHere->getEntType();
  const bool  IS_ADJ      = Utils::isPosAdj(p_, p, true);

  if(entTypeHere == EntType::assembly) {
    auto* const assembly = static_cast<Asm*>(rigidHere);
    if(!assembly->isFinished()) {
      if(IS_ADJ) {
        hasActed_ = true;
        --pwrCur_;
        assembly->tickBuild();
      } else {
        tryStepTowards(p);
      }
    }
  } else {
    if(IS_ADJ) {
      hasActed_ = true;
      --pwrCur_;
      auto* const newRigid = World::replaceRigid(new RechargeStation, p);
      auto* const assembly = static_cast<Asm*>(newRigid);
      assembly->tickBuild();
    } else {
      tryStepTowards(p);
    }
  }
}
