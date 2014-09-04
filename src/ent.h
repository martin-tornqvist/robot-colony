#ifndef ENT_H
#define ENT_H

class Ent {
public:
  Ent() : p_(P()) {}
  Ent(const P& p) : p_(p) {}

private:
  P p_;
};

class Robot : public Ent {
public:
  Robot() : Ent() {}
  Robot(const P& p) : Ent(p) {}

};

#endif // ENT_H
