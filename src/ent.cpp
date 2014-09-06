#include "ent.h"

#include "mapParsing.h"
#include "utils.h"
#include "rendering.h"

using namespace std;

//------------------------------------------------------------------- MOBILE
void Mob::tryStepTowards(const P& p) {
  if(!hasActed_ && p != p_ && Utils::isPosInMap(p, true)) {

    bool blocked[MAP_W][MAP_H];
    Utils::resetArray(blocked, false);

    vector<P> path;

    PathFind::run(p_, p, blocked, path);

    if(!path.empty()) {
      p_ = path.back();

      hasActed_ = true;
    }
  }
}

//------------------------------------------------------------------- ROBOT
void Rbt::draw() const {
  Rendering::drawGlypInMap('R', p_, clrWhite);
}
