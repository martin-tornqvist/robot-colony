#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "cmnTypes.h"

namespace Utils {

bool isInRange(const Pos2& p0, const Pos2& p1, const int RANGE);

//Note: This function is guaranteed to only return points on the line.
//* Returns p0 if DIST is negative.
//* Returns p1 if DIST is beyond p1
void getPointOnLine(const Pos2& p0, const Pos2& p1, const float DIST,
                    Pos2& pRef);

bool isInArea(const Pos2& p, const Pos2& areaP0, const Pos2& areaP1);

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

Dir getDir(const Pos2& offset);

Dir getDir(const Pos2& p0, const Pos2& p1);

void getOffset(const Dir dir, Pos2& offsetRef);

void getAdjPos(const Dir dir, const Pos2& pos, Pos2& adjPosRef);

} //DirUtils

#endif
