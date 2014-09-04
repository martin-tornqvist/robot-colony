#ifndef RENDERING_H
#define RENDERING_H

#include <vector>
#include <iostream>
#include <string>

#include "cmnTypes.h"

enum class RectType {filled, outline};

namespace Rendering {

void init();
void cleanup();

void renderPresent();

void clearScreen();

void coverArea(const Pos2& p, const Pos2& d);

void drawText(const std::string& str, const Pos2& p, const Clr& clr,
              const Clr& bgClr = clrBlack);

void drawRect(const Pos2& p, const Pos2& d, const Clr& clr, const RectType rectType);

void drawLine(const Pos2& p0, const Pos2& p1, const Clr& clr);

} //Render

#endif

