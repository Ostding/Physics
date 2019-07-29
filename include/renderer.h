#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "types.h"
#include "plane.h"
#include "sphere.h"
#include "contact.h"
#include "box.h"
#include "polyhedron.h"

namespace physics
{
  class AABB;

  class Renderer
  {
  public:
    static void setColor(float r, float g, float b);
    static void renderAABB(const AABB &o);
    static void renderContact(Contact *p);
    static void renderPlane(Plane *p);
    static void renderSphere(Sphere *p);
    static void renderBox(Box *p);
    static void renderPolyhedron(Polyhedron *p);
  };
}


#endif