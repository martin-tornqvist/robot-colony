#include "rendering.h"

#include <vector>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "Init.h"
#include "cmnData.h"
#include "ent.h"
#include "world.h"

using namespace std;

namespace Rendering {

namespace {

SDL_Window*   sdlWindow_      = nullptr;
SDL_Renderer* sdlRenderer_    = nullptr;
//SDL_Texture*  sdlTexture_     = nullptr;

bool fontPxData_[258][176];

bool isInited() {
  return sdlWindow_ != nullptr;
}

void setRenderClr(const Clr& clr) {
  SDL_SetRenderDrawColor(sdlRenderer_, clr.r, clr.g, clr.b, 255);
}

//void tryFreeTexture() {
//  if(sdlTexture_) {
//    SDL_DestroyTexture(sdlTexture_);
//    sdlTexture_ = nullptr;
//  }
//}

Uint32 getPx(SDL_Surface* const surface, const int PX_X, const int PX_Y) {
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to retrieve */
  Uint8* p = (Uint8*)surface->pixels + PX_Y * surface->pitch + PX_X * bpp;

  switch(bpp) {
    case 1:   return *p;          break;
    case 2:   return *(Uint16*)p; break;
    case 3: {
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
        return p[0] << 16 | p[1] << 8 | p[2];
      }   else {
        return p[0] | p[1] << 8 | p[2] << 16;
      }
    } break;
    case 4:   return *(Uint32*)p; break;
    default:  return -1;          break;
  }
  return -1;
}

void putPx(const P& pxPos) {
  SDL_RenderDrawPoint(sdlRenderer_, pxPos.x, pxPos.y);
}

void loadFontData() {
  TRACE_FUNC_BEGIN;

  SDL_Surface* fontSurfaceTmp = IMG_Load("16x24.png"); //IMG_Load("11x19.png");

  Uint32 bgClr = SDL_MapRGB(fontSurfaceTmp->format, 0, 0, 0);

  for(int y = 0; y < fontSurfaceTmp->h; ++y) {
    for(int x = 0; x < fontSurfaceTmp->w; ++x) {
      fontPxData_[x][y] = getPx(fontSurfaceTmp, x, y) != bgClr;
    }
  }

  SDL_FreeSurface(fontSurfaceTmp);

  TRACE_FUNC_END;
}

P getGlyphPos(const char GLYPH) {
  switch(GLYPH) {
    default:  {} break;
    case ' ': return P(0, 0);  break;
    case '!': return P(1, 0); break;
    case '"': return P(2, 0); break;
    case '#': return P(3, 0); break;
    case '%': return P(4, 0); break;
    case '&': return P(5, 0); break;
    case  39: return P(6, 0); break;
    case '(': return P(7, 0); break;
    case ')': return P(8, 0); break;
    case '*': return P(9, 0); break;
    case '+': return P(10, 0); break;
    case ',': return P(11, 0); break;
    case '-': return P(12, 0); break;
    case '.': return P(13, 0); break;
    case '/': return P(14, 0); break;
    case '0': return P(15, 0); break;
    case '1': return P(0, 1); break;
    case '2': return P(1, 1); break;
    case '3': return P(2, 1); break;
    case '4': return P(3, 1); break;
    case '5': return P(4, 1); break;
    case '6': return P(5, 1); break;
    case '7': return P(6, 1); break;
    case '8': return P(7, 1); break;
    case '9': return P(8, 1); break;
    case ':': return P(9, 1); break;
    case ';': return P(10, 1); break;
    case '<': return P(11, 1); break;
    case '=': return P(12, 1); break;
    case '>': return P(13, 1); break;
    case '?': return P(14, 1); break;
    case '@': return P(15, 1); break;
    case 'A': return P(0, 2); break;
    case 'B': return P(1, 2); break;
    case 'C': return P(2, 2); break;
    case 'D': return P(3, 2); break;
    case 'E': return P(4, 2); break;
    case 'F': return P(5, 2); break;
    case 'G': return P(6, 2); break;
    case 'H': return P(7, 2); break;
    case 'I': return P(8, 2); break;
    case 'J': return P(9, 2); break;
    case 'K': return P(10, 2); break;
    case 'L': return P(11, 2); break;
    case 'M': return P(12, 2); break;
    case 'N': return P(13, 2); break;
    case 'O': return P(14, 2); break;
    case 'P': return P(15, 2); break;
    case 'Q': return P(0, 3); break;
    case 'R': return P(1, 3); break;
    case 'S': return P(2, 3); break;
    case 'T': return P(3, 3); break;
    case 'U': return P(4, 3); break;
    case 'V': return P(5, 3); break;
    case 'W': return P(6, 3); break;
    case 'X': return P(7, 3); break;
    case 'Y': return P(8, 3); break;
    case 'Z': return P(9, 3); break;
    case '[': return P(10, 3); break;
    case  92: return P(11, 3);  break;
    case ']': return P(12, 3);  break;
    case '^': return P(13, 3);  break;
    case '_': return P(14, 3);  break;
    case '`': return P(15, 3);  break;
    case 'a': return P(0, 4);  break;
    case 'b': return P(1, 4);  break;
    case 'c': return P(2, 4);  break;
    case 'd': return P(3, 4);  break;
    case 'e': return P(4, 4);  break;
    case 'f': return P(5, 4);  break;
    case 'g': return P(6, 4);  break;
    case 'h': return P(7, 4);  break;
    case 'i': return P(8, 4);  break;
    case 'j': return P(9, 4);  break;
    case 'k': return P(10, 4);  break;
    case 'l': return P(11, 4);  break;
    case 'm': return P(12, 4);  break;
    case 'n': return P(13, 4);  break;
    case 'o': return P(14, 4);  break;
    case 'p': return P(15, 4);  break;
    case 'q': return P(0, 5);  break;
    case 'r': return P(1, 5);  break;
    case 's': return P(2, 5);  break;
    case 't': return P(3, 5);  break;
    case 'u': return P(4, 5);  break;
    case 'v': return P(5, 5);  break;
    case 'w': return P(6, 5);  break;
    case 'x': return P(7, 5);  break;
    case 'y': return P(8, 5);  break;
    case 'z': return P(9, 5);  break;
    case '{': return P(10, 5);  break;
    case '|': return P(11, 5);  break;
    case '}': return P(12, 5);  break;
    case '~': return P(13, 5);  break;
    case   1: return P(14, 5);  break;
    case   2: return P(0, 6);  break;
    case   3: return P(1, 6);  break;
    case   4: return P(2, 6);  break;
    case   5: return P(3, 6); break;
    case   6: return P(4, 6); break;
    case   7: return P(5, 6); break;
    case   8: return P(6, 6); break;
    case   9: return P(7, 6); break;
    case  10: return P(8, 6); break;
  }
  TRACE << "Illegal glyph" << endl;
  assert(false);
  return P(0, 0);
}

void putPxsOnScreenForGlyph(const char GLYPH, const P& pxPos, const Clr& clr) {
  const P sheetPos = getGlyphPos(GLYPH);
  const P sheetPxPos0(sheetPos.x * CELL_PX_W, sheetPos.y * CELL_PX_H);
  const P sheetPxPos1(sheetPxPos0.x + CELL_PX_W - 1, sheetPxPos0.y + CELL_PX_H - 1);

  int screenPxX = pxPos.x;
  int screenPxY = pxPos.y;

  setRenderClr(clr);

  for(int sheetPxY = sheetPxPos0.y; sheetPxY <= sheetPxPos1.y; sheetPxY++) {
    screenPxX = pxPos.x;
    for(int sheetPxX = sheetPxPos0.x; sheetPxX <= sheetPxPos1.x; sheetPxX++) {
      if(fontPxData_[sheetPxX][sheetPxY]) {putPx(P(screenPxX, screenPxY));}
      ++screenPxX;
    }
    ++screenPxY;
  }
}

void drawGlyphAtPx(const char GLYPH, const P& pxPos, const Clr& clr,
                   const bool DRAW_BG_CLR, const Clr& bgClr = clrBlack) {
  if(DRAW_BG_CLR) {drawRect(pxPos, P(CELL_PX_W, CELL_PX_H), bgClr, RectType::filled);}
  putPxsOnScreenForGlyph(GLYPH, pxPos, clr);
}

void drawGlyphInMap(const char GLYPH, const P& p, const Clr& clr, const Clr& bgClr) {
  if(isInited()) {
    if(p.x < 0 || p.y < 0 || p.x >= MAP_W || p.y >= MAP_H) {return;}

    const P pxPos(p.x * CELL_PX_W, p.y * CELL_PX_H);

    drawGlyphAtPx(GLYPH, pxPos, clr, true, bgClr);
  }
}

} //Namespace

void init() {
  TRACE_FUNC_BEGIN;
  cleanup();

  TRACE << "Setting up rendering window" << endl;
  const string title = "Robot Colony " + gameVersionStr;

  sdlWindow_ = SDL_CreateWindow(
                 title.data(),
                 SDL_WINDOWPOS_UNDEFINED,
                 SDL_WINDOWPOS_UNDEFINED,
                 SCR_PX_W, SCR_PX_H,
                 SDL_WINDOW_SHOWN);

  sdlRenderer_ = SDL_CreateRenderer(sdlWindow_, -1, SDL_RENDERER_ACCELERATED);

  loadFontData();

  TRACE_FUNC_END;
}

void cleanup() {
  TRACE_FUNC_BEGIN;

  if(sdlRenderer_) {
    SDL_DestroyRenderer(sdlRenderer_);
    sdlRenderer_ = nullptr;
  }

  if(sdlWindow_) {
    SDL_DestroyWindow(sdlWindow_);
    sdlWindow_ = nullptr;
  }

  TRACE_FUNC_END;
}

void coverArea(const P& p, const P& d) {
  drawRect(p, d, clrBlack, RectType::filled);
}

void drawText(const string& str, const P& p, const Clr& clr, const Clr& bgClr) {
  if(isInited()) {
    if(p.y < 0 || p.y >= SCR_H) {return;}

    const int LEN = str.size();

    P pxPos(p.x * CELL_PX_W, p.y * CELL_PX_H);

    drawRect(pxPos, P(LEN * CELL_PX_W, CELL_PX_H), bgClr, RectType::filled);

    for(const char GLYPH : str) {
      if(pxPos.x < 0 || pxPos.x >= SCR_PX_H) {return;}
      drawGlyphAtPx(GLYPH, pxPos, clr, false);
      pxPos.x += CELL_PX_W;
    }
  }
}

void drawRect(const P& p, const P& d, const Clr& clr, const RectType rectType) {
  if(isInited()) {
    const SDL_Rect sdlRect = {(Sint16)p.x, (Sint16)p.y, (Uint16)d.x, (Uint16)d.y};
    setRenderClr(clr);
    if(rectType == RectType::filled) {
      SDL_RenderFillRect(sdlRenderer_, &sdlRect);
    } else {
      SDL_RenderDrawRect(sdlRenderer_, &sdlRect);
    }
  }
}

void drawLine(const P& pxPos0, const P& pxPos1, const Clr& clr) {
  setRenderClr(clr);
  SDL_RenderDrawLine(sdlRenderer_, pxPos0.x, pxPos0.y, pxPos1.x, pxPos1.y);
}

void renderPresent() {
  if(isInited()) {SDL_RenderPresent(sdlRenderer_);}
}

void clearScreen() {
  if(isInited()) {
    setRenderClr(clrBlack);
    SDL_RenderClear(sdlRenderer_);
  }
}

void drawMap() {
  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      const auto* rigid = World::rigids[x][y];
      const auto drawInf = rigid->getGlyphAndClr();
      drawGlyphInMap(drawInf.glyph, P(x, y), drawInf.clr, drawInf.clrBg);
    }
  }

  for(const Mob* const mob : World::mobs) {
    const auto drawInf = mob->getGlyphAndClr();
    drawGlyphInMap(drawInf.glyph, mob->getPos(), drawInf.clr, drawInf.clrBg);
  }
}

} //Rendering

