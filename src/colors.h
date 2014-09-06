#ifndef COLORS_H
#define COLORS_H

#include <SDL.h>

typedef SDL_Color Clr;

//Standard terminal colors (somewhat modified...)
//===========================================================
const Clr clrBlack              = {  0,   0,   0, SDL_ALPHA_OPAQUE};
const Clr clrGray               = {128, 128, 128, SDL_ALPHA_OPAQUE};
const Clr clrWhite              = {192, 192, 192, SDL_ALPHA_OPAQUE};
const Clr clrWhiteHigh          = {255, 255, 255, SDL_ALPHA_OPAQUE};

const Clr clrRed                = {128,   0,   0, SDL_ALPHA_OPAQUE};
const Clr clrRedLgt             = {255,   0,   0, SDL_ALPHA_OPAQUE};

const Clr clrGreen              = {  0, 128,   0, SDL_ALPHA_OPAQUE};
const Clr clrGreenLgt           = {  0, 255,   0, SDL_ALPHA_OPAQUE};

const Clr clrYellow             = {255, 255,   0, SDL_ALPHA_OPAQUE};
const Clr clrBlue               = {  0,   0, 139, SDL_ALPHA_OPAQUE};
const Clr clrBlueLgt            = { 92,  92, 255, SDL_ALPHA_OPAQUE};

const Clr clrMagenta            = {139,   0, 139, SDL_ALPHA_OPAQUE};
const Clr clrMagentaLgt         = {255,   0, 255, SDL_ALPHA_OPAQUE};

const Clr clrCyan               = {  0, 128, 128, SDL_ALPHA_OPAQUE};
const Clr clrCyanLgt            = {  0, 255, 255, SDL_ALPHA_OPAQUE};

const Clr clrBrown              = {153, 102,  61, SDL_ALPHA_OPAQUE};
//===========================================================

const Clr clrViolet             = {128,   0, 255, SDL_ALPHA_OPAQUE};
const Clr clrOrange             = {255, 128,   0, SDL_ALPHA_OPAQUE};
const Clr clrBrownDrk           = { 96,  64,  32, SDL_ALPHA_OPAQUE};
const Clr clrBrownXDrk          = { 48,  32,  16, SDL_ALPHA_OPAQUE};

const Clr clrMsgGood            = clrGreenLgt;
const Clr clrMsgBad             = clrRedLgt;
const Clr clrMsgImportant       = clrOrange;

#endif
