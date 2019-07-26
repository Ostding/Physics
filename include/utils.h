#ifndef __UTILS_H__
#define __UTILS_H__
#include <vector>
using namespace std;
#include "types.h"
#include "vector3.h"
#include "matrix4.h"

namespace physics
{
  class Utils
  {
    public:
      //Calculate smallest distance between point and plane
      //Note: normal must be normalized vector
      static ffloat distPointToPlane(const Vector3 &pt, const Vector3 &ptOnPlane,  const Vector3 &planeNormal);
      
      //Calculate smallest distance between point and segement
      static ffloat distPointToSeg(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB);
      
      //Calculate smallest square distance between point and segement
      static ffloat squareDistPointToSeg(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB);
      
      //Calculate point's projection position on a sgement
      //If there is no projection(which means the projection is outside the segment) the function return false, and distance holds the smallest distance 
      //If the projection exists, the funciton willl return true, ptProjection holds the projection point, and distance holds the smallest distance
      static bool pointProjectionToSeg(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB, 
                                Vector3 &ptProjection, ffloat &distance);

      //Algorithm refer to http://mathworld.wolfram.com/BarycentricCoordinates.html
      static bool pointInTriangle(const Vector3 &pt, const Vector3 &ptA, const Vector3 &ptB, const Vector3 &ptC);
      
      static Vector3 findFarthestPointInDirection(const Vector3 &dir, std::vector<Vector3> &points);

      static ffloat calcProjectionLengthOnAxis(const Vector3 &extents, const Matrix4 &transform, const Vector3 &axis);
  };
}


#endif