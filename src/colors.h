#ifndef COLORS_H
#define COLORS_H

#include <SDL.h>

typedef SDL_Color Clr;

//Standard terminal colors (somewhat modified...)
//===========================================================
const Clr clrBlack              = {  0,   0,   0, 0};
const Clr clrGray               = {128, 128, 128, 0};
const Clr clrWhite              = {192, 192, 192, 0};
const Clr clrWhiteHigh          = {255, 255, 255, 0};

const Clr clrRed                = {128,   0,   0, 0};
const Clr clrRedLgt             = {255,   0,   0, 0}; //(220, 0, 0, 0};

const Clr clrGreen              = {  0, 128,   0, 0};
const Clr clrGreenLgt           = {  0, 255,   0, 0};

const Clr clrYellow             = {255, 255,   0, 0};
const Clr clrBlue               = {  0,   0, 139, 0};
const Clr clrBlueLgt            = { 92,  92, 255, 0};

const Clr clrMagenta            = {139,   0, 139, 0};
const Clr clrMagentaLgt         = {255,   0, 255, 0};

const Clr clrCyan               = {  0, 128, 128, 0};
const Clr clrCyanLgt            = {  0, 255, 255, 0};

const Clr clrBrown              = {153, 102,  61, 0};
//===========================================================

const Clr clrViolet             = {128,   0, 255, 0};
const Clr clrOrange             = {255, 128,   0, 0};
const Clr clrBrownDrk           = { 96,  64,  32, 0};
const Clr clrBrownGray          = { 92,  84,  78, 0};

const Clr clrMsgGood            = clrGreenLgt;
const Clr clrMsgBad             = clrRedLgt;
const Clr clrMsgImportant       = clrOrange;

#endif
