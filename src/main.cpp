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

using namespace std;

//----------------------------------------------- C++ functions called from Lua
static int wait(lua_State* luaState) {
  (void)luaState;
  World::mobs[0]->hasActed_ = true;
  return 0;
}

static int moveTo(lua_State* luaState) {
  const P p(lua_tointeger(luaState, 1), lua_tointeger(luaState, 2));
  World::mobs[0]->tryStepTowards(p);
  return 0;
}

static int build(lua_State* luaState) {
  const P p(lua_tointeger(luaState, 1), lua_tointeger(luaState, 2));
  World::mobs[0]->tryBuild(p);
  return 0;
}

static int getTickNr(lua_State* luaState) {
  const P p(World::mobs[0]->getPos());
  lua_pushnumber(luaState, Time::getTickNr());
  return 1;
}

static int getRbtPos(lua_State* luaState) {
  const P p(World::mobs[0]->getPos());
  lua_pushnumber(luaState, p.x);
  lua_pushnumber(luaState, p.y);
  return 2;
}

static int getRbtPwr(lua_State* luaState) {
  const Rbt* const rbt = static_cast<const Rbt*>(World::mobs[0]);
  lua_pushnumber(luaState, rbt->getPwrCur());
  return 1;
}

static int getRbtPwrMax(lua_State* luaState) {
  const Rbt* const rbt = static_cast<const Rbt*>(World::mobs[0]);
  lua_pushnumber(luaState, rbt->getPwrMax());
  return 1;
}

static int getRbtPwrPct(lua_State* luaState) {
  const Rbt* const rbt = static_cast<const Rbt*>(World::mobs[0]);
  lua_pushnumber(luaState, (100 * rbt->getPwrCur()) / rbt->getPwrMax());
  return 1;
}

//----------------------------------------------- Lua functions called from C++
static void luaAct(lua_State* luaState) {
  lua_getglobal(luaState, "act");
  lua_pcall(luaState, 0, 0, 0);
}

static void luaInf(lua_State* luaState) {
  lua_getglobal(luaState, "displayInfo");
  lua_pcall(luaState, 0, 1, 0);
}

#ifdef _WIN32
#undef main
#endif
int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;

  TRACE_FUNC_BEGIN;

  lua_State* luaState = nullptr; //The Lua interpreter

  Init::initIO(luaState);

  lua_register(luaState, "wait",         wait);
  lua_register(luaState, "moveTo",       moveTo);
  lua_register(luaState, "build",        build);
  lua_register(luaState, "getTickNr",    getTickNr);
  lua_register(luaState, "getRbtPos",    getRbtPos);
  lua_register(luaState, "getRbtPwr",    getRbtPwr);
  lua_register(luaState, "getRbtPwrMax", getRbtPwrMax);
  lua_register(luaState, "getRbtPwrPct", getRbtPwrPct);

  Init::initGame();

  bool quitGame = false;
  while(!quitGame) {

    Init::initSession();

    World::mobs.push_back(new Rbt(P(MAP_W_HALF - 1, MAP_H_HALF - 1)));

    World::replaceRigid(new RockWall(), P(10, 10));
    World::replaceRigid(new RockWall(), P(10, 11));
    World::replaceRigid(new RockWall(), P(11, 10));

    const Uint32 MS_PER_TICK = 60;
    Uint32 msLast = 0;

    luaL_dofile(luaState, "../../script/rbt.lua"); //Run the script

    bool quitSession = false;
    while(!quitSession && !quitGame) {

      const Uint32 MS_NOW = SDL_GetTicks();

      if(MS_NOW > (msLast + MS_PER_TICK)) {
        msLast = MS_NOW;

        Rendering::clearScreen();

        Time::tick();

        luaAct(luaState);

        Rendering::drawMap();

        luaInf(luaState);
        const string str = lua_tostring(luaState, -1);
        lua_pop(luaState, 1);
        if(!str.empty()) {Rendering::drawText(str, P(0, 0), clrWhiteHigh, clrBlack);}
      }
      Rendering::renderPresent();

      Input::read(quitGame, quitSession);

      SdlHandling::sleep(1);
    }

    Init::cleanupSession();
  }

  Init::cleanupGame();
  Init::cleanupIO(luaState);

  TRACE_FUNC_END;
  return 0;
}
