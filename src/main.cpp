#include "init.h"

#include <iostream>
#include <string>

#include "input.h"
#include "cmnTypes.h"
#include "cmnData.h"
#include "colors.h"
#include "rendering.h"
#include "sdlHandling.h"
#include "time.h"
#include "world.h"
#include "ent.h"
#include "mapParsing.h"
#include "utils.h"

using namespace std;

//Waits for the robot to be ready to act
static void waitToProceed() {
  const auto* const rbt = World::mobs[0];
  while(rbt->hasActed_ || rbt->nrTicksToSkip_ > 0) {}
}

//----------------------------------------------- C++ functions called from Lua
static int wait(lua_State* luaSt) {
  (void)luaSt;
  waitToProceed();
  World::mobs[0]->hasActed_ = true;
  return 0;
}

static int step_towards(lua_State* luaSt) {
  waitToProceed();
  const P p(lua_tointeger(luaSt, 1), lua_tointeger(luaSt, 2));
  World::mobs[0]->tryStepTowards(p);
  return 0;
}

static int build(lua_State* luaSt) {
  waitToProceed();

  const string name = lua_tostring(luaSt, 1);

  //TODO Use name to determine building type

  const P p(lua_tointeger(luaSt, 2), lua_tointeger(luaSt, 3));
  World::mobs[0]->tryBuild(AsmType::rechargeStation, p);
  return 0;
}

static int build_road_to(lua_State* luaSt) {
  const P p0(World::mobs[0]->getPos());
  const P p1(lua_tointeger(luaSt, 1), lua_tointeger(luaSt, 2));

  bool blocked[MAP_W][MAP_H];
  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      blocked[x][y] = World::rigids[x][y]->isBlocking();
    }
  }
  vector<P> path;
  PathFind::run(p0, p1, blocked, path, true);

  if(!path.empty()) {

    reverse(begin(path), end(path));

    for(const P& p : path) {
      const auto* const rigidHere   = World::rigids[p.x][p.y];
      const auto        entTypeHere = rigidHere->getEntType();
      bool isRoad = false;
      if(entTypeHere == EntType::assembly) {
        isRoad = static_cast<const Asm*>(rigidHere)->getAsmType() == AsmType::road;
      }
      if(isRoad || entTypeHere != EntType::assembly) {
        World::mobs[0]->tryBuild(AsmType::road, p);
      }
    }

  }

  return 0;
}

static int get_tick_nr(lua_State* luaSt) {
  const P p(World::mobs[0]->getPos());
  lua_pushnumber(luaSt, World::getTickNr());
  return 1;
}

static int get_rbt_pos(lua_State* luaSt) {
  const P p(World::mobs[0]->getPos());
  lua_pushnumber(luaSt, p.x);
  lua_pushnumber(luaSt, p.y);
  return 2;
}

static int is_rbt_at(lua_State* luaSt) {
  const P pRbt(World::mobs[0]->getPos());
  const P pArg(lua_tointeger(luaSt, 1), lua_tointeger(luaSt, 2));
  lua_pushboolean(luaSt, pRbt == pArg);
  return 1;
}

static int is_assembly_done_at(lua_State* luaSt) {
  const string name = lua_tostring(luaSt, 1);

  //TODO Use name to determine building type

  const P p(lua_tointeger(luaSt, 2), lua_tointeger(luaSt, 3));

  if(!Utils::isPosInMap(p, false)) {
    lua_pushboolean(luaSt, false);
  } else {
    const auto* rigid = World::rigids[p.x][p.y];
    if(rigid->getEntType() == EntType::assembly) {
      lua_pushboolean(luaSt, static_cast<const Asm*>(rigid)->isDone());
    } else {
      lua_pushboolean(luaSt, false);
    }
  }
  return 1;
}

static int get_rbt_energy(lua_State* luaSt) {
  const Rbt* const rbt = static_cast<const Rbt*>(World::mobs[0]);
  lua_pushnumber(luaSt, rbt->getEnergyCur());
  return 1;
}

static int get_rbt_energy_max(lua_State* luaSt) {
  const Rbt* const rbt = static_cast<const Rbt*>(World::mobs[0]);
  lua_pushnumber(luaSt, rbt->getEnergyMax());
  return 1;
}

static int get_rbt_energy_percent(lua_State* luaSt) {
  const Rbt* const rbt = static_cast<const Rbt*>(World::mobs[0]);
  lua_pushnumber(luaSt, (100 * rbt->getEnergyCur()) / rbt->getEnergyMax());
  return 1;
}

static int get_nearest_recharge_station(lua_State* luaSt) {
  bool blocked[MAP_W][MAP_H];
  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      blocked[x][y] = World::rigids[x][y]->isBlocking();
    }
  }

  const P rbtPos(World::mobs[0]->getPos());

  int flood[MAP_W][MAP_H];

  FloodFill::run(rbtPos, blocked, flood, INT_MAX, P(-1, -1), true);

  P ret(-1, -1);
  int curDistToNearest = INT_MAX;

  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      const auto* const rigidHere = World::rigids[x][y];
      if(rigidHere->getEntType() == EntType::assembly) {
        const auto* const assemblyHere      = static_cast<const Asm*>(rigidHere);
        const auto        assemblyTypeHere  = assemblyHere->getAsmType();
        if(assemblyTypeHere == AsmType::rechargeStation && assemblyHere->isDone()) {
          const P p(x, y);
          const int DIST = Utils::kingDist(rbtPos, p);
          if(DIST < curDistToNearest) {
            curDistToNearest = DIST;
            ret = p;
          }
        }
      }
    }
  }

  lua_pushnumber(luaSt, ret.x);
  lua_pushnumber(luaSt, ret.y);

  return 2;
}

//----------------------------------------------- Lua functions called from C++
static void luaAct(lua_State* luaSt) {
  lua_getglobal(luaSt, "act");
  lua_pcall(luaSt, 0, 0, 0);
}

static void luaInf(lua_State* luaSt) {
  lua_getglobal(luaSt, "display_info");
  lua_pcall(luaSt, 0, 1, 0);
}

//----------------------------------------------- Lua "act" function called from thread
bool isLuaDone = false;

static int luaActCalledFromThread(void* data) {
  assert(data);
  isLuaDone = false;
  luaAct(static_cast<lua_State*>(data));
  isLuaDone = true;
  return 0;
}

#ifdef _WIN32
#undef main
#endif
int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;

  TRACE_FUNC_BEGIN;

  lua_State* luaSt = nullptr; //The Lua interpreter

  Init::initIO();
  Init::initGame();
  Init::initSession(luaSt);

  lua_register(luaSt, "wait",                          wait);
  lua_register(luaSt, "step_towards",                  step_towards);
  lua_register(luaSt, "build",                         build);
  lua_register(luaSt, "build_road_to",                 build_road_to);
  lua_register(luaSt, "get_tick_nr",                   get_tick_nr);
  lua_register(luaSt, "get_rbt_pos",                   get_rbt_pos);
  lua_register(luaSt, "is_rbt_at",                     is_rbt_at);
  lua_register(luaSt, "is_assembly_done_at",           is_assembly_done_at);
  lua_register(luaSt, "get_rbt_energy",                get_rbt_energy);
  lua_register(luaSt, "get_rbt_energy_max",            get_rbt_energy_max);
  lua_register(luaSt, "get_rbt_energy_percent",        get_rbt_energy_percent);
  lua_register(luaSt, "get_nearest_recharge_station",  get_nearest_recharge_station);

  World::mobs.push_back(new Rbt(P(MAP_W_HALF - 1, MAP_H_HALF - 1)));

  World::replaceRigid(new RockWall(), P(10, 10));
  World::replaceRigid(new RockWall(), P(10, 11));
  World::replaceRigid(new RockWall(), P(11, 10));

  const Uint32 MS_PER_TICK = 1;
  Uint32 msLast = 0;

  SDL_Thread* thread1 = SDL_CreateThread(luaActCalledFromThread, nullptr, luaSt);

  bool quitGame = false;

  while(!quitGame) {

    const Uint32 MS_NOW = SDL_GetTicks();

    if(MS_NOW > (msLast + MS_PER_TICK)) {
      msLast = MS_NOW;

      Rendering::clearScreen();

      World::incrTickNr();

      for(int y = 0; y < MAP_H; ++y) {
        for(int x = 0; x < MAP_W; ++x) {
          World::rigids[x][y]->onTick();
        }
      }

      auto&       mobs  = World::mobs;
      auto* const rbt   = mobs[0];

      rbt->onTick();                          //Tick robot
      while(!rbt->hasActed_ && !isLuaDone) {} //Wait for robot to act or script to end
      for(size_t i = 1; i < mobs.size(); ++i) {mobs[i]->onTick();} //Tick other mobs

      Rendering::drawMap();

      luaInf(luaSt);
      const string str = lua_tostring(luaSt, -1);
      lua_pop(luaSt, 1);
      if(!str.empty()) {Rendering::drawText(str, P(0, 0), clrWhiteHigh, clrBlack);}
    }
    Rendering::renderPresent();

    Input::read(quitGame);

    SdlHandling::sleep(1);
  }

  //Note: The Lua script may be stuck in an infinite loop caused by the player's design.
  //Because of this it's difficult to:
  //(1) Exit cleanly, and
  //(2) Restart the game without exiting the program (i.e. an in-game restart button)
  //An abrupt "return" is done here without any cleanup. If development on this game
  //continues, a solution to this must be found.
  return 0;

//  SDL_WaitThread(thread1, nullptr);
//
//  Init::cleanupSession(luaSt);
//  Init::cleanupGame();
//  Init::cleanupIO();
//
//  TRACE_FUNC_END;
//  return 0;
}
