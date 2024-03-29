#ifndef __FIXPOINT_DEMO_H__
#define __FIXPOINT_DEMO_H__

#include "application.h"
#include "fixed_float.h"
using namespace physics;

class FixPointDemo : public Application
{
public:
  FixPointDemo(const char *title, int width, int height);

public:
  virtual void onDisplay();
  virtual void onKeyboardDown(unsigned char key);

private:
  void doTestFixedFloat();
  void doTestSpeed();

  void doTestVector3();
  void doTestQuaternion();
  void doTestMatrix3();
  void doTestMatrix4();
};

#endif