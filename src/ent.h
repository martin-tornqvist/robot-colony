#ifndef ENT_H
#define ENT_H

#include "cmnTypes.h"

enum class EntType {generic, rbt, rechargeStation};

class Ent {
public:
  Ent() {}

  virtual ~Ent() {}

  virtual GlyphAndClr getGlyphAndClr() const = 0;

  virtual EntType getEntType() const = 0;
};

class Rigid : public Ent {
public:
  Rigid() : Ent() {}

  virtual ~Rigid() {}

  virtual bool isBlocking() const = 0;
};

class RockGround : public Rigid {
public:
  RockGround() : Rigid() {}

  ~RockGround() {}

  bool isBlocking() const override {return false;}

  GlyphAndClr getGlyphAndClr() const override {
    return GlyphAndClr('.', clrBrownXDrk);
  }

  EntType getEntType() const override {return EntType::generic;}
};

class RockWall : public Rigid {
public:
  RockWall() : Rigid() {}

  ~RockWall() {}

  bool isBlocking() const override {return true;}

  GlyphAndClr getGlyphAndClr() const override {
    return GlyphAndClr(10, clrBrownXDrk, clrGray);
  }

  EntType getEntType() const override {return EntType::generic;}
};

//Note: Assemblies
class Asm : public Rigid {
public:
  Asm() : Rigid() {}

};

class RechargeStation : public Asm {
public:
  RechargeStation() : Asm() {}

  GlyphAndClr getGlyphAndClr() const override {
    return GlyphAndClr('%', clrYellow, clrGray);
  }

  EntType getEntType() const override {return EntType::rechargeStation;}

  bool isBlocking() const override {return false;}
};

class Mob : public Ent {
public:
  Mob() : Ent(), p_(P()) {}
  Mob(const P& p) : Ent(), p_(p) {}

  virtual ~Mob() {}

  virtual void onTick() {}

  P getPos() const {return p_;}

  void tryStepTowards(const P& p);

  virtual void tryBuild(const P& p) {(void)p;}

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

  EntType getEntType() const override {return EntType::rbt;}

  void onTick();

  void tryBuild(const P& p) override;

  int getPwrCur() const {return pwrCur_;}
  int getPwrMax() const {return pwrMax_;}

private:
  bool canStep() const override;

  void onStepped() override;

  int pwrMax_ = 80;
  int pwrCur_ = pwrMax_;
};

#endif // ENT_H
