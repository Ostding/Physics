#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "types.h"
#include "plane.h"
#include "sphere.h"
#include "contact.h"

namespace physics
{
  class AABB;

  class Renderer
  {
  public:
    static void renderAABB(const AABB &o);
    static void renderPlane(Plane *p);
    static void renderSphere(Sphere *p);
    static void renderContact(Contact *p);
  };
}


#endif