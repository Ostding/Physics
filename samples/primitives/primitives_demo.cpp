#include "primitives_demo.h"
#include "physics.h"

static ffloat deltaTime = ffloat(0.02f);
PrimitivesDemo::PrimitivesDemo(const char *title, int width, int height)
:Application(title, width, height)
{
  Vector3 spaceMin = Vector3(ffloat(-200.0f), ffzero, ffloat(-200.0f));
  Vector3 spaceMax = Vector3(ffloat(200.0f), ffloat(200.0f), ffloat(200.0f));
  unsigned maxContacts = 255;
  unsigned iterations = 4;
  world = new World(spaceMin, spaceMax, maxContacts, iterations);
}

PrimitivesDemo::~PrimitivesDemo()
{
  if(world)
  {
    delete world;
    world = 0;
  }
}

void PrimitivesDemo::onDisplay()
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity();
  gluLookAt(15.0f, 15.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f);

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
  world->render();
}

void PrimitivesDemo::onUpdate()
{
  double elapse = calcDuration();
  if(elapse > 0.02f) updateTime();

  world->update(deltaTime);
}

void PrimitivesDemo::doTest()
{
  Vector3 direction = Vector3::up;
  Vector3 center = Vector3(ffzero, ffzero, ffzero); 
  Vector3 extents = Vector3(ffloat(100), ffloat(100), ffloat(100));
  Plane *plane = new Plane(direction, center, extents);
  world->addPrimitive( plane );

  Vector3 pos = Vector3(ffzero, ffloat(10), ffzero);
  Sphere * sphere = new Sphere(ffloat(3.0f));
  sphere->body->setPosition(pos);
  world->addPrimitive( sphere );
}

Application * getApp()
{
    return new PrimitivesDemo("PrimitivesDemo", 1136, 640);
}

