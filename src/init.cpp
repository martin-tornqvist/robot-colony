#include "Init.h"

#include <iostream>

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

void initSession(lua_State*& luaState) {
  TRACE_FUNC_BEGIN;
  World::init();
  luaState = luaL_newstate(); //initialize Lua
  luaL_openlibs(luaState);    //Load Lua base libraries
  luaL_dofile(luaState, "../../script/rbt.lua"); //Run the script
  TRACE_FUNC_END;
}

void cleanupSession(lua_State*& luaState) {
  TRACE_FUNC_BEGIN;
  lua_close(luaState); //Cleanup Lua
  luaState = nullptr;
  World::cleanup();
  TRACE_FUNC_END;
}

} //Init
