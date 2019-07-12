#include "fix_point_demo.h"
#include "timer.h"
#include "test_fixedfloat.h"

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
    doTestSpeed();

    break;
  }

  Application::onKeyboardPress(key);
}


void FixPointDemo::doTestSpeed()
{
  printf("//////////////////test speed///////////////////////////\n");
  int count = 10000000;
  //base value type's calculation
  double i = 123456;
  double j = 3.141592;
  double ret = 0.0;
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    ret = i + j;
  Timer::stop("Base Value type +");
  printf("ret:%.8f \n", ret);
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    ret = i - j;
  Timer::stop("Base Value type -");
  printf("ret:%.8f \n", ret);
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    ret = i * j;
  Timer::stop("Base Value type *");
  printf("ret:%.8f \n", ret);
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    ret = i / j;
  Timer::stop("Base Value type /");
  printf("ret:%.8f \n", ret);
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    ret = sqrt(i);
  Timer::stop("Base Value type sqrt");
  printf("ret:%.8f \n", ret);
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    ret = sin(1.571f);
  Timer::stop("Base Value type sin");
  printf("ret:%.8f \n", ret);
  printf("\n\n");


  //test normal calculation of FixedFloat
  FixedFloat m = FixedFloat(123456);
  FixedFloat n = FixedFloat(3.14159f);
  FixedFloat ret1;
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = m + n;
  Timer::stop("FixedFloat +");
  TestFixedFloat::print("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = m - n;
  Timer::stop("FixedFloat -");
  TestFixedFloat::print("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = m * n;
  Timer::stop("FixedFloat *");
  TestFixedFloat::print("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = m / n;
  Timer::stop("FixedFloat /");
  TestFixedFloat::print("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = FixedFloat::sqrt(m);
  Timer::stop("FixedFloat sqrt");
  TestFixedFloat::print("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = FixedFloat::sin(FixedFloat::half_pi);
  Timer::stop("FixedFloat sin");
  TestFixedFloat::print("ret1", ret1);
  printf("\n\n");

}

void FixPointDemo::doTestFixedFloat()
{
  TestFixedFloat::doTest();
}


Application * getApp()
{
    return new FixPointDemo("FixPointDemo", 1136, 640);
}
