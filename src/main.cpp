#include "init.h"

#include <iostream>
#include <chrono>
#include <string>

#include "input.h"
#include "cmnTypes.h"
#include "cmnData.h"
#include "colors.h"
#include "rendering.h"
#include "sdlHandling.h"
#include "time.h"
#include "world.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

using namespace std;

static int moveTo(lua_State* L) {
  const P p(lua_tointeger(L, 1), lua_tointeger(L, 2));
  World::mobs[0]->tryStepTowards(p);
  return 0;
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

  const Uint32 MS_PER_TICK = 100;
  Uint32 msLast = 0;

  bool quit = false;
  while(quit == false) {

    const Uint32 MS_NOW = SDL_GetTicks();

    if(MS_NOW > (msLast + MS_PER_TICK)) {
      msLast = MS_NOW;

      Rendering::clearScreen();

      lua_register(L, "moveTo", moveTo);      //Register function
      luaL_dofile(L, "../../script/rbt.lua"); //Run the script

      Time::tick();
      Rendering::drawMap();
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
