#include "utils.h"

#include <math.h>

#include "MersenneTwister.h"

using namespace std;

namespace Utils {

bool isInRange(const P& p0, const P& p1, const int RANGE) {
  const float DX        = p1.x - p0.x;
  const float DY        = p1.y - p0.y;
  const float LINE_LEN  = sqrt((DX * DX) + (DY * DY));
  return int(LINE_LEN) <= RANGE;
}

void getPointOnLine(const P& p0, const P& p1, const float DIST, P& pRef) {
  if(DIST < 0) {pRef = p0; return;}

  const float DX = p1.x - p0.x;
  const float DY = p1.y - p0.y;

  const float LINE_LEN = sqrt((DX * DX) + (DY * DY));

  if(DIST > LINE_LEN) {pRef = p1; return;}

  pRef.set(int((float(p0.x) + (DX / LINE_LEN) * DIST)),
           int((float(p0.y) + (DY / LINE_LEN) * DIST)));
}

bool isInArea(const P& p, const P& areaP0, const P& areaP1) {
  return(p >= areaP0 && p <= areaP1);
}

void resetArray(int a[MAP_W][MAP_H]) {
  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      a[x][y] = 0;
    }
  }
}

void resetArray(bool a[MAP_W][MAP_H], const bool VAL) {
  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      a[x][y] = VAL;
    }
  }
}

void reverseBoolArray(bool a[MAP_W][MAP_H]) {
  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      a[x][y] = !a[x][y];
    }
  }
}

bool isPosInMap(const P& pos, const bool COUNT_EDGE_AS_INSIDE) {
  if(COUNT_EDGE_AS_INSIDE) {
    return pos.x >= 0 && pos.y >= 0 && pos.x < MAP_W && pos.y < MAP_H;
  } else {
    return pos.x > 0 && pos.y > 0 && pos.x < MAP_W - 1 && pos.y < MAP_H - 1;
  }
}

bool isPosIn(const P& pos, const Rect& area) {
  return
    pos.x >= area.p0.x &&
    pos.x <= area.p1.x &&
    pos.y >= area.p0.y &&
    pos.y <= area.p1.y;
}

bool isAreaInOther(const Rect& inner, const Rect& outer,
                   const bool COUNT_EQUAL_AS_INSIDE) {
  if(COUNT_EQUAL_AS_INSIDE) {
    return
      inner.p0.x >= outer.p0.x &&
      inner.p1.x <= outer.p1.x &&
      inner.p0.y >= outer.p0.y &&
      inner.p1.y <= outer.p1.y;
  } else {
    return
      inner.p0.x > outer.p0.x &&
      inner.p1.x < outer.p1.x &&
      inner.p0.y > outer.p0.y &&
      inner.p1.y < outer.p1.y;
  }
}

bool isAreaInMap(const Rect& area) {
  return isPosInMap(area.p0) && isPosInMap(area.p1);
}

bool isPosAdj(const P& pos1, const P& pos2, const bool COUNT_SAME_CELL_AS_ADJ) {
  if(pos1.x == pos2.x && pos1.y == pos2.y)  {return COUNT_SAME_CELL_AS_ADJ;}
  if(pos1.x < pos2.x - 1)                   {return false;}
  if(pos1.x > pos2.x + 1)                   {return false;}
  if(pos1.y < pos2.y - 1)                   {return false;}
  if(pos1.y > pos2.y + 1)                   {return false;}
  return true;
}

int kingDist(const int X0, const int Y0, const int X1, const int Y1) {
  return max(abs(X1 - X0), abs(Y1 - Y0));
}

int kingDist(const P& p0, const P& p1) {
  return max(abs(p1.x - p0.x), abs(p1.y - p0.y));
}

bool isValInRange(const int VAL, const Range& range) {
  if(VAL < range.lower) {return false;}
  if(VAL > range.upper) {return false;}
  return true;
}

} //Utils

namespace Rnd {

namespace {

MTRand mtRand;

int roll(const int ROLLS, const int SIDES) {
  if(SIDES <= 0) {return 0;}
  if(SIDES == 1) {return ROLLS * SIDES;}

  int result = 0;
  for(int i = 0; i < ROLLS; i++) {
    result += mtRand.randInt(SIDES - 1) + 1;
  }
  return result;
}

} //Namespace

void seed(const unsigned long val) {mtRand = MTRand(val);}

int dice(const int ROLLS, const int SIDES) {return roll(ROLLS, SIDES);}

int dice(const DiceParam& p) {return roll(p.rolls, p.sides);}

bool coin() {return roll(1, 2) == 2;}

bool fraction(const int NUMERATOR, const int DENOMINATOR) {
  return roll(1, DENOMINATOR) <= NUMERATOR;
}

bool oneIn(const int N) {return fraction(1, N);}

int range(const int MIN, const int MAX) {
  return MIN + roll(1, MAX - MIN + 1) - 1;
}

int range(const Range& valueRange) {
  return range(valueRange.lower, valueRange.upper);
}

int percentile() {return roll(1, 100);}

} //Rnd

namespace DirUtils {

namespace {

MTRand mtRand;

const string compassDirNames[3][3] = {
  {"NW", "N", "NE"},
  { "W",  "",  "E",},
  {"SW", "S", "SE"}
};

const float PI_DB            = 3.14159265;
const float ANGLE_45_DB      = 2 * PI_DB / 8;
const float ANGLE_45_HALF_DB = ANGLE_45_DB / 2.0;

const float edge[4] = {
  ANGLE_45_HALF_DB + (ANGLE_45_DB * 0),
  ANGLE_45_HALF_DB + (ANGLE_45_DB * 1),
  ANGLE_45_HALF_DB + (ANGLE_45_DB * 2),
  ANGLE_45_HALF_DB + (ANGLE_45_DB * 3)
};

} //namespace

Dir getDir(const P& offset) {
  assert(offset.x >= -1 && offset.y >= -1 && offset.x <= 1 && offset.y <= 1);

  if(offset.y == -1) {
    return offset.x == -1 ? Dir::upLeft :
           offset.x ==  0 ? Dir::up :
           offset.x ==  1 ? Dir::upRight :
           Dir::endOfDirs;
  }

  if(offset.y == 0) {
    return offset.x == -1 ? Dir::left :
           offset.x ==  0 ? Dir::center :
           offset.x ==  1 ? Dir::right :
           Dir::endOfDirs;
  }
  if(offset.y == 1) {
    return offset.x == -1 ? Dir::downLeft :
           offset.x ==  0 ? Dir::down :
           offset.x ==  1 ? Dir::downRight :
           Dir::endOfDirs;
  }
  return Dir::endOfDirs;
}

void getOffset(const Dir dir, P& offsetRef) {
  assert(dir != Dir::endOfDirs);

  switch(dir) {
    case Dir::downLeft:   offsetRef = P(-1, 1);   break;
    case Dir::down:       offsetRef = P(0, 1);    break;
    case Dir::downRight:  offsetRef = P(1, 1);    break;
    case Dir::left:       offsetRef = P(-1, 0);   break;
    case Dir::center:     offsetRef = P(0, 0);    break;
    case Dir::right:      offsetRef = P(1, 0);    break;
    case Dir::upLeft:     offsetRef = P(-1, -1);  break;
    case Dir::up:         offsetRef = P(0, -1);   break;
    case Dir::upRight:    offsetRef = P(1, -1);   break;
    case Dir::endOfDirs:  offsetRef = P(0, 0);    break;
  }
}

void getAdjPos(const Dir dir, const P& pos, P& adjPosRef) {
  P offset;
  getOffset(dir, offset);
  adjPosRef = pos + offset;
}

Dir getDir(const P& p0, const P& p1) {
  if(p0 == p1) {return Dir::center;}

  const P  offset(p1 - p0);
  const float ANGLE_DB = atan2(-offset.y, offset.x);

  if(ANGLE_DB        <  -edge[2] && ANGLE_DB >  -edge[3]) {
    return Dir::downLeft;
  } else if(ANGLE_DB <= -edge[1] && ANGLE_DB >= -edge[2]) {
    return Dir::down;
  } else if(ANGLE_DB <  -edge[0] && ANGLE_DB >  -edge[1]) {
    return Dir::downRight;
  } else if(ANGLE_DB >= -edge[0] && ANGLE_DB <=  edge[0]) {
    return Dir::right;
  } else if(ANGLE_DB >   edge[0] && ANGLE_DB <   edge[1]) {
    return Dir::upRight;
  } else if(ANGLE_DB >=  edge[1] && ANGLE_DB <=  edge[2]) {
    return Dir::up;
  } else if(ANGLE_DB >   edge[2] && ANGLE_DB <   edge[3]) {
    return Dir::upLeft;
  } else {
    return Dir::left;
  }
}

} //DirUtils
