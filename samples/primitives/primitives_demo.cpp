#include <math.h>
#include "primitives_demo.h"
#include "physics.h"

static const double pi = 3.1416;
static float stepTime = 0.01f;
static ffloat deltaTime = ffloat(stepTime);
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
  radY = (-1.0f/6)*pi;
  radP = pi;

  eX = 0.0f;
  eY = 25.0f;
  eZ = 50.0f;

  lookDist = 50;
  started = false;

  wasdMode = WASD_MODE::CAMERA;

  moveID = 0;
  acc = 10;
  velocity = 50;
  
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

  textOut(10.0f, 100.0f, "Physic Demo: Test primitives and force generators \n \
    Press 'g' to run; \n \
    Press 'space' to simulate one step; \n \
    Press mouse left button and drag to adjust camera angle; \n \
    Press 'w' to move camera forward; \n \
    Press 'a' to move camera left; \n \
    Press 's' to move camera back; \n \
    Press 'd' to move camera right; \n \
    Press 'm' to switch wasd key mode; \n \
    Press 'q' to quite sample application;");
  
  char psz[64] = {0};
  switch(wasdMode)
  {
    case WASD_MODE::CAMERA:
      sprintf(psz, "wasd mode:%s", "camera");
      break;
    case WASD_MODE::MOVE:
      sprintf(psz, "wasd mode:%s", "force");
      break;
    case WASD_MODE::VELOCITY:
      sprintf(psz, "wasd mode:%s", "velocity");
      break;
  }
 
  textOut(10, height - 20, psz);

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

  }
}

void PrimitivesDemo::setMoveAcc(unsigned char key)
{
  ffloat x, z;
  switch(key)
  {
    case 'w': case 'W':
    {
      x = ffloat(std::sin(radP)) * acc;
      z = ffloat(std::cos(radP)) * acc;
      break;
    }
    case 's': case 'S':
    {
      x = -ffloat(std::sin(radP)) * acc;
      z = -ffloat(std::cos(radP)) * acc;
      break;
    }
    case 'a': case 'A':
    {
      x = ffloat(std::sin(radP + pi/2)) * acc;
      z = ffloat(std::cos(radP + pi/2)) * acc;
      break;
    }
    case 'd': case 'D':
    {
      x = ffloat(std::sin(radP - pi/2)) * acc;
      z = ffloat(std::cos(radP - pi/2)) * acc;
      break;
    }
  }
  
  if(moveID != 0)
  {
    ForceGenerator *f = world->removeForceGenerator(moveSphere, moveID);
    if(f != 0)
      delete f;
    moveID = 0;
  }

  Vector3 a = Vector3(x, ffzero, z);
  MoveForce *moveAcc = new MoveForce(Vector3::zero, Vector3::zero);
  moveAcc->setForceAcceleration(a);
  
  moveSphere->body->setAwake(true);
  moveID = world->addForceGenerator(moveSphere, moveAcc);
}

void PrimitivesDemo::setMoveVelocity(unsigned char key)
{
  ffloat x, z;
  switch(key)
  {
    case 'w': case 'W':
    {
      x = ffloat(std::sin(radP));
      z = ffloat(std::cos(radP));
      break;
    }
    case 's': case 'S':
    {
      x = -ffloat(std::sin(radP));
      z = -ffloat(std::cos(radP));
      break;
    }
    case 'a': case 'A':
    {
      x = ffloat(std::sin(radP + pi/2));
      z = ffloat(std::cos(radP + pi/2));
      break;
    }
    case 'd': case 'D':
    {
      x = ffloat(std::sin(radP - pi/2));
      z = ffloat(std::cos(radP - pi/2));
      break;
    }
  }

  moveSphere->body->setAwake();
  Vector3 v = Vector3(x, ffzero, z) * velocity;
  moveSphere->body->setVelocity(v);
}

void PrimitivesDemo::setCameraPos(unsigned char key)
{
  switch(key)
  {
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
}

void PrimitivesDemo::onKeyboardUp(unsigned char key)
{
  if(moveID != 0)
  {
    world->removeForceGenerator(moveSphere, moveID);
    moveID = 0;
  }

  Application::onKeyboardUp(key);
}

void PrimitivesDemo::onKeyboardDown(unsigned char key)
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
      if(wasdMode == WASD_MODE::CAMERA)
        setCameraPos(key);
      else
      if(wasdMode == WASD_MODE::MOVE)
        setMoveAcc(key);
      else
      if(wasdMode == WASD_MODE::VELOCITY)
        setMoveVelocity(key);
      break;
    }
    case 's': case 'S':
    {
      if(wasdMode == WASD_MODE::CAMERA)
        setCameraPos(key);
      else
      if(wasdMode == WASD_MODE::MOVE)
        setMoveAcc(key);
      else
      if(wasdMode == WASD_MODE::VELOCITY)
        setMoveVelocity(key);
      break;
    }
    case 'a': case 'A':
    {
      if(wasdMode == WASD_MODE::CAMERA)
        setCameraPos(key);
      else
      if(wasdMode == WASD_MODE::MOVE)
        setMoveAcc(key);
      else
      if(wasdMode == WASD_MODE::VELOCITY)
        setMoveVelocity(key);
      break;
    }
    case 'd': case 'D':
    {
      if(wasdMode == WASD_MODE::CAMERA)
        setCameraPos(key);
      else
      if(wasdMode == WASD_MODE::MOVE)
        setMoveAcc(key);
      else
      if(wasdMode == WASD_MODE::VELOCITY)
        setMoveVelocity(key);
      break;
    }
    case 'm': case 'M':
    {
      if(wasdMode == WASD_MODE::CAMERA)
      {
        wasdMode = WASD_MODE::MOVE;
        glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
      }  
      else 
      if(wasdMode == WASD_MODE::MOVE)
      {
        wasdMode = WASD_MODE::VELOCITY;
        glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
      }else
      if(wasdMode == WASD_MODE::VELOCITY)
      {
        wasdMode = WASD_MODE::CAMERA;
        glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
      }
    }
  }

  Application::onKeyboardDown(key);
}

void PrimitivesDemo::render()
{
  world->render();
}

void PrimitivesDemo::onUpdate()
{
  double elapse = calcDuration();
  if(elapse >= stepTime) 
  {
    updateTime(elapse);
    if(simulate)
      world->update(deltaTime);
  }
}
GravityForce * PrimitivesDemo::genGravityForce()
{
  return new GravityForce(Vector3(ffzero, ffloat(-9.8), ffzero));
}

Capsule *  PrimitivesDemo::initOneCapsule(const Vector3 &pos, const ffloat &radius, const ffloat &halfHeight, const ffloat &mass, const Vector3 &angles)
{
  Capsule *cap = new Capsule(halfHeight, radius);
  cap->body->setConstAccumulator(Vector3::zero);
  cap->initInertiaTensor(mass);
  cap->body->setLinearDamp(ffloat(0.95f));
	cap->body->setAngularDamp(ffloat(0.8f));
  cap->body->setMass(mass);
  cap->body->enableSleep(true);
  cap->body->setAwake();

  cap->setPosition(pos);

  int anglex = std::rand() % 180;
  int angley = std::rand() % 180;
  int anglez = std::rand() % 180;
  // Quaternion q = Quaternion::fromEulerAngles(angles);
  Quaternion q = Quaternion::fromEulerAngles(Vector3(ffloat(anglex), ffloat(angley), ffloat(anglez)));
  cap->setOrientation(q);
  world->addPrimitive(cap);
  world->addForceGenerator(cap, genGravityForce());
  return cap;
}

Polyhedron * PrimitivesDemo::initOnePolyHedron(const Vector3 &pos, const ffloat &mass, const Vector3 &angles)
{
  Polyhedron::Points points;
  points.emplace_back(Vector3(ffloat(-4), ffloat(0), ffloat(2))); points.emplace_back(Vector3(ffloat(-4), ffloat(0), ffloat(-2)));
  points.emplace_back(Vector3(ffloat(-2), ffloat(2), ffloat(2))); points.emplace_back(Vector3(ffloat(-2), ffloat(2), ffloat(-2)));
  points.emplace_back(Vector3(ffloat(2), ffloat(2), ffloat(2))); points.emplace_back(Vector3(ffloat(2), ffloat(2), ffloat(-2)));
  points.emplace_back(Vector3(ffloat(4), ffloat(0), ffloat(2))); points.emplace_back(Vector3(ffloat(4), ffloat(0), ffloat(-2)));
  points.emplace_back(Vector3(ffloat(2), ffloat(-2), ffloat(2))); points.emplace_back(Vector3(ffloat(2), ffloat(-2), ffloat(-2)));
  points.emplace_back(Vector3(ffloat(-2), ffloat(-2), ffloat(2))); points.emplace_back(Vector3(ffloat(-2), ffloat(-2), ffloat(-2)));
  points.emplace_back(Vector3(ffloat(0), ffloat(0), ffloat(-2))); points.emplace_back(Vector3(ffloat(0), ffloat(0), ffloat(2)));

  Polyhedron::Indices indices;
  indices.emplace_back(0); indices.emplace_back(2); indices.emplace_back(1);
  indices.emplace_back(1); indices.emplace_back(2); indices.emplace_back(3);
  indices.emplace_back(2); indices.emplace_back(4); indices.emplace_back(3);
  indices.emplace_back(3); indices.emplace_back(4); indices.emplace_back(5);
  indices.emplace_back(4); indices.emplace_back(6); indices.emplace_back(5);
  indices.emplace_back(5); indices.emplace_back(6); indices.emplace_back(7);
  indices.emplace_back(6); indices.emplace_back(8); indices.emplace_back(7);
  indices.emplace_back(7); indices.emplace_back(8); indices.emplace_back(9);
  indices.emplace_back(8); indices.emplace_back(10); indices.emplace_back(9);
  indices.emplace_back(9); indices.emplace_back(10); indices.emplace_back(11);
  indices.emplace_back(10); indices.emplace_back(0); indices.emplace_back(11);
  indices.emplace_back(11); indices.emplace_back(0); indices.emplace_back(1);

  indices.emplace_back(0); indices.emplace_back(13); indices.emplace_back(2);
  indices.emplace_back(2); indices.emplace_back(13); indices.emplace_back(4);
  indices.emplace_back(4); indices.emplace_back(13); indices.emplace_back(6);
  indices.emplace_back(6); indices.emplace_back(13); indices.emplace_back(8);
  indices.emplace_back(8); indices.emplace_back(13); indices.emplace_back(10);
  indices.emplace_back(10); indices.emplace_back(13); indices.emplace_back(0);
  indices.emplace_back(1); indices.emplace_back(3); indices.emplace_back(12);
  indices.emplace_back(3); indices.emplace_back(5); indices.emplace_back(12);
  indices.emplace_back(5); indices.emplace_back(7); indices.emplace_back(12);
  indices.emplace_back(7); indices.emplace_back(9); indices.emplace_back(12);
  indices.emplace_back(9); indices.emplace_back(11); indices.emplace_back(12);
  indices.emplace_back(11); indices.emplace_back(1); indices.emplace_back(12);


  Polyhedron *poly = new Polyhedron();
  poly->setPoints(points, indices);

  poly->body->setConstAccumulator(Vector3::zero);
  poly->body->setLinearDamp(ffloat(0.95f));
	poly->body->setAngularDamp(ffloat(0.8f));
  
  poly->initInertiaTensor( mass);
  poly->body->setMass(mass);
  poly->body->enableSleep(true);
  poly->body->setAwake();

  poly->setPosition(pos);
  int anglex = std::rand() % 180;
  int angley = std::rand() % 180;
  int anglez = std::rand() % 180;
  Quaternion q = Quaternion::fromEulerAngles(Vector3(ffloat(anglex), ffloat(angley), ffloat(anglez)));
  // Quaternion q = Quaternion::fromEulerAngles(angles);
  poly->setOrientation(q);
  world->addPrimitive( poly );
  world->addForceGenerator(poly, genGravityForce());
  return poly;
}

Box * PrimitivesDemo::initOneBox(const Vector3 &pos, const Vector3 &extents, const Vector3 &angles, const ffloat &mass)
{
  Box *box = new Box(extents);
  box->initInertiaTensor(mass);
  box->body->setConstAccumulator(Vector3::zero);
  box->body->setLinearDamp(ffloat(0.95f));
	box->body->setAngularDamp(ffloat(0.8f));
  box->body->setMass(mass);
  box->body->enableSleep(true);
  box->body->setAwake();

  box->setPosition(pos);
  Quaternion q = Quaternion::fromEulerAngles(Vector3(angles.x, angles.y, angles.z));
  box->setOrientation(q);
  world->addPrimitive( box );
  world->addForceGenerator(box, genGravityForce());
  return box;
}

Sphere *  PrimitivesDemo::initOneSphere(const ffloat &radius, const Vector3 &pos, const ffloat &mass)
{
  Sphere * sphere = new Sphere(radius);
  sphere->body->setConstAccumulator(Vector3::zero);
  sphere->initInertiaTensor(mass);
  sphere->body->setLinearDamp(ffloat(0.95f));
	sphere->body->setAngularDamp(ffloat(0.8f));
  sphere->body->setMass(mass);
  sphere->body->enableSleep(true);
  sphere->body->setAwake();

  sphere->setPosition(pos);
  world->addPrimitive( sphere );
  world->addForceGenerator(sphere, genGravityForce());
  return sphere;
}

Plane * PrimitivesDemo::initOnePlane(const Vector3 &dir, const Vector3 &extents, const ffloat &offset)
{
  Plane *plane = new Plane(dir, extents, offset);
  world->addPrimitive( plane );
  return plane;
}

void PrimitivesDemo::initTest()
{
  if(started) return;

  Vector3 e1 = Vector3(ffloat(100), ffloat(0.5), ffloat(100));
  initOnePlane( Vector3::up, e1, ffzero);

  Vector3 p0 = Vector3(ffzero, ffloat(1), ffzero);
  ffloat r0 = ffloat(2);
  ffloat m0 = ffloat(10);
  moveSphere = initOneSphere(r0, p0, m0);

  // Vector3 p1 = Vector3(ffzero, ffloat(35), ffzero);
  // ffloat r1 = ffloat(3);
  // ffloat m1 = ffloat(10);
  // initOneSphere(r1, p1, m1);

  // Vector3 p2 = Vector3(ffzero, ffloat(20), ffzero);
  // ffloat r2 = ffloat(5);
  // ffloat m2 = ffloat(10);
  // initOneSphere(r2, p2, m2);

  // Vector3 p3 = Vector3(ffzero, ffloat(5), ffzero);
  // Vector3 e3 = Vector3(ffloat(2), ffloat(2), ffloat(2));
  // Vector3 a3 = Vector3(ffzero, ffloat(90), ffzero);
  // ffloat m3 = ffloat(10);
  // initOneBox(p3, e3, a3, m3);

  // Vector3 p4 = Vector3(ffloat(3), ffloat(10), ffzero);
  // Vector3 e4 = Vector3(ffloat(2), ffloat(2), ffloat(2));
  // Vector3 a4 = Vector3(ffzero, ffloat(90), ffloat(45));
  // ffloat m4 = ffloat(10);
  // initOneBox(p4, e4, a4, m4);

  // ffloat m5 = ffloat(2);
  // Vector3 p5 = Vector3(ffzero, ffloat(5), ffzero);
  // initOnePolyHedron(p5, m5, Vector3(ffzero, ffloat(90), ffloat(45)));

  // ffloat m6 = ffloat(2);
  // Vector3 p6 = Vector3(ffzero, ffloat(15), ffzero);
  // initOnePolyHedron(p6, m6, Vector3(ffzero, ffloat(90), ffzero));

  // p6 = Vector3(ffloat(4), ffloat(15), ffzero);
  // initOnePolyHedron(p6, m6, Vector3(ffloat(45), ffloat(80), ffloat(45)));
  // p6 = Vector3(ffloat(-4), ffloat(15), ffzero);
  // initOnePolyHedron(p6, m6, Vector3(ffloat(-45), ffloat(70), ffloat(45)));
  // p6 = Vector3(ffzero, ffloat(10), ffloat(4));
  // initOnePolyHedron(p6, m6, Vector3(ffloat(90), ffloat(60), ffloat(30)));
  // p6 = Vector3(ffzero, ffloat(15), ffloat(-4));
  // initOnePolyHedron(p6, m6, Vector3(ffloat(60), ffloat(50), ffloat(35)));

  Vector3 p7 = Vector3(ffzero, ffloat(15), ffzero);
  Vector3 a7 = Vector3(ffloat(10), ffloat(30), ffloat(60));
  ffloat m7 = ffloat(10);
  ffloat r7 = ffloat(2);
  ffloat h7 = ffloat(2);
  Capsule *cap = initOneCapsule(p7, r7, h7, m7, a7);

  // Vector3 p8 = Vector3(ffzero, ffloat(25), ffzero);
  // Vector3 a8 = Vector3(ffloat(10), ffloat(30), ffloat(60));
  // ffloat m8 = ffloat(10);
  // ffloat r8 = ffloat(2);
  // ffloat h8 = ffloat(2);
  // initOneCapsule(p8, r8, h8, m8, a8);

  //Test spring 
  Vector3 connectPt0 = Vector3(ffzero, ffzero, ffzero);
  Vector3 connectPt1 = Vector3(ffzero, ffzero, ffzero);
  ffloat springCeof = ffloat(100);
  ffloat maxForce = ffloat(500);
  ffloat springLength = ffloat(6);
  SpringForce *spA = new SpringForce(connectPt0, cap->body, connectPt1, springCeof, springLength, maxForce );
  SpringForce *spB = new SpringForce(connectPt1, moveSphere->body, connectPt0, springCeof, springLength, maxForce );
  world->addForceGenerator(moveSphere, spA);
  world->addForceGenerator(cap, spB);
  
  world->prepare();
  started = true;
}

Application * getApp()
{
    return new PrimitivesDemo("PrimitivesDemo", 1136, 640);
}

