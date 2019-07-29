#include "utils.h"

namespace physics
{
  ffloat Utils::distPointToPlane(const Vector3 &pt, const Vector3 &ptOnPlane,  const Vector3 &planeNormal)
  {
    Vector3 v = (pt - ptOnPlane);
    ffloat dist = v.dot(planeNormal);
    return dist;
  }

  ffloat Utils::distPointToSeg(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB)
  {
    return ffsqrt(squareDistPointToSeg(pt, ptSegA, ptSegB));
  }

  ffloat Utils::squareDistPointToSeg(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB)
  {
    Vector3 va = pt - ptSegA;
    Vector3 vb = ptSegB - ptSegA;
    ffloat magB = vb.mag();
    ffloat projLen = va.dot(vb)/magB;
    if(projLen <= ffzero)
      return va.squareMag();

    if( projLen >= magB)
      return (pt - ptSegB).squareMag();

    return (va.squareMag() - projLen * projLen);
  }

  Vector3 Utils::closestPointOnSegment(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB)
  {
    Vector3 line = (ptSegB - ptSegA);
    ffloat len = line.mag();
    line.normalise();

    ffloat v = (-ptSegA.dot(line)) / len;
    ffloat u = ptSegB.dot(line) / len;

    Vector3 closestPt;
    if (u <= ffzero)
    {
      return ptSegB;
    }
    else if (v <= ffzero)
    {
      return ptSegA;
    }
    else
    {
      return (ptSegA * u + ptSegB * v);
    }
  }

  bool Utils::pointProjectionToSegment( const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB, 
                              Vector3 &ptProjection, ffloat &distance)
  {
    Vector3 va = pt - ptSegA;
    Vector3 vb = ptSegB - ptSegA;
    ffloat magB = vb.mag();
    ffloat projLen = va.dot(vb)/magB;
    if(projLen <= ffzero)
    {
      distance = va.mag();
      ptProjection = ptSegA;
      return false;
    }

    if( projLen >= magB)
    {
      distance = (pt - ptSegB).mag();
      ptProjection = ptSegB;
      return false;
    }

    distance = ffsqrt(va.squareMag() - projLen * projLen);
    ptProjection = ptSegA + vb.scale(projLen/magB);
    return true;
  }

  bool Utils::pointInTriangle(const Vector3 &pt, const Vector3 &ptA, const Vector3 &ptB, const Vector3 &ptC)
  {
    Vector3 v0 = ptC - ptA ;
    Vector3 v1 = ptB - ptA ;
    Vector3 v2 = pt - ptA ;

    ffloat dot00 = v0.dot(v0) ;
    ffloat dot01 = v0.dot(v1) ;
    ffloat dot02 = v0.dot(v2) ;
    ffloat dot11 = v1.dot(v1) ;
    ffloat dot12 = v1.dot(v2) ;

    ffloat deno = (dot00 * dot11 - dot01 * dot01) ;

    ffloat u = (dot11 * dot02 - dot01 * dot12) / deno ;
    if (u < ffzero || u > ffone) 
      return false ;

    ffloat v = (dot00 * dot12 - dot01 * dot02) / deno ;
    if (v < ffzero || v > ffone) 
      return false ;

    return (u + v) <= ffone ;
  }

  Vector3 Utils::findFarthestPointInDirection(const Vector3 &dir, std::vector<Vector3> &points)
  {
    int fi = 0;
    ffloat maxValue = points[0].dot(dir) * points[0].squareMag();
    for (int i = 1; i < points.size(); i++)
    {
      ffloat value = points[i].dot(dir) * points[i].squareMag();
      if (maxValue < value)
      {
        fi = i;
        maxValue = value;
      }
    }
    return points[fi];
  }

  ffloat Utils::calcProjectionLengthOnAxis(const Vector3 &extents, const Matrix4 &transform, const Vector3 &axis)
  { 
    return  extents.x * ffabs(axis.dot(transform.getColumnVector(0))) +
            extents.y * ffabs(axis.dot(transform.getColumnVector(1))) +
            extents.z * ffabs(axis.dot(transform.getColumnVector(2)));
  }

}