#ifndef CMN_TYPES_H
#define CMN_TYPES_H

#include <string>
#include <algorithm>
#include <assert.h>

#include "CmnData.h"
#include "colors.h"

struct P {
  P() : x(0), y(0) {}
  P(const int X, const int Y) : x(X), y(Y) {}
  P(const P& p) : x(p.x), y(p.y) {}
  P(const int V) : x(V), y(V) {}

  P& operator=(const P& p) {x = p.x; y = p.y; return *this;}

  P& operator/=(const int  V) {x /= V;   y /= V;   return *this;}
  P& operator+=(const P& o)   {x += o.x; y += o.y; return *this;}
  P& operator-=(const P& o)   {x -= o.x; y -= o.y; return *this;}
  P operator+(const P& p)         const {return P(x + p.x,  y + p.y);}
  P operator+(const int V)        const {return P(x + V,    y + V);}
  P operator-(const P& p)         const {return P(x - p.x,  y - p.y);}
  P operator-(const int V)        const {return P(x - V,    y - V);}
  P operator/(const int V)        const {return P(x / V,    y / V);}
  P operator*(const int V)        const {return P(x * V,    y * V);}
  P operator*(const P& p)         const {return P(x * p.x,  y * p.y);}
  bool operator==(const P& p)     const {return x == p.x  && y == p.y;}
  bool operator!=(const P& p)     const {return x != p.x  || y != p.y;}
  bool operator!=(const int V)    const {return x != V    || y != V;}
  bool operator>(const P& p)      const {return x > p.x   && y > p.y;}
  bool operator>(const int  V)    const {return x > V     && y > V;}
  bool operator<(const P& p)      const {return x < p.x   && y < p.y;}
  bool operator<(const int  V)    const {return x < V     && y < V;}
  bool operator>=(const P&  p)    const {return x >= p.x  && y >= p.y;}
  bool operator>=(const int   V)  const {return x >= V    && y >= V;}
  bool operator<=(const P&  p)    const {return x <= p.x  && y <= p.y;}
  bool operator<=(const int   V)  const {return x <= V    && y <= V;}

  P getSigns() const {
    return P(x == 0 ? 0 : x > 0 ? 1 : -1,
             y == 0 ? 0 : y > 0 ? 1 : -1);
  }

  void set(const int x_, const int y_)  {x = x_;  y = y_;}
  void set(const P& p)                  {x = p.x; y = p.y;}

  void swap(P& p) {P pTemp(p); p = *this; set(pTemp);}

  int x, y;
};

struct Rect {
  Rect() : p0(P()), p1(P()) {}

  Rect(const P& p0_, const P& p1_) :  p0(p0_), p1(p1_) {}

  Rect(const int X0, const int Y0, const int X1, const int Y1) :
    p0(P(X0, Y0)), p1(P(X1, Y1)) {}

  Rect(const Rect& r) : p0(r.p0), p1(r.p1) {}

  P p0;
  P p1;
};

struct GlyphAndClr {
  GlyphAndClr() : glyph(0), clr(clrBlack), clrBg(clrBlack) {}

  GlyphAndClr(char glyph_, const Clr& clr_) :
    glyph(glyph_), clr(clr_), clrBg(clrBlack) {}

  GlyphAndClr(char glyph_, const Clr& clr_, const Clr& clrBg_) :
    glyph(glyph_), clr(clr_), clrBg(clrBg_) {}

  GlyphAndClr& operator=(const GlyphAndClr& other) {
    glyph = other.glyph;
    clr   = other.clr;
    clrBg = other.clrBg;
    return *this;
  }

  char glyph;
  Clr clr;
  Clr clrBg;
};

struct StrAndClr {
  StrAndClr() : str(""), clr(clrBlack) {}

  StrAndClr(const std::string& text, const Clr& color) :
    str(text), clr(color) {
  }

  StrAndClr& operator=(const StrAndClr& other) {
    str = other.str;
    clr = other.clr;
    return *this;
  }

  std::string str;
  Clr clr;
};

struct DiceParam {
public:
  DiceParam() : rolls(1), sides(100), plus(0) {}

  DiceParam(const int ROLLS, const int SIDES, const int PLUS = 0) :
    rolls(ROLLS), sides(SIDES), plus(PLUS) {}

  DiceParam(const DiceParam& other) :
    rolls(other.rolls), sides(other.sides), plus(other.plus) {}

  DiceParam& operator=(const DiceParam& other) {
    rolls = other.rolls;
    sides = other.sides;
    plus  = other.plus;
    return *this;
  }

  inline int getHighest() const {return (rolls * sides) + plus;}

  int rolls, sides, plus;
};

struct Range {
  Range() : lower(-1), upper(-1) {}

  Range(const int LOWER, const int UPPER) :
    lower(LOWER), upper(UPPER) {}

  int lower, upper;
};

enum class Dir {
  downLeft   = 1,
  down       = 2,
  downRight  = 3,
  left       = 4,
  center     = 5,
  right      = 6,
  upLeft     = 7,
  up         = 8,
  upRight    = 9,
  endOfDirs  = 10
};

#endif
