#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

#include <SDL.h>

namespace SdlHandling {

void init();

void cleanup();

void sleep(const Uint32 DURATION);

}

#endif
