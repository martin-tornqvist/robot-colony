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

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

using namespace std;

static int drawText(lua_State* L) {
  const string str = lua_tostring(L, 1);
  const P p(lua_tointeger(L, 2), lua_tointeger(L, 3));
  Rendering::drawText(str, p, clrWhite);
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

  L = luaL_newstate();  //initialize Lua
  luaL_openlibs(L);     //Load Lua base libraries

  const Uint32 MS_PER_TICK = 80;
  Uint32 msLast = 0;

  bool quit = false;
  while(quit == false) {

    const Uint32 MS_NOW = SDL_GetTicks();

    if(MS_NOW > (msLast + MS_PER_TICK)) {
      msLast = MS_NOW;

      Rendering::clearScreen();

      lua_register(L, "drawText", drawText);    //Register function
      luaL_dofile(L, "../../script/test.lua");  //Run the script


      Time::tick();
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
