#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "cmnTypes.h"

namespace Utils {

bool isInRange(const P& p0, const P& p1, const int RANGE);

//Note: This function is guaranteed to only return points on the line.
//* Returns p0 if DIST is negative.
//* Returns p1 if DIST is beyond p1
void getPointOnLine(const P& p0, const P& p1, const float DIST, P& pRef);

bool isInArea(const P& p, const P& areaP0, const P& areaP1);

void resetArray(int a[MAP_W][MAP_H]);

void resetArray(bool a[MAP_W][MAP_H], const bool VAL);

template<typename T> void resetArray(T* a[MAP_W][MAP_H]) {
  for(int y = 0; y < MAP_H; ++y) {
    for(int x = 0; x < MAP_W; ++x) {
      a[x][y] = nullptr;
    }
  }
}

void reverseBoolArray(bool a[MAP_W][MAP_H]);

bool isPosInMap(const P& pos, const bool COUNT_EDGE_AS_INSIDE = true);

bool isPosIn(const P& pos, const Rect& area);

bool isAreaInOther(const Rect& inner, const Rect& outer,
                       const bool COUNT_EQUAL_AS_INSIDE);

bool isAreaInMap(const Rect& area);

bool isPosAdj(const P& pos1, const P& pos2, const bool COUNT_SAME_CELL_AS_ADJ);

int kingDist(const int X0, const int Y0, const int X1, const int Y1);
int kingDist(const P& p0, const P& p1);

bool isValInRange(const int VAL, const Range& range);


} //Utils

namespace Rnd {

//Note: If MTRand is not provided any parameters to the constructor, it will be
//seeded with current time. So seeding it manually is not necessary for normal
//gameplay purposes - only if seed should be controlled for testing purposes,
//or recreating a certain level, etc.
void seed(const unsigned long val);

int dice(const int ROLLS, const int SIDES);

int dice(const DiceParam& p);

bool coin();

bool fraction(const int NUMERATOR, const int DENOMINATOR);

bool oneIn(const int N);

int range(const int MIN, const int MAX);

int range(const Range& valueRange);

int percentile();

} //Rnd

namespace DirUtils {

Dir getDir(const P& offset);

Dir getDir(const P& p0, const P& p1);

void getOffset(const Dir dir, P& offsetRef);

void getAdjPos(const Dir dir, const P& pos, P& adjPosRef);

} //DirUtils

#endif
