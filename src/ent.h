#ifndef ENT_H
#define ENT_H

#include "cmnTypes.h"

enum class EntType {genericRigid, assembly, rbt};

class Ent {
public:
  Ent() {}

  virtual ~Ent() {}

  virtual GlyphAndClr getGlyphAndClr() const = 0;

  virtual EntType getEntType() const = 0;

  virtual void onTick() {}
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

  EntType getEntType() const override {return EntType::genericRigid;}
};

class RockWall : public Rigid {
public:
  RockWall() : Rigid() {}

  ~RockWall() {}

  bool isBlocking() const override {return true;}

  GlyphAndClr getGlyphAndClr() const override {
    return GlyphAndClr(10, clrBrownXDrk, clrGray);
  }

  EntType getEntType() const override {return EntType::genericRigid;}
};

//Note: Assemblies
class Asm : public Rigid {
public:
  Asm() : Rigid() {}

  void onTick() override {isBeingBuiltThisTick_ = false;}

  void tickBuild();

  bool isFinished() const {return ticksBuilt_ >= getNrTicksToBuild();}

  GlyphAndClr getGlyphAndClr() const override final;

protected:
  virtual int getNrTicksToBuild() const = 0;

  virtual GlyphAndClr getGlyphAndClr_() const = 0;

  bool isBeingBuiltThisTick_ = false;

  int ticksBuilt_ = 0;
};

class RechargeStation : public Asm {
public:
  RechargeStation() : Asm() {}

  EntType getEntType() const override {return EntType::assembly;}

  bool isBlocking() const override {return false;}

protected:
  GlyphAndClr getGlyphAndClr_() const override {
    return GlyphAndClr('+', clrYellow, clrBlack);
  }

  int getNrTicksToBuild() const override {return 8;}
};

class Mob : public Ent {
public:
  Mob() : Ent(), p_(P()) {}
  Mob(const P& p) : Ent(), p_(p) {}

  virtual ~Mob() {}

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
