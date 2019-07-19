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

void FixPointDemo::onKeyboardPress(unsigned char key)
{
  switch( key ) {
  case 'g': case 'G':
    doTestFixedFloat();
    doTestVector3();
    doTestQuaternion();
    doTestMatrix3();
    doTestMatrix4();

    doTestSpeed();
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

void FixPointDemo::doTestMatrix4()
{
  myPrintf("/////////test Matrix4/////// \n");
  Quaternion q = Quaternion::fromEulerAngles(Vector3(0, 0, 0));
  Vector3 p = Vector3(ffloat(1.5), ffloat(2.5), ffloat(3));
  Matrix4 m = Matrix4();
  m.setOrientationAndPos(q, p);

  Vector3 v = Vector3(1,1,1);
  v = m.transform(v);
  v.inspect("m.transform(v)[just position translate]");

  q = Quaternion::fromEulerAngles(Vector3(0, 90, 0));
  m.setOrientationAndPos(q, p);
  v = Vector3(1,1,1);
  v = m.transform(v);
  v.inspect("m.transform(v)[trun 90 around y axis]");
}

void FixPointDemo::doTestMatrix3()
{
  myPrintf("/////////test Matrix3/////// \n");
  Vector3 angles = Vector3(ffzero, ffloat(90), ffzero);
  Quaternion q = Quaternion::fromEulerAngles(angles);
  Matrix3 m = Matrix3();
  m.setOrientation(q);
  Vector3 v = Vector3(ffzero, ffzero, ffloat(-10));
  v.inspect("v");
  v = m.transform(v);
  v.inspect("m.transform(v)");

  v = m.transposeTransform(v);
  v.inspect("m.transposeTransform(v)");

  Vector3 angles1 = Vector3(ffloat(45), ffzero, ffloat(30));
  Quaternion q1 = Quaternion::fromEulerAngles(angles1);
  Matrix3 m1 = Matrix3();
  m1.setOrientation(q1);
  m1.inspect("m1");
  m.inspect("m");
  m = m1;
  m.inspect("m = m1");

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
   q.inspect("Quaternion::fromEulerAngles(Vector3(0, 90, 0))");
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
  a.addScaledVector(b, ffloat(2));
  a.inspect("a.addScaledVector(b, ffloat(2))");

  a.normalize();
  a.inspect("a.normalize()");
}


Application * getApp()
{
    return new FixPointDemo("FixPointDemo", 1136, 640);
}
