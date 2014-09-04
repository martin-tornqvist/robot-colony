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

using namespace std;

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

//using std::chrono::duration_cast;
//using std::chrono::microseconds;
//using std::chrono::seconds;
//using std::chrono::steady_clock;

//int lotsOfWork(void* data) {
//  (void)data;
//
//  for(int i = 0; i < 99999999; i++) {
//    int x = sqrt(i);
//    int y = x * i;
//    y = 1 + (y / 3);
//    if(y == -1) {return 0;}
//  }
//
//  return 0;
//}

//The Lua interpreter
lua_State* L = nullptr;

#ifdef _WIN32
#undef main
#endif
int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;

  TRACE_FUNC_BEGIN;

//  const int NR_CORES = SDL_GetCPUCount();
//  trace << "Nr cores: " << NR_CORES << endl << endl;
//
//  trace << "Running " << NR_CORES << " times sequentially...\n";
//  steady_clock::time_point start = steady_clock::now();
//  for(int i = 0 ; i < NR_CORES; i++) {
//    lotsOfWork(nullptr);
//  }
//  steady_clock::time_point end = steady_clock::now();
//  trace << "Done!\n";
//  trace << "Time: " << duration_cast<seconds>(end - start).count() << "s.\n\n";
//
//  trace << "Running with " << NR_CORES << " threads...\n";
//  start = steady_clock::now();
//  SDL_Thread* thread1 = SDL_CreateThread(lotsOfWork, "1", nullptr);
//  SDL_Thread* thread2 = SDL_CreateThread(lotsOfWork, "2", nullptr);
//  SDL_Thread* thread3 = SDL_CreateThread(lotsOfWork, "2", nullptr);
//  SDL_Thread* thread4 = SDL_CreateThread(lotsOfWork, "2", nullptr);
//  SDL_WaitThread(thread1, nullptr);
//  SDL_WaitThread(thread2, nullptr);
//  SDL_WaitThread(thread3, nullptr);
//  SDL_WaitThread(thread4, nullptr);
//  end = steady_clock::now();
//  trace << "Done!\n";
//  trace << "Time: " << duration_cast<seconds>(end - start).count() << "s.\n";


  Init::initIO();
  Init::initGame();
  Init::initSession();

  //initialize Lua
  L = luaL_newstate();

  //Load Lua base libraries
  luaL_openlibs(L);



  const Uint32 MS_PER_TICK = 16;
  Uint32 msLast = 0;

  bool quit = false;
  while(quit == false) {

    const Uint32 MS_NOW = SDL_GetTicks();

    if(MS_NOW > (msLast + MS_PER_TICK)) {
      msLast = MS_NOW;

      Rendering::clearScreen();

  //Run the script
  luaL_dofile(L, "../../src/script/test.lua");
      lua_getglobal(L, "getStr");
      lua_call(L, 0, 1);
      const string str = lua_tostring(L, -1);
	    lua_pop(L, 1);

      Rendering::drawText(str, Pos2(0, 0), clrWhite);

      Time::tick();
    }
    Rendering::renderPresent();

    Input::read(quit);

    SdlHandling::sleep(1);
  }

  //Cleanup Lua
  lua_close(L);

  Init::cleanupSession();
  Init::cleanupGame();
  Init::cleanupIO();

  TRACE_FUNC_END;
  return 0;
}
