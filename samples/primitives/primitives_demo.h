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
  void initTest();
  void initOneSphere(ffloat radius, const Vector3 &pos, ffloat mass);
  void initOnePlane(const Vector3 &dir, const Vector3 &extents, const ffloat &offset);
  void initOneBox(const Vector3 &pos, const Vector3 &extents, ffloat mass);
  void initOnePolyHedron(const Vector3 &pos, ffloat mass, const Vector3 &angles);

private:
  bool simulate;
  bool started;
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
  float eX, eY, eZ;
  float dX, dY, dZ;
};

#endif