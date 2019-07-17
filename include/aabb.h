#ifndef __AABB_H__
#define __AABB_H__

#include "types.h"
#include "vector3.h"
#include "renderer.h"

namespace physics
{
  class AABB
  {
  public:
    Vector3 min;
    Vector3 max;

  public:
    AABB()
    {
      reset();
    }

    AABB(const Vector3 &min, const Vector3 &max) 
    {
      AABB::min = min;
      AABB::max = max;
    }

    ~AABB(){}

  public:
    AABB& operator = (const AABB &other)
    {
      set(other.min, other.max);
      return (*this);
    }

    void set(const Vector3 &min, const Vector3 &max)
    {
      AABB::min = min;
      AABB::max = max;
    }

    void set2(const Vector3 &center, const Vector3 &extents)
    {
      min = center - extents;
      max = center + extents;
    }

    void reset()
    {
      min.x = 0;
      min.y = 0;
      min.z = 0;

      max.x = 0;
      max.y = 0;
      max.z = 0;
    }

    Vector3 getCenter()
    {
      return (min + max) * ffhalf;
    }

    bool intersect(AABB &other)
    {
      if (min.x > other.max.x) return false;
      if (max.x < other.min.x) return false;
      if (min.y > other.max.y) return false;
      if (max.y < other.min.y) return false;
      if (min.z > other.max.z) return false;
      if (max.z < other.min.z) return false;
    
      return true;
    }

    bool isEmpty()
    {
      return ((min.x >= max.x) ||
              (min.y >= max.y) ||
              (min.z >= max.z));
    }

    void render()
    {
      Renderer::renderAABB(*this);
    }
  };
}


#endif