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

void coverArea(const P& p, const P& d);

//void drawGlyphInMap(const char GLYPH, const P& p, const Clr& clr);

void drawText(const std::string& str, const P& p, const Clr& clr,
              const Clr& bgClr = clrBlack);

void drawRect(const P& p, const P& d, const Clr& clr, const RectType rectType);

void drawLine(const P& p0, const P& p1, const Clr& clr);

void drawMap();

} //Render

#endif

