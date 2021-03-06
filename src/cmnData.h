#ifndef CMN_DATA_H
#define CMN_DATA_H

#include <string>

const std::string gameVersionStr = "v0.01";

const int SCR_BPP = 32;

const int MAP_W       = 80;
const int MAP_H       = 24;
const int MAP_W_HALF  = MAP_W / 2;
const int MAP_H_HALF  = MAP_H / 2;

const int SCR_W       = MAP_W;
const int SCR_H       = MAP_H;
const int SCR_W_HALF  = SCR_W / 2;
const int SCR_H_HALF  = SCR_H / 2;

//-----------------------------------------------------------------------------
//TODO Move to Config, make non-const:
const std::string fontImg = "11x19.png";
const int CELL_PX_W       = 11;
const int CELL_PX_H       = 19;
const int SCR_PX_W        = SCR_W * CELL_PX_W;
const int SCR_PX_H        = SCR_H * CELL_PX_H;
const bool IS_FULLSCR     = false;
//-----------------------------------------------------------------------------

#endif
