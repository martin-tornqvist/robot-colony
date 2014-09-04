#include "sdlHandling.h"

#include <iostream>

#include <SDL_image.h>

#include "Init.h"
#include "cmnTypes.h"

using namespace std;

namespace SdlHandling {

namespace {

bool isInited = false;

} //namespace

void init() {
  TRACE_FUNC_BEGIN;

  isInited = true;

  if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    TRACE << "Failed to init SDL: " << SDL_GetError() << endl;
    assert(false);
  }

  if(IMG_Init(IMG_INIT_PNG) == -1) {
    TRACE << "Failed to init SDL IMG: " << IMG_GetError() << endl;
    assert(false);
  }

  TRACE_FUNC_END;
}

void cleanup() {
  isInited = false;
  IMG_Quit();
  SDL_Quit();
}

void sleep(const Uint32 DURATION) {
  if(isInited) {
    if(DURATION == 1) {
      SDL_Delay(1);
    } else {
      const Uint32 WAIT_UNTIL = SDL_GetTicks() + DURATION;
      while(SDL_GetTicks() < WAIT_UNTIL) {SDL_PumpEvents();}
    }
  }
}

} //SdlHandling
