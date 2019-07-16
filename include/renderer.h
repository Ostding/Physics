#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "types.h"


namespace physics
{
  class AABB;

  class Renderer
  {
  public:
    #define COLOR_AABB glColor3f(0, 1, 0);
    static void renderAABB(const AABB &o);

  };
}


#endif