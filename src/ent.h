#ifndef ENT_H
#define ENT_H

#include "cmnTypes.h"

class Ent {
public:
  Ent() : p_(P()) {}
  Ent(const P& p) : p_(p) {}

  virtual ~Ent() {}

  virtual void draw() const = 0;

protected:
  P p_;
};

class Mob : public Ent {
public:
  Mob() : Ent() {}
  Mob(const P& p) : Ent(p) {}

  virtual ~Mob() {}

  void tryStepTowards(const P& p);

  bool hasActed_ = false;
};

class Rbt : public Mob {
public:
  Rbt() : Mob() {}
  Rbt(const P& p) : Mob(p) {}

  void draw() const override;
};

#endif // ENT_H
