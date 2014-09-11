#include "Init.h"

#include <iostream>

#include "sdlHandling.h"
#include "rendering.h"
#include "input.h"
#include "world.h"

using namespace std;

namespace Init {

//Note: Initialization order matters in some cases
void initIO(lua_State*& luaState) {
  TRACE_FUNC_BEGIN;
  SdlHandling::init();
//  Config::init();
  Input::init();
  Rendering::init();
  luaState = luaL_newstate(); //initialize Lua
  luaL_openlibs(luaState);    //Load Lua base libraries
//  Audio::init();
  TRACE_FUNC_END;
}

void cleanupIO(lua_State*& luaState) {
  TRACE_FUNC_BEGIN;
//  Audio::cleanup();
  lua_close(luaState); //Cleanup Lua
  luaState = nullptr;
  Rendering::cleanup();
  SdlHandling::cleanup();
  TRACE_FUNC_END;
}

//Note: Initialization order matters in some cases
void initGame() {
  TRACE_FUNC_BEGIN;

  TRACE_FUNC_END;
}

void cleanupGame() {
  TRACE_FUNC_BEGIN;

  TRACE_FUNC_END;
}

//Note: Initialization order matters in some cases
void initSession() {
  TRACE_FUNC_BEGIN;
  World::init();
  TRACE_FUNC_END;
}

void cleanupSession() {
  TRACE_FUNC_BEGIN;
  World::cleanup();
  TRACE_FUNC_END;
}

} //Init
