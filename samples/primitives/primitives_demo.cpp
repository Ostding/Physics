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
  radP = pi;

  rBtnDown = false;
  radDY = 0.0f;
  radDP = 0.0f;

  eX = 0.0f;
  eY = 1.0f;
  eZ = 25.0f;

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

  float rp = std::abs(lookDist * std::cos(radY));
  float dx = std::sin(radP) * rp;
  float dz = std::cos(radP) * rp;
  float dy = lookDist * std::sin(radY);
  
  dX = eX + dx;
  dY = eY + dy;
  dZ = eZ + dz;

  gluLookAt(eX, eY, eZ,  dX, dY, dZ,  0.0f, 1.0f, 0.0f);

  render();

  glColor3f(0.0f, 0.0f, 0.0f);
  textOut(10.0f, 100.0f, "Physic Demo: Test Fraction \n \
    Press 'g' to run; \n \
    Press 'space' to simulate one step; \n \
    Press mouse left button and drag to adjust camera angle; \n \
    Press 'w' to move camera forward; \n \
    Press 'a' to move camera left; \n \
    Press 's' to move camera back; \n \
    Press 'd' to move camera right; \n \
    Press 'q' to quite sample application;");
  
  Application::onDisplay();
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
  }else 
  if(button == GLUT_RIGHT_BUTTON)
  {
    if(state == GLUT_DOWN)
    {
      radDY = 0.0f;
      radDP = 0.0f;

      lastPoint.x = x;
      lastPoint.y = y;
      rBtnDown = true;
    }
    else
    {
      rBtnDown = false;
    }
  }
}

void PrimitivesDemo::onMouseMove(int x, int y)
{
  int deltaX = x - lastPoint.x;
  int deltaY = y - lastPoint.y;

  float dtP = pi * ((float)deltaX / (float)width);
  float dtY = pi * ((float)deltaY / (float)height);

  lastPoint.x = x;
  lastPoint.y = y;

  if(lBtnDown)
  {  
    radY += dtY;
    radP += dtP;
    if(radY < (-pi/2)) radY = -pi/2;
    if(radY > (pi/2)) radY = pi/2;

  }else
  if(rBtnDown)
  {
    radDY += dtY;
    radDP += dtP;
    if(radDY < (-pi/2)) radDY = -pi/2;
    if(radDY > (pi/2)) radDY = pi/2;

    float rp = std::abs(lookDist * std::cos(radDY));
    float dx = std::sin(radDP) * rp;
    float dz = std::cos(radDP) * rp;
    float dy = lookDist * std::sin(radDY);
    eX = dX + dx;
    eY = dY + dy;
    eZ = dZ + dz;
  }
}

void PrimitivesDemo::onKeyboardPress(unsigned char key)
{
  switch( key ) 
  {
    case 'g': case 'G':
      simulate = true;
      initTest();
      break;
    case ' ':
      simulate = false;
      world->update(deltaTime);
      break;
    case 'w': case 'W':
    {
      float step = 1;
      float rp = std::abs(step * std::cos(radY));
      float dx = std::sin(radP) * rp;
      float dz = std::cos(radP) * rp;
      float dy = step * std::sin(radY);
      eX += dx;
      eY += dy;
      eZ += dz;
      break;
    }
    case 's': case 'S':
    {
      float step = 1;
      float rp = std::abs(step * std::cos(radY));
      float dx = std::sin(radP) * rp;
      float dz = std::cos(radP) * rp;
      float dy = step * std::sin(radY);
      eX -= dx;
      eY -= dy;
      eZ -= dz;
      break;
    }
    case 'a': case 'A':
    {
      float step = 1;
      float rp = std::abs(step * std::cos(radY));
      float dx = std::sin(radP + pi/2) * rp;
      float dz = std::cos(radP + pi/2) * rp;
      eX += dx;
      eZ += dz;
      break;
    }
    case 'd': case 'D':
    {
      float step = 1;
      float rp = std::abs(step * std::cos(radY));
      float dx = std::sin(radP - pi/2) * rp;
      float dz = std::cos(radP - pi/2) * rp;
      eX += dx;
      eZ += dz;
      break;
    }
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
  if(elapse >= 0.01f) 
  {
    updateTime(elapse);
    if(simulate)
      world->update(deltaTime);
  }
}

void PrimitivesDemo::initOneBox(const Vector3 &pos, const Vector3 &extents, ffloat mass)
{
  Box *box = new Box(extents);
  box->body->setLinearDamp(ffloat(0.95f));
	box->body->setAngularDamp(ffloat(0.2f));

  Vector3 squares = extents.product(extents);
  ffloat f3 = ffone / ffloat(enlarge * 12ll);
  ffloat yz = mass * (squares.y + squares.z);
  ffloat xz = mass * (squares.x + squares.z);
  ffloat xy = mass * (squares.x + squares.y);

  Matrix3 tensor;
  tensor.setDiagonal(f3 * yz, f3 * xz, f3 * xy);
  box->body->setInertiaTensor(tensor);

  box->body->setMass(mass);
  box->body->enableSleep(true);
  box->body->setAwake();

  box->setPosition(pos);
  Quaternion q = Quaternion::fromEulerAngles(Vector3(ffzero, ffzero, ffloat(45)));
  box->setOrientation(q);
  world->addPrimitive( box );
}

void PrimitivesDemo::initOneSphere(ffloat radius, const Vector3 &pos, ffloat mass)
{
  Sphere * sphere = new Sphere(radius);

  sphere->body->setLinearDamp(ffloat(0.95f));
	sphere->body->setAngularDamp(ffloat(0.2f));

  ffloat coeff = ffloat(40000LL) * mass * radius * radius;
  Matrix3 tensor;
  tensor.setDiagonal(coeff, coeff, coeff);
  sphere->body->setInertiaTensor(tensor);

  sphere->body->setMass(mass);
  sphere->body->enableSleep(true);
  sphere->body->setAwake();

  sphere->setPosition(pos);
  world->addPrimitive( sphere );
}

void PrimitivesDemo::initOnePlane(const Vector3 &dir, const Vector3 &extents, const ffloat &offset)
{
  Plane *plane = new Plane(dir, extents, offset);
  world->addPrimitive( plane );
}

void PrimitivesDemo::initTest()
{
  if(started) return;

  Vector3 e1 = Vector3(ffloat(100), ffloat(0.5), ffloat(100));
  initOnePlane( Vector3::up, e1, ffzero);

  Vector3 p1 = Vector3(ffzero, ffloat(15), ffzero);
  ffloat r1 = ffloat(3);
  ffloat m1 = ffloat(10);
  initOneSphere(r1, p1, m1);

  Vector3 p2 = Vector3(ffone, ffloat(20), ffzero);
  ffloat r2 = ffloat(5);
  ffloat m2 = ffloat(10);
  initOneSphere(r2, p2, m2);

  Vector3 p3 = Vector3(ffloat(3), ffloat(20), ffzero);
  Vector3 e3 = Vector3(ffloat(2), ffloat(2), ffloat(2));
  ffloat m3 = ffloat(10);
  initOneBox(p3, e3, m3);

  Vector3 p4 = Vector3(ffloat(5), ffloat(25), ffzero);
  Vector3 e4 = Vector3(ffloat(2), ffloat(2), ffloat(2));
  ffloat m4 = ffloat(10);
  initOneBox(p4, e4, m4);

  world->prepare();
  started = true;
}

Application * getApp()
{
    return new PrimitivesDemo("PrimitivesDemo", 1136, 640);
}

