#include <math.h>
#include "primitives_demo.h"
#include "physics.h"

static const double pi = 3.1416;
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
  lBtnDown = false;
  radY = (1.0f/6)*pi;
  radP = 0;
  lookDist = 50;
  started = false;
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
  glEnable(GL_DEPTH_TEST);

  float rp = lookDist * std::cos(radY);
  float ex = std::sin(radP) * rp;
  float ez = std::cos(radP) * rp;
  float ey = lookDist * std::sin(radY);;
  gluLookAt(ex, ey, ez,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f);

  render();

  glColor3f(0.0f, 0.0f, 0.0f);
  textOut(10.0f, 34.0f, "Physic Demo: Test Fraction \n \
    Press 'g' to run demo and watch primitives in window; \n \
    Press 'q' to quite sample application;");
}

void PrimitivesDemo::onMousePress(int button, int state, int x, int y)
{
  if(button == GLUT_LEFT_BUTTON)
  {
    if(state == GLUT_DOWN)
    {
      lastPoint.x = x;
      lastPoint.y = y;
      lBtnDown = true;
    }
    else
    {
      lBtnDown = false;
    }
  }
}

void PrimitivesDemo::onMouseMove(int x, int y)
{
  if(lBtnDown)
  {
    int deltaX = x - lastPoint.x;
    int deltaY = y - lastPoint.y;

    float dtP = pi * ((float)deltaX / (float)width);
    float dtY = pi * ((float)deltaY / (float)height);

    lastPoint.x = x;
    lastPoint.y = y;
    
    radY += dtY;
    radP += dtP;
    if(radY < (-pi/2)) radY = -pi/2;
    if(radY > (pi/2)) radY = pi/2;
  }
}

void PrimitivesDemo::onKeyboardPress(unsigned char key)
{
  switch( key ) {
  case 'g': case 'G':
    simulate = true;
    initTest();
    break;
  case 's': case 'S':
    simulate = false;
    break;
  case ' ':
    simulate = false;
    world->update(deltaTime);
    break;
  case 'f': case 'F':
    lookDist -= 0.5;
    break;
  case 'b': case 'B':
    lookDist += 0.5;
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

void PrimitivesDemo::initTest()
{
  if(started) return;

  Vector3 direction = Vector3::up;
  Vector3 center = Vector3(ffzero, ffzero, ffzero); 
  Vector3 extents = Vector3(ffloat(50), ffloat(0.5), ffloat(50));
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

  started = true;
}

Application * getApp()
{
    return new PrimitivesDemo("PrimitivesDemo", 1136, 640);
}

