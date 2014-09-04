#include "utils.h"

#include <math.h>

#include "MersenneTwister.h"

using namespace std;

namespace Utils {

bool isInRange(const Pos2& p0, const Pos2& p1, const int RANGE) {
  const float DX        = p1.x - p0.x;
  const float DY        = p1.y - p0.y;
  const float LINE_LEN  = sqrt((DX * DX) + (DY * DY));
  return int(LINE_LEN) <= RANGE;
}

void getPointOnLine(const Pos2& p0, const Pos2& p1, const float DIST,
                    Pos2& pRef) {
  if(DIST < 0) {pRef = p0; return;}

  const float DX = p1.x - p0.x;
  const float DY = p1.y - p0.y;

  const float LINE_LEN = sqrt((DX * DX) + (DY * DY));

  if(DIST > LINE_LEN) {pRef = p1; return;}

  pRef.set(int((float(p0.x) + (DX / LINE_LEN) * DIST)),
           int((float(p0.y) + (DY / LINE_LEN) * DIST)));
}

bool isInArea(const Pos2& p, const Pos2& areaP0, const Pos2& areaP1) {
  return(p >= areaP0 && p <= areaP1);
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

Dir getDir(const Pos2& offset) {
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

void getOffset(const Dir dir, Pos2& offsetRef) {
  assert(dir != Dir::endOfDirs);

  switch(dir) {
    case Dir::downLeft:   offsetRef = Pos2(-1, 1);   break;
    case Dir::down:       offsetRef = Pos2(0, 1);    break;
    case Dir::downRight:  offsetRef = Pos2(1, 1);    break;
    case Dir::left:       offsetRef = Pos2(-1, 0);   break;
    case Dir::center:     offsetRef = Pos2(0, 0);    break;
    case Dir::right:      offsetRef = Pos2(1, 0);    break;
    case Dir::upLeft:     offsetRef = Pos2(-1, -1);  break;
    case Dir::up:         offsetRef = Pos2(0, -1);   break;
    case Dir::upRight:    offsetRef = Pos2(1, -1);   break;
    case Dir::endOfDirs:  offsetRef = Pos2(0, 0);    break;
  }
}

void getAdjPos(const Dir dir, const Pos2& pos, Pos2& adjPosRef) {
  Pos2 offset;
  getOffset(dir, offset);
  adjPosRef = pos + offset;
}

Dir getDir(const Pos2& p0, const Pos2& p1) {
  if(p0 == p1) {return Dir::center;}

  const Pos2  offset(p1 - p0);
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
