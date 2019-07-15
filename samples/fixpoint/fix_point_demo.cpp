#include "fix_point_demo.h"
#include "test_fixed_float.h"
#include "physics.h"

FixPointDemo::FixPointDemo(const char *title, int width, int height)
:Application(title, width, height)
{
}

void FixPointDemo::onDisplay()
{
   glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glLoadIdentity();
   gluLookAt(5.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f);
   
   glColor3f(0.0f, 0.0f, 0.0f);
   
   textOut(10.0f, 34.0f, "Physic Demo: Test Fraction \n \
      Press 'g' to run demo and watch console for fraction calculation; \n \
      Press 'q' to quite sample application;");
}

FixedFloat testFunc()
{
  return 123LL;
}

void FixPointDemo::onKeyboardPress(unsigned char key)
{
  switch( key ) {
  case 'g': case 'G':
    doTestFixedFloat();
    // doTestSpeed();

    doTestVector3();
    doTestQuaternion();
    break;
  }

  Application::onKeyboardPress(key);
}


void FixPointDemo::doTestSpeed()
{
  TestFixedFloat::doTestSpeed();
}

void FixPointDemo::doTestFixedFloat()
{
  TestFixedFloat::doTest();
}

void FixPointDemo::doTestQuaternion()
{
   myPrintf("/////////test Quaternion/////// \n");
   Vector3 angles = Vector3(ffloat(30), ffloat(60), ffloat(45));
   Quaternion a = Quaternion::fromEulerAngles(angles);
   a.inspect("Quaternion::fromEulerAngles(Vector3(ffloat(30), ffloat(60), ffloat(45))");
   Vector3 ret = a.toEulerAngles();
   ret.inspect("a.toEulerAngles()");

   angles = Vector3(ffzero, ffloat(90), ffzero);
   Quaternion q = Quaternion::fromEulerAngles(angles);
   Vector3 v = Vector3(ffzero, ffzero, ffloat(-10));
   v = q * v;
   v.inspect("q * v");
}

void FixPointDemo::doTestVector3()
{
  myPrintf("/////////test Vector3///////");
  Vector3 a = Vector3(1,1,1);
  a.inspect("Vector3(1,1,1)");
  a = a.scale(ffloat(2));
  a.inspect("a.scale(ffloat(2))");
  a.scaleUpdate(ffloat(2));
  a.inspect("a.scaleUpdate(2)");
  Vector3 b = Vector3(2,2,2);
  b.inspect("b");
  a += b;
  a.inspect("a += b");
  a -= b;
  a.inspect("a -= b");
  bool _b = a == b;
  myPrintf("a == b => %s \n", _b ? "true" : "false");
  _b = a > b;
  myPrintf("a > b => %s \n", _b ? "true" : "false");
  _b = a < b;
  myPrintf("a < b => %s \n", _b ? "true" : "false");
   _b = a >= b;
  myPrintf("a >= b => %s \n", _b ? "true" : "false");
  _b = a <= b;
  myPrintf("a <= b => %s \n", _b ? "true" : "false");
  a = -a;
  a.inspect("a = -a");
  Vector3 c = a.product(b);
  c.inspect("a.product(b)");


  Vector3::up.inspect("Vector3::up");
  Vector3::right.inspect("Vector3::right");
  Vector3::dx.inspect("Vector3::dx");
  Vector3::dy.inspect("Vector3::dy");
  Vector3::dz.inspect("Vector3::dz");
  Vector3::zero.inspect("Vector3::zero");
  Vector3::one.inspect("Vector3::one");

  a = Vector3(-1, 0, 0);
  b = Vector3(-1, 1, 0);
  ffloat f = Vector3::angleTo(a, b);
  f *= ffdeg_unit;
  printff("Vector3::angleTo(a, b)", f);

  ffloat r = a.dot(b);
  printff("a.dot(b)", r);

  c = a.cross(b);
  c.inspect("a.cross(b)");

  a.inspect("a");
  b.inspect("b");
  a.addScaleVector(b, ffloat(2));
  a.inspect("a.addScaleVector(b, ffloat(2))");

  a.normalize();
  a.inspect("a.normalize()");
}


Application * getApp()
{
    return new FixPointDemo("FixPointDemo", 1136, 640);
}
