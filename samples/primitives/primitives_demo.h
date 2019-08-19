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
  virtual void onKeyboardDown(unsigned char key);
  virtual void onKeyboardUp(unsigned char key);
  virtual void onFixedUpdate(double duration);
  virtual void onMousePress(int button, int state, int x, int y);
  virtual void onMouseMove(int x, int y);

private:
  void render();
  void initTest();
  Sphere * initOneSphere(const ffloat &radius, const Vector3 &pos, const ffloat &mass);
  Plane * initOnePlane(const Vector3 &dir, const Vector3 &extents, const ffloat &offset);
  Box * initOneBox(const Vector3 &pos, const Vector3 &extents, const Vector3 &angles, const ffloat &mass);
  Polyhedron * initOnePolyHedron(const Vector3 &pos, const ffloat &mass, const Vector3 &angles);
  Capsule * initOneCapsule(const Vector3 &pos, const ffloat &radius, const ffloat &halfHeight, const ffloat &mass, const Vector3 &angles);

  void setMoveAcc(unsigned char key);
  void setCameraPos(unsigned char key);
  void setMoveVelocity(unsigned char key);
  GravityForce * genGravityForce();

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
  
  float cameraStep;
  float radY;
  float radP;

  float lookDist;
  float eX, eY, eZ;
  float dX, dY, dZ;

  enum WASD_MODE
  {
    CAMERA = 0,
    MOVE = 1,
    VELOCITY = 2,
  };
  WASD_MODE wasdMode;

  Sphere *moveSphere;
  unsigned int moveID;
  ffloat acc;

  ffloat velocity;
  
  ffloat deltaTime;
};

#endif