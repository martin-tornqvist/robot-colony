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

void read(bool& quitGame, bool& quitSession) {
  if(isInited_) {
    while(SDL_PollEvent(&sdlEvent_) != 0) {

      if(sdlEvent_.type == SDL_QUIT) {quitGame = true;}

      if(sdlEvent_.type == SDL_KEYDOWN) {
        switch(sdlEvent_.key.keysym.sym) {
          case 'z': {quitSession = true;} break;

          default: {} break;
        }
      }
    }
  }
}

} //Input
