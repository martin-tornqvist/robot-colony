#ifndef CMN_TYPES_H
#define CMN_TYPES_H

#include <string>
#include <algorithm>
#include <assert.h>

#include "CmnData.h"
#include "colors.h"

class Pos2 {
public:
  Pos2() : x(0), y(0) {}
  Pos2(const int x_, const int y_) : x(x_), y(y_) {}
  Pos2(const Pos2& p) : x(p.x), y(p.y) {}

  Pos2& operator*=(const int V)     {x *= V; y *= V;      return *this;}
  Pos2& operator/=(const int V)     {x /= V; y /= V;      return *this;}
  Pos2& operator+=(const Pos2& o)   {x += o.x; y += o.y;  return *this;}
  Pos2& operator-=(const Pos2& o)   {x -= o.x; y -= o.y;  return *this;}

  Pos2 operator+(const Pos2& p)     const {return Pos2(x + p.x, y + p.y);}
  Pos2 operator+(const int V)       const {return Pos2(x + V, y + V);}
  Pos2 operator-(const Pos2& p)     const {return Pos2(x - p.x, y - p.y);}
  Pos2 operator-(const int V)       const {return Pos2(x - V, y - V);}
  Pos2 operator/(const int V)       const {return Pos2(x / V, y / V);}
  Pos2 operator*(const int V)       const {return Pos2(x * V, y * V);}
  Pos2 operator*(const Pos2& p)     const {return Pos2(x * p.x, y * p.y);}
  bool operator==(const Pos2& p)    const {return x == p.x && y == p.y;}
  bool operator!=(const Pos2& p)    const {return x != p.x || y != p.y;}
  bool operator<=(const Pos2& p)    const {return x <= p.x && y <= p.y;}
  bool operator>=(const Pos2& p)    const {return x >= p.x && y >= p.y;}

  Pos2 getSigns() const {
    return Pos2(x == 0 ? 0 : x > 0 ? 1 : -1,
                y == 0 ? 0 : y > 0 ? 1 : -1);
  }

  void set(const int x_, const int y_)  {x = x_; y = y_;}
  void set(const Pos2& p)                {x = p.x; y = p.y;}

  void swap(Pos2& p) {Pos2 pTemp(p); p.set(*this); set(pTemp);}

  int x, y;
};

class Pos3 {
public:
  Pos3() : x(0), y(0), z(0) {}
  Pos3(const double X, const double Y, const double Z) : x(X), y(Y), z(Z) {}
  Pos3(const Pos3& p) : x(p.x), y(p.y), z(p.z) {}

  double x, y, z;
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
