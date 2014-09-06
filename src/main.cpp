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

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

using namespace std;

//----------------------------------------------- C++ functions called from Lua
static int moveTo(lua_State* L) {
  const P p(lua_tointeger(L, 1), lua_tointeger(L, 2));
  World::mobs[0]->tryStepTowards(p);
  return 0;
}

static int getRbtPos(lua_State* L) {
  const P p(World::mobs[0]->getPos());
  lua_pushnumber(L, p.x);
  lua_pushnumber(L, p.y);
  return 2;
}

//----------------------------------------------- Lua functions called from C++
static void luaAct(lua_State* L) {
  lua_getglobal(L, "act");
  lua_call(L, 0, 0);
}

static void luaInf(lua_State* L) {
  lua_getglobal(L, "displayInfo");
  lua_call(L, 0, 1);
}

#ifdef _WIN32
#undef main
#endif
int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;

  TRACE_FUNC_BEGIN;

  lua_State* L = nullptr; //The Lua interpreter

  Init::initIO();
  Init::initGame();
  Init::initSession();

  World::mobs.push_back(new Rbt(P(MAP_W_HALF - 1, MAP_H_HALF - 1)));

  L = luaL_newstate();  //initialize Lua
  luaL_openlibs(L);     //Load Lua base libraries

  const Uint32 MS_PER_TICK = 220;
  Uint32 msLast = 0;

  lua_register(L, "moveTo",     moveTo);
  lua_register(L, "getRbtPos",  getRbtPos);

  luaL_dofile(L, "../../script/rbt.lua"); //Run the script

  bool quit = false;
  while(quit == false) {

    const Uint32 MS_NOW = SDL_GetTicks();

    if(MS_NOW > (msLast + MS_PER_TICK)) {
      msLast = MS_NOW;

      Rendering::clearScreen();

      luaAct(L);

      Time::tick();
      Rendering::drawMap();

      luaInf(L);
      const string str = lua_tostring(L, -1);
      lua_pop(L, 1);
      if(!str.empty()) {
        Rendering::drawText(str, P(0, 0), clrBlack, clrGray);
      }
    }
    Rendering::renderPresent();

    Input::read(quit);

    SdlHandling::sleep(1);
  }

  lua_close(L); //Cleanup Lua

  Init::cleanupSession();
  Init::cleanupGame();
  Init::cleanupIO();

  TRACE_FUNC_END;
  return 0;
}
