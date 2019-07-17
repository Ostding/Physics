#ifndef __PRIMITIVES_DEMO_H__
#define __PRIMITIVES_DEMO_H__

#include "application.h"
#include "fixed_float.h"
using namespace physics;

class PrimitivesDemo : public Application
{
public:
  PrimitivesDemo(const char *title, int width, int height);

public:
  virtual void onDisplay();
  void onKeyboardPress(unsigned char key);

private:
  void render();
  void doTest();
};

#endif