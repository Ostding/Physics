#ifndef __UTILS_H__
#define __UTILS_H__

#include "types.h"
#include "vector3.h"

namespace physics
{
  class Utils
  {
    public:
      //Calculate smallest distance between point and plane
      ffloat distPointToPlane(const Vector3 &pt, const Vector3 &ptOnPlane,  const Vector3 &planeNormal);
      //Calculate smallest distance between point and segement
      ffloat distPointToSeg(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB);
      //Calculate smallest square distance between point and segement
      ffloat squareDistPointToSeg(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB);
      //Calculate point's projection position on a sgement
      //If there is no projection(which means the projection is outside the segment) the function return false, and distance holds the smallest distance 
      //If the projection exists, the funciton willl return true, ptProjection holds the projection point, and distance holds the smallest distance
      bool pointProjectionToSeg(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB, 
                                Vector3 &ptProjection, ffloat &distance);
                                
  };
}


#endif