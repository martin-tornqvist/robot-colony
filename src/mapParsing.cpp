#include "mapParsing.h"

#include <climits>

#include "utils.h"

using namespace std;

//------------------------------------------------------------ FLOOD FILL
namespace FloodFill {

void run(const P& p0, bool blocked[MAP_W][MAP_H],
         int out[MAP_W][MAP_H], int travelLimit, const P& p1,
         const bool ALLOW_DIAGONAL) {

  Utils::resetArray(out);

  vector<P> positions;
  positions.resize(0);

  size_t  nrElementsToSkip  = 0;
  int     curVal            = 0;
  bool    pathExists        = true;
  bool    isAtTarget        = false;
  bool    isStoppingAtP1    = p1.x != -1;
  const   Rect bounds(P(1, 1), P(MAP_W - 2, MAP_H - 2));
  P       curPos(p0);

  vector<P> dirs {P(0, -1), P(-1, 0), P(0, 1), P(1, 0)};
  if(ALLOW_DIAGONAL) {
    dirs.push_back(P(-1, -1));
    dirs.push_back(P(-1, 1));
    dirs.push_back(P(1, -1));
    dirs.push_back(P(1, 1));
  }

  bool done = false;
  while(!done) {

    for(const P& d : dirs) {
      if((d != 0)) {
        const P newPos(curPos + d);
        if(
          !blocked[newPos.x][newPos.y]    &&
          Utils::isPosIn(newPos, bounds)  &&
          out[newPos.x][newPos.y] == 0    &&
          newPos != p0) {
          curVal = out[curPos.x][curPos.y];

          if(curVal < travelLimit) {out[newPos.x][newPos.y] = curVal + 1;}

          if(isStoppingAtP1 && curPos == p1 - d) {
            isAtTarget = true;
            break;
          }
          if(!isStoppingAtP1 || !isAtTarget) {positions.push_back(newPos);}
        }
      }
    }

    if(isStoppingAtP1) {
      if(positions.size() == nrElementsToSkip)  {pathExists = false;}
      if(isAtTarget || !pathExists)             {done = true;}
    } else if(positions.size() == nrElementsToSkip) {
      done = true;
    }

    if(curVal == travelLimit) {done = true;}

    if(!isStoppingAtP1 || !isAtTarget) {
      if(positions.size() == nrElementsToSkip) {
        pathExists = false;
      } else {
        curPos = positions.at(nrElementsToSkip);
        nrElementsToSkip++;
      }
    }
  }
}

} //FloodFill

//------------------------------------------------------------ PATHFINDER
namespace PathFind {

void run(const P& p0, const P& p1, bool blocked[MAP_W][MAP_H],
         vector<P>& out, const bool ALLOW_DIAGONAL) {

  out.resize(0);

  int flood[MAP_W][MAP_H];
  FloodFill::run(p0, blocked, flood, INT_MAX, p1, ALLOW_DIAGONAL);

  if(flood[p1.x][p1.y] == 0) {return;} //No path exists

  P curPos(p1);
  out.push_back(curPos);

  vector<P> dirs {P(0, -1), P(-1, 0), P(0, 1), P(1, 0)};
  if(ALLOW_DIAGONAL) {
    dirs.push_back(P(-1, -1));
    dirs.push_back(P(-1, 1));
    dirs.push_back(P(1, -1));
    dirs.push_back(P(1, 1));
  }

  while(true) {
    for(const P& d : dirs) {

      const P adjPos(curPos + d);

      if(Utils::isPosInMap(adjPos)) {
        const int VAL_AT_ADJ = flood[adjPos.x][adjPos.y];
        const int VAL_AT_CUR = flood[curPos.x][curPos.y];
        if(
          (VAL_AT_ADJ < VAL_AT_CUR && VAL_AT_ADJ != 0) ||
          (adjPos == p0)) {

          if(adjPos == p0) {return;} //Origin reached

          out.push_back(adjPos);

          curPos = adjPos;

          break;
        }
      }
    }
  }
}

} //PathFind
