#include "fix_point_demo.h"
#include "timer.h"
#include "test_fixed_float.h"

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


Application * getApp()
{
    return new FixPointDemo("FixPointDemo", 1136, 640);
}
