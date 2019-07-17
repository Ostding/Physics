#include "primitives_demo.h"
#include "physics.h"

PrimitivesDemo::PrimitivesDemo(const char *title, int width, int height)
:Application(title, width, height)
{}

void PrimitivesDemo::onDisplay()
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity();
  gluLookAt(5.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f);

  render();

  glColor3f(0.0f, 0.0f, 0.0f);
  textOut(10.0f, 34.0f, "Physic Demo: Test Fraction \n \
    Press 'g' to run demo and watch primitives in window; \n \
    Press 'q' to quite sample application;");
}

void PrimitivesDemo::onKeyboardPress(unsigned char key)
{
  switch( key ) {
  case 'g': case 'G':
    doTest();
    break;
  }

  Application::onKeyboardPress(key);
}

void PrimitivesDemo::render()
{
  //to do
}

void PrimitivesDemo::doTest()
{
  //to do 
}

Application * getApp()
{
    return new PrimitivesDemo("PrimitivesDemo", 1136, 640);
}

