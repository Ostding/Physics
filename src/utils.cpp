#include "utils.h"

namespace physics
{
  const ffloat Utils::SMALL_NUM = ffloat(0.00001);

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

  ffloat Utils::squareDistanceOfTwoSegments(const Vector3 &pa0, const Vector3 &pa1, const Vector3 &pb0, const Vector3 &pb1,
                                                        Vector3 &pa, Vector3 &pb)
  {
    Vector3   u = pa1 - pa0;
    Vector3   v = pb1 - pb0;
    Vector3   w = pa0 - pb0;
    ffloat    a = Vector3::dot(u,u);    // always >= 0
    ffloat    b = Vector3::dot(u,v);
    ffloat    c = Vector3::dot(v,v);    // always >= 0
    ffloat    d = Vector3::dot(u,w);
    ffloat    e = Vector3::dot(v,w);
    ffloat    D = a*c - b*b;            // always >= 0
    ffloat    sc, sN, sD = D;           // sc = sN / sD, default sD = D >= 0
    ffloat    tc, tN, tD = D;           // tc = tN / tD, default tD = D >= 0
    
    // compute the line parameters of the two closest points
    if (D < SMALL_NUM) {    // the lines are almost parallel
        sN = ffzero;        // force using point P0 on segment S1
        sD = ffone;         // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    }
     // get the closest points on the infinite lines
    else {                
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        // sc < 0 => the s=0 edge is visible
        if (sN < ffzero) {        
            sN = ffzero;
            tN = e;
            tD = c;
        }
        // sc > 1  => the s=1 edge is visible
        else if (sN > sD) {  
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    // tc < 0 => the t=0 edge is visible
    if (tN < ffzero) {            
        tN = ffzero;
        // recompute sc for this edge
        if (-d < ffzero)
            sN = ffzero;
        else if (-d > a)
            sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    }
    // tc > 1  => the t=1 edge is visible
    else if (tN > tD) {      
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < ffzero)
            sN = ffzero;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d +  b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (ffabs(sN) < SMALL_NUM ? ffzero : sN / sD);
    tc = (ffabs(tN) < SMALL_NUM ? ffzero : tN / tD);

    // get the the two closest points
    pa = pa0 + (u * sc);
    pb = pb0 + (v * tc);

    return (pa - pb).squareMag(); 
  }
}