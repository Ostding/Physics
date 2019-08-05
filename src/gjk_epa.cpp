#include "gjk_epa.h"
#include <vector>
using namespace std;
namespace physics
{
  bool GjkEpa::generateContacts(Primitive *cpa, Primitive *cpb, CollisionData *cData)
  {
    Simplex simplex;

    Vector3 searchDir = Vector3::one;
    SupportPoint sp = SupportPoint::support(cpa, cpb, searchDir);
    if (searchDir.dot(sp.minkowskiPoint) >= sp.minkowskiPoint.mag() * ffloat(80000000LL)) //0.8
    {
      searchDir = Vector3::dy;
      sp = SupportPoint::support(cpa, cpb, searchDir);
    }
    simplex.push(sp);

    searchDir = -searchDir;

    const unsigned iterationMax = 32;
    unsigned iterationCount = 0;
    while (true)
    {
      if (iterationCount++ >= iterationMax)
      {
        printf(">>>>>>Gjk max count:%d \n", iterationCount);
        return false;
      }

      // static ffloat _MIN_SMAG_DIR = ffloat(10000LL); //0.0001f
      // if (searchDir.squareMag() <= _MIN_SMAG_DIR)
      // {
      //   printf(">>>>>>Gjk exit 2 iterationCount:%d\n", iterationCount);
      //   return false;
      // }

      SupportPoint newSp = SupportPoint::support(cpa, cpb, searchDir);
      simplex.push(newSp);

      ffloat x = newSp.minkowskiPoint.dot(searchDir);
      if (x < ffzero)
      {
        printf(">>>>>>Gjk exit 1 iterationCount:%d dir(%.3f %.3f %.3f) pt(%.3f %.3f %.3f) \n", 
                iterationCount, searchDir.x.to_d(), searchDir.y.to_d(), searchDir.z.to_d(),
                newSp.minkowskiPoint.x.to_d(), newSp.minkowskiPoint.y.to_d(), newSp.minkowskiPoint.z.to_d());
        return false;
      }
      else
      {
        if (simplex.isContainOrigin(searchDir))
        {
          printf(">>>>>>Gjk iteration count:%d \n", iterationCount);
          // Run contact detection when collision is detected
          return epaContactDetection(simplex, cpa, cpb, cData);
        }
      }
    }
  }

  bool GjkEpa::epaContactDetection(Simplex & aSimplex, Primitive * cpa, Primitive * cpb, CollisionData * cData)
  {
    const ffloat exitThreshold = ffloat(10000000LL);//0.1f
    const unsigned iterationMax = 32;
    unsigned iterationCount = 0;

    std::list<PolytopeFace> polytopeFaces;
    std::list<PolytopeEdge> polytopeEdges;

    // Add all faces of simplex to the polytope
    polytopeFaces.emplace_back(aSimplex.a, aSimplex.b, aSimplex.c);
    polytopeFaces.emplace_back(aSimplex.a, aSimplex.c, aSimplex.d);
    polytopeFaces.emplace_back(aSimplex.a, aSimplex.d, aSimplex.b);
    polytopeFaces.emplace_back(aSimplex.b, aSimplex.d, aSimplex.c);

    while (true)
    {
      if (iterationCount++ >= iterationMax)
        return false;

      // Find the closest face to origin (i.e. projection of any vertex along its face normal with the least value)
      ffloat minimumDistance = ffmax;
      std::list<PolytopeFace>::iterator closestFace = polytopeFaces.begin();
      for (auto iterator = polytopeFaces.begin(); iterator != polytopeFaces.end(); ++iterator)
      {
        ffloat distance = iterator->faceNormal.dot(iterator->points[0].minkowskiPoint);
        if (distance < minimumDistance)
        {
          minimumDistance = distance;
          closestFace = iterator;
        }
      }

      // With the closest face now known, find new support point on the Minkowski Hull using normal to that face
      SupportPoint newPolytopePoint = SupportPoint::support(cpa, cpb, closestFace->faceNormal);

      // If this new point is within a tolerable limit of the origin, 
      // assume we have found the closest triangle on the Minkowski Hull to the origin
      if ((closestFace->faceNormal.dot(newPolytopePoint.minkowskiPoint) - minimumDistance) < exitThreshold)
      {
        printf(">>>>>>Epa iteration count:%d \n", iterationCount);
        return extrapolateContactInformation(&(*closestFace), cpa, cpb, cData);
      }

      // Otherwise, check what faces can be 'seen' from the newly added support point and delete them from the polytope
      for (auto iterator = polytopeFaces.begin(); iterator != polytopeFaces.end();)
      {
        // A face is considered as "seen" if the new support point is on the positive halfspace of the plane defined by it
        Vector3 planeVector = newPolytopePoint.minkowskiPoint - iterator->points[0].minkowskiPoint;

        if (iterator->faceNormal.dot(planeVector) > ffzero)
        {
          // Only adds the outside edges of the 'seen' face, and ignores the others
          polytopeAddEdge(polytopeEdges, iterator->points[0], iterator->points[1]);
          polytopeAddEdge(polytopeEdges, iterator->points[1], iterator->points[2]);
          polytopeAddEdge(polytopeEdges, iterator->points[2], iterator->points[0]);
          // Remove the face from the Polytope
          iterator = polytopeFaces.erase(iterator);
          continue;
        }
        ++iterator;
      }

      // Create new polytope faces using the new support point from the valid edges in the edge list
      for (auto iterator = polytopeEdges.begin(); iterator != polytopeEdges.end(); ++iterator)
        polytopeFaces.emplace_back(newPolytopePoint, iterator->points[0], iterator->points[1]);

      // Clear the edge list every iteration of the expansion
      polytopeEdges.clear();
    }
  }

  bool GjkEpa::extrapolateContactInformation(PolytopeFace * aClosestFace, Primitive * cpa, Primitive * cpb, CollisionData  * cData)
  {
    ffloat distanceFromOrigin = aClosestFace->faceNormal.dot(aClosestFace->points[0].minkowskiPoint);

    // calculate the barycentric coordinates of the closest triangle with respect to
    // the projection of the origin onto the triangle
    ffloat bary_u, bary_v, bary_w;
    Vector3 v = aClosestFace->faceNormal * distanceFromOrigin;
    bool suc = barycentricProjection(v,
      aClosestFace->points[0].minkowskiPoint,
      aClosestFace->points[1].minkowskiPoint,
      aClosestFace->points[2].minkowskiPoint,
      bary_u, bary_v, bary_w);
    if (!suc)
      return false;

    // If any of the barycentric coefficients have a magnitude greater than 1 or lesser than and equal to 0, 
    // then the origin is not within the triangular prism described by 'triangle'
    // thus, there is no collision here, return false
    if (ffabs(bary_u) > ffone || ffabs(bary_v) > ffone || ffabs(bary_w) > ffone)
      return false;

    // distanceFromOrigin = distanceFromOrigin * aClosestFace->points[0].minkowskiPoint.mag();
    // A Contact points
    Vector3 supportLocal1 = aClosestFace->points[0].localPointA;
    Vector3 supportLocal2 = aClosestFace->points[1].localPointA;
    Vector3 supportLocal3 = aClosestFace->points[2].localPointA;

    aClosestFace->faceNormal.normalise();
    Vector3 normal = -aClosestFace->faceNormal;
    Vector3 ptLocalA = (supportLocal1 * bary_u) + (supportLocal2 * bary_v) + (supportLocal3 * bary_w);
    Vector3 contactPoint = cpa->getPointInWorldSpace(ptLocalA);

    Contact* contact = cData->nextContact;
    contact->penetration = distanceFromOrigin;
    contact->contactNormal = normal;
    contact->contactPoint = contactPoint;

    printf(">>>pen:%.3f\n", distanceFromOrigin.to_d());

    contact->setBodyData(cpa->body, cpb->body, cData->friction, cData->restitution);
    cData->addContacts(1);
    return true;
  }
}