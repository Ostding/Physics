#ifndef __PRIMITIVES_DEMO_H__
#define __PRIMITIVES_DEMO_H__

#include "application.h"
#include "physics.h"
using namespace physics;

class PrimitivesDemo : public Application
{
public:
  PrimitivesDemo(const char *title, int width, int height);
  ~PrimitivesDemo();

public:
  virtual void onDisplay();
  void onKeyboardPress(unsigned char key);
  virtual void onUpdate();

private:
  void render();
  void doTest();

private:
  World *world;
};

#endif