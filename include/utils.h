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
      static const ffloat SMALL_NUM;
      //Calculate smallest distance between point and plane
      //Note: normal must be normalised vector
      static ffloat distPointToPlane(const Vector3 &pt, const Vector3 &ptOnPlane,  const Vector3 &planeNormal);
      
      //Calculate smallest distance between point and segement
      static ffloat distPointToSeg(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB);
      
      //Calculate smallest square distance between point and segement
      static ffloat squareDistPointToSeg(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB);
      
      //Simply calculate the closest project point on the given segment
      static Vector3 closestPointOnSegment(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB);

      //Calculate point's projection position on a sgement
      //If there is no projection(which means the projection is outside the segment) the function return false, and distance holds the smallest square distance 
      //If the projection exists, the funciton willl return true, ptProjection holds the projection point, and distance holds the smallest square distance
      static bool pointProjectionToSegment(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB, 
                                            Vector3 &ptProjection, ffloat &distance);

      //Algorithm refer to http://mathworld.wolfram.com/BarycentricCoordinates.html
      static bool pointInTriangle(const Vector3 &pt, const Vector3 &ptA, const Vector3 &ptB, const Vector3 &ptC);
      
      //This function could finde farthest point  along the given direction which within given points
      static Vector3 findFarthestPointInDirection(const Vector3 &dir, std::vector<Vector3> &points);

      //Calculate box's projection length on an axis
      //param extents is box bounding size, transform is box transform matrix
      static ffloat calcProjectionLengthOnAxis(const Vector3 &extents, const Matrix4 &transform, const Vector3 &axis);

      //This ia a geometry solution, we can find the position of the closest points, judge whether the point is on the line segment
      //return value is the smallest square distance, out params pa and pb are used to receive closest points on segment A and B
      static ffloat squareDistanceSegment2Segments(const Vector3 &pa0, const Vector3 &pa1, const Vector3 &pb0, const Vector3 &pb1,
                                                    Vector3 &pa, Vector3 &pb);
      
      //This is a geometry solution, we can find the position of the closest points
      //return value is the smallest square distance, out params pa and pb are used to receive closest points on line A and B
      static ffloat squareDistanceLine2Line(const Vector3 &pa0, const Vector3 &pa1, const Vector3 &pb0, const Vector3 &pb1,
                                            Vector3 &pa, Vector3 &pb);
  };
}


#endif