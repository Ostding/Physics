#ifndef __SIMPLEX_H__
#define __SIMPLEX_H__
#include <algorithm>
#include <list>
using namespace std;
#include "types.h"
#include "vector3.h"
#include "primitive.h"

namespace physics
{
  struct SupportPoint
  {
    Vector3 minkowskiPoint;

    Vector3 world_SupportPointA;
    Vector3 world_SupportPointB;

    Vector3 local_SupportPointA;
    Vector3 local_SupportPointB;
    bool operator == (const SupportPoint & ref) { return minkowskiPoint == ref.minkowskiPoint; }
  
    static SupportPoint support(Primitive *pri1, Primitive *pri2, Vector3 direction)
    {
      direction.normalise();
      SupportPoint newSupportPoint;
      newSupportPoint.local_SupportPointA = pri1->findFarthestPointInDirection(direction);
      newSupportPoint.local_SupportPointB = pri2->findFarthestPointInDirection(-direction);

      newSupportPoint.world_SupportPointA = pri1->getPointInWorldSpace(newSupportPoint.local_SupportPointA);
      newSupportPoint.world_SupportPointB = pri2->getPointInWorldSpace(newSupportPoint.local_SupportPointB);
      
      newSupportPoint.minkowskiPoint = newSupportPoint.world_SupportPointA - newSupportPoint.world_SupportPointB;
      return newSupportPoint;
    };
  };

  struct Point
	{
		Vector3 position;
		Point(const Vector3 &p) : position(p)
		{}
	};

  struct LineSegment
	{
		Point A;
		Point B;
		LineSegment(const Vector3 &a, const Vector3 &b) : A(a), B(b)
		{}

		inline Point closestPointOnLineFromPoint(const Point &pt, ffloat &u, ffloat &v)
		{
			Vector3 line = (B.position - A.position);
			ffloat len = line.mag();
			line.normalise();

			v = (-A.position).dot(line) / len;
			u = B.position.dot(line) / len;

			Vector3 closestPt;
			if (u <= ffzero)
			{
				return Point(B.position);
			}
			else if (v <= ffzero)
			{
				return Point(A.position);
			}
			else
			{
				return Point(A.position * u + B.position * v);
			}
		}
	};

  struct Simplex
	{
		SupportPoint points[4];
		int size = 0;

		SupportPoint &a;
		SupportPoint &b;
		SupportPoint &c;
		SupportPoint &d;

		inline void clear() { size = 0; }

		Simplex() :
			a(points[0]),
			b(points[1]),
			c(points[2]),
			d(points[3])
		{}


		inline void set(SupportPoint a, SupportPoint b, SupportPoint c, SupportPoint d) { size = 4, this->a = a; this->b = b; this->c = c; this->d = d; }
		inline void set(SupportPoint a, SupportPoint b, SupportPoint c) { size = 3, this->a = a; this->b = b; this->c = c; }
		inline void set(SupportPoint a, SupportPoint b) { size = 2, this->a = a; this->b = b; }
		inline void set(SupportPoint a) { size = 1, this->a = a; }

		// Most recently added point is always added to the start of the list.
		// With every insertion all existing entries are shifted one to the right
		inline void push(SupportPoint newPoint)
		{
			size = std::min(size + 1, 4);

			for (int i = size - 1; i > 0; i--)
				points[i] = points[i - 1];

			points[0] = newPoint;
		}

		inline bool isContainOrigin(Vector3 &searchDir)
		{
			// line case
			if (size == 2)
			{
				LineSegment line(a.minkowskiPoint, b.minkowskiPoint);
				ffloat u = ffzero;
				ffloat v = ffzero;

				Vector3 origin = Vector3::zero;
				Point closestPoint = line.closestPointOnLineFromPoint(origin, u, v);
				if (v <= ffzero)
				{
					set(a);
					searchDir = -closestPoint.position;
					return false;
				}
				else if (u <= ffzero)
				{
					set(b);
					searchDir = -closestPoint.position;
					return false;
				}
				else
				{
					searchDir = -closestPoint.position;
					return false;
				}
			}
			else if (size == 3)
			{
				Vector3 newPointToOrigin = -a.minkowskiPoint;
				Vector3 edge1 = b.minkowskiPoint - a.minkowskiPoint;
				Vector3 edge2 = c.minkowskiPoint - a.minkowskiPoint;

				Vector3 triangleNormal = edge1.cross(edge2);
				Vector3 edge1Normal = edge1.cross(triangleNormal);
				Vector3 edge2Normal = triangleNormal.cross(edge2);

				if (edge2Normal.dot(newPointToOrigin) > ffzero)
				{
					if (edge2.dot(newPointToOrigin) > ffzero)
					{
						searchDir = Vector3::tripleCross(edge2, newPointToOrigin, edge2);
						//return as [A,C]
						clear();
						set(a, c);
						return false;
					}
					else
					{
						if (edge1.dot(newPointToOrigin) > ffzero)
						{
							searchDir = Vector3::tripleCross(edge1, newPointToOrigin, edge1);
							//return as [A,B]
							clear();
							set(a, b);
							return false;
						}
						else
						{
							searchDir = newPointToOrigin;
							//return a point A
							clear();
							set(a);
							return false;
						}
					}
				}
				else
				{
					//If closer to the first edge normal
					if (edge1Normal.dot(newPointToOrigin) > ffzero)
					{
						//If new search direction should be along edge normal
						if (edge1.dot(newPointToOrigin) > ffzero)
						{
							searchDir = Vector3::tripleCross(edge1, newPointToOrigin, edge1);
							// Return it as [A, B]
							clear();
							set(a, b);
							return false;
						}
						else
						{
							searchDir = newPointToOrigin;
							clear();
							set(a);
							return false;
						}
					}
					else
					{
						// Check if it is above the triangle
						if (triangleNormal.dot(newPointToOrigin) > ffzero)
						{
							searchDir = triangleNormal;
							return false;
						}
						else
						{
							searchDir = -triangleNormal;
							set(a, c, b);
							return false;
						}
					}
				}
			}
			else if (size == 4)
			{
				Vector3 edge1 = b.minkowskiPoint - a.minkowskiPoint;
				Vector3 edge2 = c.minkowskiPoint - a.minkowskiPoint;
				Vector3 edge3 = d.minkowskiPoint - a.minkowskiPoint;

				Vector3 face1Normal = edge1.cross(edge2);
				Vector3 face2Normal = edge2.cross(edge3);
				Vector3 face3Normal = edge3.cross(edge1);

				Vector3 newPointToOrigin = -a.minkowskiPoint;
				if (face1Normal.dot(newPointToOrigin) > ffzero)
				{
					// Origin is in front of first face, simplex is correct already
					goto proc;
				}

				if (face2Normal.dot(newPointToOrigin) > ffzero)
				{
					// Origin is in front of second face, simplex is set to this triangle [A, C, D]
					clear();
					set(a, c, d);
					goto proc;
				}

				if (face3Normal.dot(newPointToOrigin) > ffzero)
				{
					// Origin is in front of third face, simplex is set to this triangle [A, D, B]
					clear();
					set(a, d, b);
					goto proc;
				}
				// If reached here it means the simplex MUST contain the origin, intersection confirmed
				return true;

			proc:

				Vector3 edge1Normal = edge1.cross(face1Normal);
				if (edge1Normal.dot(newPointToOrigin) > ffzero)
				{
					searchDir = Vector3::tripleCross(edge1, newPointToOrigin, edge1);
					// Origin is along the normal of edge1, set the simplex to that edge [A, B]
					clear();
					set(a, b);
					return false;
				}

				Vector3 edge2Normal = face1Normal.cross(edge2);
				if (edge2Normal.dot(newPointToOrigin) > ffzero)
				{
					searchDir = Vector3::tripleCross(edge2, newPointToOrigin, edge2);
					// Origin is along the normal of edge2, set the simplex to that edge [A, C]
					clear();
					set(a, c);
					return false;
				}

				searchDir = face1Normal;
				// If reached here the origin is along the first face normal, set the simplex to this face [A, B, C]
				clear();
				set(a, b, c);
				return false;
			}
			return false;
		}
	};

	struct PolytopeFace
	{
		SupportPoint points[3];
		Vector3 faceNormal;
		PolytopeFace(const SupportPoint & supportA, const SupportPoint & supportB, const SupportPoint & supportC)
		{
			points[0] = supportA;
			points[1] = supportB;
			points[2] = supportC;
			Vector3 edge1 = supportB.minkowskiPoint - supportA.minkowskiPoint;
			Vector3 edge2 = supportC.minkowskiPoint - supportA.minkowskiPoint;
			faceNormal = edge1.cross(edge2);
			faceNormal.normalise();
		}
	};


	struct PolytopeEdge
	{
		SupportPoint points[2];

		PolytopeEdge(const SupportPoint & supportA, const SupportPoint & supportB)
		{
			points[0] = supportA;
			points[1] = supportB;
		}
	};

	// Process the specified edge, if another edge with the same points in the
	// opposite order exists then it is removed and the new point is also not added
	// "The trick is that if two triangles that are removed share an edge then that edge is gone for good, 
	// but if an edge is used by only one removed triangle then it will form part of the hole."
	inline void polytopeAddEdge(std::list<PolytopeEdge> & aEdgeList, const SupportPoint & a, const SupportPoint & b)
	{
		for (auto iterator = aEdgeList.begin(); iterator != aEdgeList.end(); ++iterator)
		{
			if (iterator->points[0] == b && iterator->points[1] == a)
			{
				// Encountered the same edge with opposite winding, remove it and don't add a new one
				aEdgeList.erase(iterator);
				return;
			}
		}
		aEdgeList.emplace_back(a, b);
	}

	// Code from Christer Ericson's Real-Time Collision Detection
	// Compute barycentric coordinates (u, v, w) for
	// point p with respect to triangle (a, b, c)
	inline bool barycentricProjection(Vector3 & aPoint, Vector3 & a, Vector3 & b, Vector3 & c, ffloat & u, ffloat & v, ffloat & w)
	{
		Vector3 v0 = b - a, v1 = c - a, v2 = aPoint - a;
		ffloat d00 = v0.dot(v0);
		ffloat d01 = v0.dot(v1);
		ffloat d11 = v1.dot(v1);
		ffloat d20 = v2.dot(v0);
		ffloat d21 = v2.dot(v1);

		ffloat den = d00 * d11 - d01 * d01;
		if (den == ffzero)
			return false;

		v = (d11 * d20 - d01 * d21) / den;
		w = (d00 * d21 - d01 * d20) / den;
		u = ffone - v - w;

		return true;
	}

}

#endif