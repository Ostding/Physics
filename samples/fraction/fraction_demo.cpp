#include "fraction_demo.h"

FractionDemo::FractionDemo(const char *title, int width, int height)
:Application(title, width, height)
{

}

void FractionDemo::onDisplay()
{
   glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glLoadIdentity();
   gluLookAt(5.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f);
   
   glColor3f(0.0f, 0.0f, 0.0f);
   
   textOut(10.0f, 34.0f, "Physic Demo:\n Test Fraction");
}


void FractionDemo::onKeyboardPress(unsigned char key)
{
  switch( key ) {
  case 'g': case 'G':
    doTestFraction();
    break;
  }

  Application::onKeyboardPress(key);
}

void FractionDemo::print(const char* pszCap, Fraction &f)
{
  float v = f.to_f();
  printf("%s => %.5f\n", pszCap, v);
}

void FractionDemo::doTestFraction()
{
  char psz[32] = {0};
  Fraction a = Fraction(1);
  a.cstr(psz);
  printf("Fraction(1) => %s\n", psz);

  a = Fraction(1,2);
  print("Fraction(1,2)", a);
}

Application * getApp()
{
    return new FractionDemo("FractionDemo", 1136, 640);
}
