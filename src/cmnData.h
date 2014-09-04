#ifndef CMN_DATA_H
#define CMN_DATA_H

#include <string>

const std::string gameVersionStr = "v0.01";

const int SCR_BPP = 32;

const int SCR_W       = 60;
const int SCR_H       = 20;
const int MAP_W_HALF  = SCR_W / 2;
const int MAP_H_HALF  = SCR_H / 2;

//-----------------------------------------------------------------------------
//TODO Move to Config, make non-const:
const int CELL_PX_W   = 11;
const int CELL_PX_H   = 19;
const int SCR_PX_W    = SCR_W * CELL_PX_W;
const int SCR_PX_H    = SCR_H * CELL_PX_H;
const bool IS_FULLSCR = false;
//-----------------------------------------------------------------------------

#endif
