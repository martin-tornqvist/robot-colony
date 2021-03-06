#include "ent.h"

#include "mapParsing.h"
#include "utils.h"
#include "rendering.h"
#include "world.h"
#include "time.h"

using namespace std;

//------------------------------------------------------------------- MOBILE
void Mob::tryGoTowards(const P& p) {
  if(!hasTriedAct_ && p != p_ && Utils::isPosInMap(p, true) && canGo()) {

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
      onDidGo();
    }
  }

  hasTriedAct_ = true;
}

void Mob::onTick() {
  if(nrTicksToSkip_ <= 0) {
    hasTriedAct_ = false;
    onTick_();
  } else {
    hasTriedAct_ = true;
  }

  nrTicksToSkip_  = max(0, nrTicksToSkip_ - 1);
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
  if(isDone()) {
    return def;
  } else {
    //"Animation"
    if(!isBeingBuiltThisTick_ || (World::getTickNr() % 2) == 0) {
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

void Rbt::onTick_() {
  energyCur_ = min(energyMax_, energyCur_ + 1);

  auto* rigidHere = World::rigids[p_.x][p_.y];
  if(rigidHere->getEntType() == EntType::assembly) {
    const auto* const assemblyHere     = static_cast<const Asm*>(rigidHere);
    const auto        assemblyTypeHere = assemblyHere->getAsmType();
    if(assemblyTypeHere == AsmType::rechargeStation && assemblyHere->isDone()) {
      energyCur_ = min(energyMax_, energyCur_ + 100);
    }
  }
}

bool Rbt::canGo() const {
  return energyCur_ >= static_cast<int>(RbtActionCosts::go);
}

void Rbt::onDidGo() {
  energyCur_ -= static_cast<int>(RbtActionCosts::go);

  auto* const rigidHere   = World::rigids[p_.x][p_.y];
  const auto  entTypeHere = rigidHere->getEntType();

  bool isRoad = false;

  if(entTypeHere == EntType::assembly) {
    auto* const assemblyHere     = static_cast<Asm*>(rigidHere);
    const auto  assemblyTypeHere = assemblyHere->getAsmType();
    isRoad = assemblyTypeHere == AsmType::road && assemblyHere->isDone();
  }

  if(!isRoad) {nrTicksToSkip_ = 1;}
}

void Rbt::tryBuild(const AsmType assemblyType, const P& p) {
  if(!hasTriedAct_ && energyCur_ >= static_cast<int>(RbtActionCosts::build)) {
    auto* const rigidHere   = World::rigids[p.x][p.y];
    const auto  entTypeHere = rigidHere->getEntType();
    const bool  IS_ADJ      = Utils::isPosAdj(p_, p, true);

    bool buildNew = true;

    if(entTypeHere == EntType::assembly) {
      auto* const assemblyHere     = static_cast<Asm*>(rigidHere);
      const auto  assemblyTypeHere = assemblyHere->getAsmType();
      if(assemblyTypeHere == assemblyType) {
        buildNew = false;
        if(!assemblyHere->isDone()) {
          if(IS_ADJ) {
            energyCur_ -= static_cast<int>(RbtActionCosts::build);
            assemblyHere->tickBuild();
          } else {
            tryGoTowards(p);
          }
        }
      }
    }

    if(buildNew) {
      if(IS_ADJ) {
        energyCur_ -= static_cast<int>(RbtActionCosts::build);

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
        tryGoTowards(p);
      }
    }
  }
  hasTriedAct_ = true;
}
