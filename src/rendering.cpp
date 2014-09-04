#include "rendering.h"

#include <vector>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "Init.h"
#include "cmnData.h"

using namespace std;

namespace Rendering {

namespace {

SDL_Window*   sdlWindow_      = nullptr;
SDL_Renderer* sdlRenderer_    = nullptr;
//SDL_Texture*  sdlTexture_     = nullptr;

bool fontPxData_[177][137];

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

void putPx(const Pos2& pxPos) {
  SDL_RenderDrawPoint(sdlRenderer_, pxPos.x, pxPos.y);
}

void loadFontData() {
  TRACE_FUNC_BEGIN;

  SDL_Surface* fontSurfaceTmp = IMG_Load("11x19.png");

  Uint32 bgClr = SDL_MapRGB(fontSurfaceTmp->format, 0, 0, 0);

  for(int y = 0; y < fontSurfaceTmp->h; ++y) {
    for(int x = 0; x < fontSurfaceTmp->w; ++x) {
      fontPxData_[x][y] = getPx(fontSurfaceTmp, x, y) != bgClr;
    }
  }

  SDL_FreeSurface(fontSurfaceTmp);

  TRACE_FUNC_END;
}

Pos2 getGlyphPos2(const char GLYPH) {
  switch(GLYPH) {
    default:  {} break;
    case ' ': return Pos2(0, 0);  break;
    case '!': return Pos2(1, 0); break;
    case '"': return Pos2(2, 0); break;
    case '#': return Pos2(3, 0); break;
    case '%': return Pos2(4, 0); break;
    case '&': return Pos2(5, 0); break;
    case  39: return Pos2(6, 0); break;
    case '(': return Pos2(7, 0); break;
    case ')': return Pos2(8, 0); break;
    case '*': return Pos2(9, 0); break;
    case '+': return Pos2(10, 0); break;
    case ',': return Pos2(11, 0); break;
    case '-': return Pos2(12, 0); break;
    case '.': return Pos2(13, 0); break;
    case '/': return Pos2(14, 0); break;
    case '0': return Pos2(15, 0); break;
    case '1': return Pos2(0, 1); break;
    case '2': return Pos2(1, 1); break;
    case '3': return Pos2(2, 1); break;
    case '4': return Pos2(3, 1); break;
    case '5': return Pos2(4, 1); break;
    case '6': return Pos2(5, 1); break;
    case '7': return Pos2(6, 1); break;
    case '8': return Pos2(7, 1); break;
    case '9': return Pos2(8, 1); break;
    case ':': return Pos2(9, 1); break;
    case ';': return Pos2(10, 1); break;
    case '<': return Pos2(11, 1); break;
    case '=': return Pos2(12, 1); break;
    case '>': return Pos2(13, 1); break;
    case '?': return Pos2(14, 1); break;
    case '@': return Pos2(15, 1); break;
    case 'A': return Pos2(0, 2); break;
    case 'B': return Pos2(1, 2); break;
    case 'C': return Pos2(2, 2); break;
    case 'D': return Pos2(3, 2); break;
    case 'E': return Pos2(4, 2); break;
    case 'F': return Pos2(5, 2); break;
    case 'G': return Pos2(6, 2); break;
    case 'H': return Pos2(7, 2); break;
    case 'I': return Pos2(8, 2); break;
    case 'J': return Pos2(9, 2); break;
    case 'K': return Pos2(10, 2); break;
    case 'L': return Pos2(11, 2); break;
    case 'M': return Pos2(12, 2); break;
    case 'N': return Pos2(13, 2); break;
    case 'O': return Pos2(14, 2); break;
    case 'P': return Pos2(15, 2); break;
    case 'Q': return Pos2(0, 3); break;
    case 'R': return Pos2(1, 3); break;
    case 'S': return Pos2(2, 3); break;
    case 'T': return Pos2(3, 3); break;
    case 'U': return Pos2(4, 3); break;
    case 'V': return Pos2(5, 3); break;
    case 'W': return Pos2(6, 3); break;
    case 'X': return Pos2(7, 3); break;
    case 'Y': return Pos2(8, 3); break;
    case 'Z': return Pos2(9, 3); break;
    case '[': return Pos2(10, 3); break;
    case  92: return Pos2(11, 3);  break;
    case ']': return Pos2(12, 3);  break;
    case '^': return Pos2(13, 3);  break;
    case '_': return Pos2(14, 3);  break;
    case '`': return Pos2(15, 3);  break;
    case 'a': return Pos2(0, 4);  break;
    case 'b': return Pos2(1, 4);  break;
    case 'c': return Pos2(2, 4);  break;
    case 'd': return Pos2(3, 4);  break;
    case 'e': return Pos2(4, 4);  break;
    case 'f': return Pos2(5, 4);  break;
    case 'g': return Pos2(6, 4);  break;
    case 'h': return Pos2(7, 4);  break;
    case 'i': return Pos2(8, 4);  break;
    case 'j': return Pos2(9, 4);  break;
    case 'k': return Pos2(10, 4);  break;
    case 'l': return Pos2(11, 4);  break;
    case 'm': return Pos2(12, 4);  break;
    case 'n': return Pos2(13, 4);  break;
    case 'o': return Pos2(14, 4);  break;
    case 'p': return Pos2(15, 4);  break;
    case 'q': return Pos2(0, 5);  break;
    case 'r': return Pos2(1, 5);  break;
    case 's': return Pos2(2, 5);  break;
    case 't': return Pos2(3, 5);  break;
    case 'u': return Pos2(4, 5);  break;
    case 'v': return Pos2(5, 5);  break;
    case 'w': return Pos2(6, 5);  break;
    case 'x': return Pos2(7, 5);  break;
    case 'y': return Pos2(8, 5);  break;
    case 'z': return Pos2(9, 5);  break;
    case '{': return Pos2(10, 5);  break;
    case '|': return Pos2(11, 5);  break;
    case '}': return Pos2(12, 5);  break;
    case '~': return Pos2(13, 5);  break;
    case   1: return Pos2(14, 5);  break;
    case   2: return Pos2(0, 6);  break;
    case   3: return Pos2(1, 6);  break;
    case   4: return Pos2(2, 6);  break;
    case   5: return Pos2(3, 6); break;
    case   6: return Pos2(4, 6); break;
    case   7: return Pos2(5, 6); break;
    case   8: return Pos2(6, 6); break;
    case   9: return Pos2(7, 6); break;
    case  10: return Pos2(8, 6); break;
  }
  TRACE << "Illegal glyph" << endl;
  assert(false);
  return Pos2(0, 0);
}

void putPxsOnScreenForGlyph(const char GLYPH, const Pos2& pxPos, const Clr& clr) {
  const Pos2 sheetPos = getGlyphPos2(GLYPH);
  const Pos2 sheetPxPos0(sheetPos.x * CELL_PX_W, sheetPos.y * CELL_PX_H);
  const Pos2 sheetPxPos1(sheetPxPos0.x + CELL_PX_W - 1, sheetPxPos0.y + CELL_PX_H - 1);

  int screenPxX = pxPos.x;
  int screenPxY = pxPos.y;

  setRenderClr(clr);

  for(int sheetPxY = sheetPxPos0.y; sheetPxY <= sheetPxPos1.y; sheetPxY++) {
    screenPxX = pxPos.x;
    for(int sheetPxX = sheetPxPos0.x; sheetPxX <= sheetPxPos1.x; sheetPxX++) {
      if(fontPxData_[sheetPxX][sheetPxY]) {putPx(Pos2(screenPxX, screenPxY));}
      ++screenPxX;
    }
    ++screenPxY;
  }
}

void drawGlyphAtPx(const char GLYPH, const Pos2& pxPos, const Clr& clr,
                   const bool DRAW_BG_CLR, const Clr& bgClr = clrBlack) {
  if(DRAW_BG_CLR) {drawRect(pxPos, Pos2(CELL_PX_W, CELL_PX_H), bgClr, RectType::filled);}
  putPxsOnScreenForGlyph(GLYPH, pxPos, clr);
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

//  tryFreeTexture();

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

void coverArea(const Pos2& p, const Pos2& d) {
  drawRect(p, d, clrBlack, RectType::filled);
}

void drawText(const string& str, const Pos2& p, const Clr& clr, const Clr& bgClr) {
  if(isInited()) {
    if(p.y < 0 || p.y >= SCR_H) {return;}

    const int LEN = str.size();
    drawRect(p, Pos2(LEN, 1), bgClr, RectType::filled);

    Pos2 pxPos(p.x * CELL_PX_W, p.y * CELL_PX_H);

    for(const char GLYPH : str) {
      if(pxPos.x < 0 || pxPos.x >= SCR_PX_H) {return;}
      drawGlyphAtPx(GLYPH, pxPos, clr, false);
      pxPos.x += CELL_PX_W;
    }
  }
}

void drawRect(const Pos2& p, const Pos2& d, const Clr& clr, const RectType rectType) {
  if(isInited()) {
    const Pos2 pxPos0(p.x * CELL_PX_W, p.y * CELL_PX_H);
    const Pos2 pxD(d.x * CELL_PX_W, d.y * CELL_PX_H);
    const SDL_Rect sdlRect =
    {(Sint16)pxPos0.x, (Sint16)pxPos0.y, (Uint16)pxD.x, (Uint16)pxD.y};
    setRenderClr(clr);
    if(rectType == RectType::filled) {
      SDL_RenderFillRect(sdlRenderer_, &sdlRect);
    } else {
      SDL_RenderDrawRect(sdlRenderer_, &sdlRect);
    }
  }
}

void drawLine(const Pos2& p0, const Pos2& p1, const Clr& clr) {
  setRenderClr(clr);
  SDL_RenderDrawLine(sdlRenderer_, p0.x, p0.y, p1.x, p1.y);
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

} //Rendering

