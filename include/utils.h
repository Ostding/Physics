#ifndef __UTILS_H__
#define __UTILS_H__

#include "types.h"
#include "vector3.h"

namespace physics
{
  class Utils
  {
    public:
      ffloat distPointToPlane(const Vector3 &pt, const Vector3 &ptOnPlane,  const Vector3 &planeNormal);
      ffloat distPointToSeg(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB);
      ffloat squareDistPointToSeg(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB);
  };
}


#endif