#ifndef ENT_H
#define ENT_H

#include "cmnTypes.h"

class Ent {
public:
  Ent() {}

  virtual ~Ent() {}

  virtual GlyphAndClr getGlyphAndClr() const = 0;
};

class Rigid : public Ent {
public:
  Rigid() : Ent() {}

  virtual ~Rigid() {}

  virtual bool isBlocking() = 0;
};

class Ground : public Ent {
public:
  Ground() : Ent() {}

  virtual ~Ground() {}
};

class RockGround : public Ground {
public:
  RockGround() : Ground() {}

  ~RockGround() {}

  bool isBlocking() {return false;}

  GlyphAndClr getGlyphAndClr() const override;
};

class Mob : public Ent {
public:
  Mob() : Ent(), p_(P()) {}
  Mob(const P& p) : Ent(), p_(p) {}

  virtual ~Mob() {}

  P getPos() const {return p_;}

  void tryStepTowards(const P& p);

  bool hasActed_ = false;

protected:
  virtual bool canStep() const {return true;}
  virtual void onStepped() {}

  P p_;
};

class Rbt : public Mob {
public:
  Rbt() : Mob() {}
  Rbt(const P& p) : Mob(p) {}

  GlyphAndClr getGlyphAndClr() const override;

private:
  bool canStep() const override;

  void onStepped() override;

  int pwrMax_ = 80;
  int pwrCur_ = pwrMax_;
};

#endif // ENT_H
