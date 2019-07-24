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
  textOut(10.0f, 100.0f, "Physic Demo: Test Fraction \n \
    Press 'g' to run; \n \
    Press 's' to pause; \n \
    Press 'space' to simulate one step; \n \
    Drag mouse to adjust camera angle; \n \
    Press 'f' to move camera forward; \n \
    Press 'b' to move camera back; \n \
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
    lookDist -= 1;
    break;
  case 'b': case 'B':
    lookDist += 1;
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

void PrimitivesDemo::initOneSphere(ffloat radius, const Vector3 &pos, ffloat mass)
{
  Sphere * sphere = new Sphere(radius);

  sphere->body->setLinearDamp(ffloat(0.95f));
	sphere->body->setAngularDamp(ffloat(0.2f));

  ffloat coeff = ffloat(0.4f) * mass * radius * radius;
  Matrix3 tensor;
  tensor.setDiagonal(coeff, coeff, coeff);
  sphere->body->setInertiaTensor(tensor);

  sphere->body->setMass(mass);
  sphere->body->enableSleep(true);
  sphere->body->setAwake();

  sphere->setPosition(pos);
  world->addPrimitive( sphere );
}

void PrimitivesDemo::initOnePlane(const Vector3 &pos, const Vector3 &dir, const Vector3 &extents)
{
  Plane *plane = new Plane(dir, extents);
  plane->setPosition(pos);
  world->addPrimitive( plane );
}

void PrimitivesDemo::initTest()
{
  if(started) return;

  Vector3 extents = Vector3(ffloat(50), ffloat(0.5), ffloat(50));
  initOnePlane(Vector3::zero, Vector3::up, extents);

  Vector3 pos = Vector3(ffloat(12), ffloat(8), ffzero); 
  extents = Vector3(ffloat(10), ffloat(0.1), ffloat(10));
  Vector3 dir = Vector3(ffloat(-1), ffone, ffzero);
  dir.normalize();
  initOnePlane(pos, Vector3::up, extents);

  pos = Vector3(ffzero, ffloat(15), ffzero);
  ffloat radius = ffloat(3);
  ffloat mass = ffloat(10);
  initOneSphere(radius, pos, mass);

  pos = Vector3(ffzero, ffloat(20), ffzero);
  radius = ffloat(1.5);
  mass = ffloat(20);
  initOneSphere(radius, pos, mass);

  world->prepare();
  started = true;
}

Application * getApp()
{
    return new PrimitivesDemo("PrimitivesDemo", 1136, 640);
}

