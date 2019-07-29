#include "polyhedron.h"
#include "renderer.h"

namespace physics
{
    Polyhedron::Polyhedron(RigidBody *body)
    :Primitive(PRIMITIVE_TYPE::PRIT_POLYHEDRON)
    {
      Polyhedron::body = body;
    }
    
    Polyhedron::Polyhedron()
    :Primitive(PRIMITIVE_TYPE::PRIT_POLYHEDRON)
    {
      body = new RigidBody(this);
    }

    void Polyhedron::addPoints(Vector3 *points, unsigned count)
    {
      for(unsigned i = 0; i < count; i++)
      {
        pointsLocal.emplace_back(*(points+i));
        pointsWorld.emplace_back(Vector3::zero);
      }
    }

    void Polyhedron::refreshAABB()
    {
      Vector3 pt = body->getPosInWorldSpace(pointsLocal[0]);
      ffloat xMax = pt.x, xMin = pt.x;
      ffloat yMax = pt.y, yMin = pt.y;
      ffloat zMax = pt.z, zMin = pt.z;
      for (int i = 1; i < pointsLocal.size(); i++)
      {
        pt = body->getPosInWorldSpace(pointsLocal[i]);
        pointsWorld[i] = pt;

        if (pt.x > xMax)
          xMax = pt.x;
        if (pt.x < xMin)
          xMin = pt.x;

        if (pt.y > yMax)
          yMax = pt.y;
        if (pt.y < yMin)
          yMin = pt.y;

        if (pt.z > zMax)
          zMax = pt.z;
        if (pt.z < zMin)
          zMin = pt.z;
      }

      aabb.set(Vector3(xMin, yMin, zMin), Vector3(xMax, yMax, zMax));
    }
    
    void Polyhedron::setPosition(const Vector3 &position)
    {
      body->setPosition(position);
      body->updateDerivedData();

      refreshAABB();
      updateTransform();
    }
    
    void Polyhedron::setOrientation(const Quaternion &direction)
    {
      body->setOrientation(direction);
      body->updateDerivedData();

      refreshAABB();
      updateTransform();
    }

    void Polyhedron::findFarthestPointInDirection(const Vector3 &direction, Vector3 &pointLocal, Vector3 &pointWorld)
    {
      int i = 0;
      Vector3 dirLocal = body->getDirectionInBodySpace(direction);
      ffloat maxValue = pointsLocal[0].dot(dirLocal) * pointsLocal[0].squareMag();
      for (int j = 1; j < pointsLocal.size(); j++)
      {
        ffloat value = pointsLocal[j].dot(dirLocal) * pointsLocal[j].squareMag();
        if (maxValue < value)
        {
          i = j;
          maxValue = value;
        }
      }
      pointLocal = pointsLocal[i];
      pointWorld = pointsWorld[i];
    }

    void Polyhedron::render()
    {
      Renderer::renderPolyhedron(this);
    }

} 
