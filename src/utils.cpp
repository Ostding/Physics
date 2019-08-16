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

  // This ia a geometry solution, we can find the position of the point, judge whether the point is on the line segment
  // Algorithm：
  // There are two lines s、t, s0、t0 is the start points, direction is u、v
  // The two closest points：on line s it is s0+sc*u，on line t it is t0+tc*v
  // w = (s0+sc*u)-(t0+tc*v), set w0 = s0-t0
  // a=u*u，b=u*v，c=v*v，d=u*w0，e=v*w0   (fa)
  // as 
  // u*w=0 v*w=0，w=-tc*v+w0+sc*u
  // so
  // (u*u)*sc - (u*v)*tc = -u*w0  (f1)
  // (v*u)*sc - (v*v)*tc = -v*w0  (f2)
  // as 
  // (fa) 
  // so 
  //  sc=(be-cd)/(ac-b2)、tc=(ae-bd)/(ac-b2) （fb）
  // We could found ac-b2=|u|2|v|2-(|u||v|cosq)2=(|u||v|sinq)2 is always positive
  // so according to（fb）, we could judge the colsest points whether on segments or not by sc、tc and sc、tc
  // ac-b2=0 means two lins are parallel, set sc=0 could get tc
  // Finally closest distance d(L1、L2)=|（P0-Q0)+[(be-cd)*u-(ae-bd)v]/(ac-b2)|
  ffloat Utils::smallestSquareDistanceOfTwoSegments(const Vector3 &pa0, const Vector3 &pa1, const Vector3 &pb0, const Vector3 &pb1,
                                                        Vector3 &pa, Vector3 &pb)
  {
    ffloat ux = pa1.x - pa0.x;
    ffloat uy = pa1.y - pa0.y;
    ffloat uz = pa1.z - pa0.z;

    ffloat vx = pb1.x - pb0.x;;
    ffloat vy = pb1.y - pb0.y;;
    ffloat vz = pb1.z - pb0.z;;

    ffloat wx = pa1.x - pb1.x;
    ffloat wy = pa1.y - pb1.y;
    ffloat wz = pa1.z - pb1.z;

    ffloat a = (ux * ux + uy * uy + uz * uz); //u*u
    ffloat b = (ux * vx + uy * vy + uz * vz); //u*v
    ffloat c = (vx * vx + vy * vy + vz * vz); //v*v
    ffloat d = (ux * wx + uy * wy + uz * wz); //u*w 
    ffloat e = (vx * wx + vy * wy + vz * wz); //v*w
    ffloat dt = a * c - b * b;

    ffloat sd = dt;
    ffloat td = dt;

    ffloat sn = ffzero;  //sn = be-cd
    ffloat tn = ffzero;  //tn = ae-bd

    //Two lines are parallel
    if (dt == ffzero)
    {
      sn = ffzero;    //Get point s0 on line s
      sd = ffone;   //Avoid divide 0

      tn = e;      //Accroding (f2) to calculate tc
      td = c;
    }
    else
    {
      sn = (b * e - c * d);
      tn = (a * e - b * d);
      //The closest point is out of start point of line s
      if (sn < ffzero)
      {
        sn = ffzero;
        tn = e; //Accroding (f2) to calculate tc
        td = c;
      }
      //The closest point is out of end point of line s (as sc>1, so make sc=1)
      else if (sn > sd)
      {
        sn = sd;
        tn = e + b; //Accroding (f2) to calculate tc
        td = c;
      }
    }
    if (tn < ffzero)
    {
        //The closest point is out of start point of line t
        tn = ffzero;
        //According (f1)，if right of (f1) is nagetive，sc is nagetive too，so sc=0
        if (-d < ffzero) 
          sn = ffzero;
        //According (f1)，if sc is greater than 1，so sc=1
        else if (-d > a) 
          sn = sd;
        else
        {
          sn = -d;
          sd = a;
        }
    }
    else if (tn > td)
    {
      tn = td;
      if ((-d + b) < ffzero)
        sn = ffzero;
      else if ((-d + b) > a)
        sn = sd;
      else
      {
        sn = (-d + b);
        sd = a;
      }
    }

    ffloat sc;
    ffloat tc;

    if (sn == ffzero)
    {
      sc = ffzero;
      pa = pa1;
    }
    else
    {
      sc = sn / sd;
      //p = so + sc * u
      ffloat da = ffsqrt(a);
      pa.x = pa1.x + sc * ux / da;
      pa.y = pa1.y + sc * uy / da;
      pa.z = pa1.z + sc * uz / da;
    }

    if (tn == ffzero)
    {
      tc = ffzero;
      pb = pb1;
    }
    else
    {
      tc = tn / td;
      //p = to + tc * v
      ffloat db = ffsqrt(c);
      pb.x = pb1.x + tc * vx / db;
      pb.y = pb1.y + tc * vy / db;
      pb.z = pb1.z + tc * vz / db;
    }

    ffloat dx = wx + (sc * ux) - (tc * vx);
    ffloat dy = wy + (sc * uy) - (tc * vy);
    ffloat dz = wz + (sc * uz) - (tc * vz);
    return dx * dx + dy * dy + dz * dz;
  }
}