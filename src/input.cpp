#include "input.h"

#include <SDL.H>
#include <iostream>

#include "init.h"
#include "CmnTypes.h"

using namespace std;

namespace Input {

namespace {

SDL_Event sdlEvent_;
bool      isInited_ = false;

} //namespace

void init() {
  isInited_ = true;
}

void read(bool& quit) {
  if(isInited_) {
    while(SDL_PollEvent(&sdlEvent_) != 0) {

      if(sdlEvent_.type == SDL_QUIT) {quit = true;}

    }
  }
}

} //Input
