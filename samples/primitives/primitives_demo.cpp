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
  simulate = false;
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
    if(!simulate)
    {
      doTest();
      simulate = true;
    }
    break;
  case 's': case 'S':
    simulate = false;
    break;
  case ' ':
    simulate = false;
    world->update(deltaTime);
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
  if(elapse >= 0.02f) 
  {
    updateTime();
    if(simulate)
      world->update(deltaTime);
  }
}

void PrimitivesDemo::doTest()
{
  Vector3 direction = Vector3::up;
  Vector3 center = Vector3(ffzero, ffzero, ffzero); 
  Vector3 extents = Vector3(ffloat(100), ffloat(100), ffloat(100));
  Plane *plane = new Plane(direction, center, extents);
  world->addPrimitive( plane );

  
  ffloat radius = ffloat(3.0f);
  Sphere * sphere = new Sphere(radius);

  sphere->body->setLinearDamp(ffloat(0.95f));
	sphere->body->setAngularDamp(ffloat(0.2f));

  ffloat mass = ffloat(10);
  ffloat coeff = ffloat(0.4f) * mass * radius * radius;
  Matrix3 tensor;
  tensor.setDiagonal(coeff, coeff, coeff);
  sphere->body->setInertiaTensor(tensor);

  sphere->body->setMass(mass);
  sphere->body->enableSleep(true);
  sphere->body->setAwake();

  Vector3 pos = Vector3(ffzero, ffloat(10), ffzero);
  sphere->body->setPosition(pos);
  world->addPrimitive( sphere );
}

Application * getApp()
{
    return new PrimitivesDemo("PrimitivesDemo", 1136, 640);
}

