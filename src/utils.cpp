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
    ffsqrt(squareDistPointToSeg(pt, ptSegA, ptSegB));
  }

  ffloat Utils::squareDistPointToSeg(const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB)
  {
    Vector3 va = pt - ptSegA;
    Vector3 vb = ptSegB - ptSegA;
    ffloat value = va.dot(vb);
    if(value <= ffzero)
      return va.squareMag();

    ffloat squareProject = value * value;
    if( squareProject >= vb.squareMag())
      return (pt - ptSegB).squareMag();

    return (va.squareMag() - squareProject);
  }

   bool pointProjectionToSeg( const Vector3 &pt, const Vector3 &ptSegA, const Vector3 &ptSegB, 
                              Vector3 &ptProjection, ffloat &distance)
  {
    Vector3 va = pt - ptSegA;
    Vector3 vb = ptSegB - ptSegA;
    ffloat value = va.dot(vb);
    if(value <= ffzero)
    {
      distance = va.squareMag();
      return false;
    }

    ffloat squareProject = value * value;
    if( squareProject >= vb.squareMag())
    {
      distance = (pt - ptSegB).squareMag();
      return false;
    }

    distance = (va.squareMag() - squareProject);
    ptProjection = ptSegA + vb.scale(ffsqrt(distance/vb.squareMag()));
    return true;
  }

}