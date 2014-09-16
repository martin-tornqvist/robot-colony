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

static void waitRbtReady() {
  const auto* const rbt = World::mobs[0];
  while(rbt->hasActed_ || rbt->nrTicksToSkip_ > 0) {}
}

//----------------------------------------------- C++ functions called from Lua
static int wait(lua_State* luaSt) {
  (void)luaSt;
  waitRbtReady();
  World::mobs[0]->hasActed_ = true;
  return 0;
}

static int step_towards(lua_State* luaSt) {
  waitRbtReady();
  const P p(lua_tointeger(luaSt, 1), lua_tointeger(luaSt, 2));
  World::mobs[0]->tryStepTowards(p);
  return 0;
}

static int build(lua_State* luaSt) {
  waitRbtReady();

//  const string name = lua_tostring(luaSt, 1);

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
  lua_pushnumber(luaSt, Time::getTickNr());
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
  return pRbt == pArg;
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
        if(assemblyTypeHere == AsmType::rechargeStation && assemblyHere->isFinished()) {
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

//----------------------------------------------- Asdf
static int asdf(void* data) {
  assert(data);
  luaAct(static_cast<lua_State*>(data));
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

  bool quitGame = false;
  while(!quitGame) {

    Init::initSession(luaSt);

    lua_register(luaSt, "wait",                          wait);
    lua_register(luaSt, "step_towards",                  step_towards);
    lua_register(luaSt, "build",                         build);
    lua_register(luaSt, "build_road_to",                 build_road_to);
    lua_register(luaSt, "get_tick_nr",                   get_tick_nr);
    lua_register(luaSt, "get_rbt_pos",                   get_rbt_pos);
    lua_register(luaSt, "is_rbt_at",                     is_rbt_at);
    lua_register(luaSt, "get_rbt_energy",                get_rbt_energy);
    lua_register(luaSt, "get_rbt_energy_max",            get_rbt_energy_max);
    lua_register(luaSt, "get_rbt_energy_percent",        get_rbt_energy_percent);
    lua_register(luaSt, "get_nearest_recharge_station",  get_nearest_recharge_station);

    World::mobs.push_back(new Rbt(P(MAP_W_HALF - 1, MAP_H_HALF - 1)));

    World::replaceRigid(new RockWall(), P(10, 10));
    World::replaceRigid(new RockWall(), P(10, 11));
    World::replaceRigid(new RockWall(), P(11, 10));

    const Uint32 MS_PER_TICK = 100;
    Uint32 msLast = 0;

    SDL_Thread* thread1 = SDL_CreateThread(asdf, "t1", luaSt);

    bool quitSession = false;
    while(!quitSession && !quitGame) {

      const Uint32 MS_NOW = SDL_GetTicks();

      if(MS_NOW > (msLast + MS_PER_TICK)) {
        msLast = MS_NOW;

        Rendering::clearScreen();

        Time::tick();

        Rendering::drawMap();

        luaInf(luaSt);
        const string str = lua_tostring(luaSt, -1);
        lua_pop(luaSt, 1);
        if(!str.empty()) {Rendering::drawText(str, P(0, 0), clrWhiteHigh, clrBlack);}
      }
      Rendering::renderPresent();

      Input::read(quitGame, quitSession);

      SdlHandling::sleep(1);
    }

    SDL_WaitThread(thread1, nullptr);

    Init::cleanupSession(luaSt);
  }

  Init::cleanupGame();
  Init::cleanupIO();

  TRACE_FUNC_END;
  return 0;
}
