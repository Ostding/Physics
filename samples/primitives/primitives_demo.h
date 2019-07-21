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
  virtual void onKeyboardPress(unsigned char key);
  virtual void onUpdate();
  virtual void onMousePress(int button, int state, int x, int y);
  virtual void onMouseMove(int x, int y);

private:
  void render();
  void doTest();

private:
  bool simulate;
  World *world;
  
  typedef struct point
  {
    int x;
    int y;
  } Point;
  Point lastPoint;
  bool lBtnDown;
  float radY;
  float radP;
  float lookDist;
};

#endif