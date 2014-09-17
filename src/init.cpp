#include "Init.h"

#include <iostream>
#include <setjmp.h>

#include "sdlHandling.h"
#include "rendering.h"
#include "input.h"
#include "world.h"

using namespace std;

namespace Init {

//Note: Initialization order matters in some cases
void initIO() {
  TRACE_FUNC_BEGIN;
  SdlHandling::init();
//  Config::init();
  Input::init();
  Rendering::init();
//  Audio::init();
  TRACE_FUNC_END;
}

void cleanupIO() {
  TRACE_FUNC_BEGIN;
//  Audio::cleanup();
  Rendering::cleanup();
  SdlHandling::cleanup();
  TRACE_FUNC_END;
}

void initGame() {
  TRACE_FUNC_BEGIN;

  TRACE_FUNC_END;
}

void cleanupGame() {
  TRACE_FUNC_BEGIN;

  TRACE_FUNC_END;
}

void initSession(lua_State*& luaSt) {
  TRACE_FUNC_BEGIN;
  World::init();
  luaSt = luaL_newstate(); //Init Lua
  luaL_openlibs(luaSt);    //Load Lua base libraries
#ifdef NDEBUG
  luaL_dofile(luaSt, "rbt.lua");
#else
  luaL_dofile(luaSt, "../../script/rbt.lua");
#endif
  TRACE_FUNC_END;
}

void cleanupSession(lua_State*& luaSt) {
  TRACE_FUNC_BEGIN;
  lua_close(luaSt);
  luaSt = nullptr;
  World::cleanup();
  TRACE_FUNC_END;
}

} //Init
