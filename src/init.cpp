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
