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
  Clr clr = energyCur_ < 1 ? clrRed :
            (energyCur_ < (energyMax_ / 2) ? clrYellow : clrGreen);
  return GlyphAndClr('R', clrBlack, clr);
}

void Rbt::onTick() {
  auto* rigidHere = World::rigids[p_.x][p_.y];
  if(rigidHere->getEntType() == EntType::assembly) {
    const auto* const assemblyHere     = static_cast<const Asm*>(rigidHere);
    const auto        assemblyTypeHere = assemblyHere->getAsmType();
    if(assemblyTypeHere == AsmType::rechargeStation && assemblyHere->isFinished()) {
      if(energyCur_ < energyMax_) {energyCur_ += 20;}
    }
  }
}

bool Rbt::canStep() const {
  return energyCur_ > 0;
}

void Rbt::onStepped() {
  energyCur_ -= 20;
}

void Rbt::tryBuild(const AsmType assemblyType, const P& p) {
  if(!hasActed_) {
    auto* const rigidHere   = World::rigids[p.x][p.y];
    const auto  entTypeHere = rigidHere->getEntType();
    const bool  IS_ADJ      = Utils::isPosAdj(p_, p, true);

    if(entTypeHere == EntType::assembly) {
      auto* const assemblyHere     = static_cast<Asm*>(rigidHere);
      const auto  assemblyTypeHere = assemblyHere->getAsmType();
      if(assemblyTypeHere != assemblyType || !assemblyHere->isFinished()) {
        if(IS_ADJ) {
          hasActed_ = true;
          energyCur_ -= 20;
          assemblyHere->tickBuild();
        } else {
          tryStepTowards(p);
        }
      }
    } else {
      if(IS_ADJ) {
        hasActed_ = true;
        energyCur_ -= 20;

        Rigid* newRigid = nullptr;
        switch(assemblyType) {
          case AsmType::rechargeStation: {
            newRigid = World::replaceRigid(new RechargeStation, p);
          } break;

          case AsmType::road: {
            newRigid = World::replaceRigid(new Road, p);
          } break;
        }

        assert(newRigid && "Failed to create rigid");

        auto* const assembly = static_cast<Asm*>(newRigid);
        assembly->tickBuild();
      } else {
        tryStepTowards(p);
      }
    }
  }
}
