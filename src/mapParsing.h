#ifndef MAP_PARSING_H
#define MAP_PARSING_H

#include <vector>

#include "cmnData.h"

class P;

namespace FloodFill {

void run(const P& p0, bool blocked[MAP_W][MAP_H], int out[MAP_W][MAP_H], int travelLmt,
         const P& p1, const bool ALLOW_DIAGONAL);

} //FloodFill

namespace PathFind {

//Note: The resulting path does not include the origin
void run(const P& p0, const P& p1, bool blocked[MAP_W][MAP_H], std::vector<P>& out,
         const bool ALLOW_DIAGONAL = true);

} //PathFind


#endif
