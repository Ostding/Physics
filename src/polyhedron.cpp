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

#define Subexpressions(w0,w1,w2,f1,f2,f3,g0,g1,g2) \
        tmp0 = w0+w1; \
        ffloat f1 = tmp0+w2; \
        tmp1 = w0*w0; \
        tmp2 = tmp1+w1*tmp0; \
        ffloat f2 = tmp2+w2*f1; \
        ffloat f3 = w0*tmp1+w1*tmp2+w2*f2; \
        ffloat g0 = f2+w0*(f1+w0); \
        ffloat g1 = f2+w1*(f1+w1); \
        ffloat g2 = f2+w2*(f1+w2);\
         
    void Polyhedron::calculateInertiaTensor(Points &points, Indecies &indices, ffloat &mass, 
                                            Vector3 &massCenter, Matrix3 &inertiaTensor)
    {
      //{1/6,1/24,1/24,1/24,1/60,1/60,1/60,1/120,1/120,1/120}
      const ffloat mult[10] = {ffloat(16666667LL),
                              ffloat(4166666LL),ffloat(4166666LL),ffloat(4166666LL),
                              ffloat(1666666LL),ffloat(1666666LL),ffloat(1666666LL),
                              ffloat(833333LL),ffloat(833333LL),ffloat(833333LL)};
      // order: 1, x, y, z, x^2, y^2, z^2, xy, yz, zx
      ffloat intg[10] = {ffzero, ffzero, ffzero, ffzero, ffzero, ffzero, ffzero, ffzero, ffzero, ffzero} ; 
      unsigned triangleCount = indices.size() / 3;
      for (unsigned t = 0; t < triangleCount; t++)
      {
        // get vertices of triangle t
        unsigned i0 = indices[3*t]; 
        unsigned i1 = indices[3*t+1];
        unsigned i2 = indices[3*t+2];

        ffloat x0 = points[i0].x;
        ffloat y0 = points[i0].y;
        ffloat z0 = points[i0].z;

        ffloat x1 = points[i1].x;
        ffloat y1 = points[i1].y;
        ffloat z1 = points[i1].z;

        ffloat x2 = points[i2].x;
        ffloat y2 = points[i2].y;
        ffloat z2 = points[i2].z;

        // get edges and cross product of edges
        ffloat a1 = x1-x0; 
        ffloat b1 = y1-y0;
        ffloat c1 = z1-z0; 
        ffloat a2 = x2-x0; 
        ffloat b2 = y2-y0; 
        ffloat c2 = z2-z0;
        ffloat d0 = b1*c2-b2*c1; 
        ffloat d1 = a2*c1-a1*c2; 
        ffloat d2 = a1*b2-a2*b1;

        // compute integral terms
        ffloat tmp0, tmp1, tmp2;
        Subexpressions(x0,x1,x2,f1x,f2x,f3x,g0x,g1x,g2x)
        Subexpressions(y0,y1,y2,f1y,f2y,f3y,g0y,g1y,g2y)
        Subexpressions(z0,z1,z2,f1z,f2z,f3z,g0z,g1z,g2z)

        // update integrals
        intg[0] += d0*f1x;
        intg[1] += d0*f2x; intg[2] += d1*f2y; intg[3] += d2*f2z;
        intg[4] += d0*f3x; intg[5] += d1*f3y; intg[6] += d2*f3z;
        intg[7] += d0*(y0*g0x+y1*g1x+y2*g2x);
        intg[8] += d1*(z0*g0y+z1*g1y+z2*g2y);
        intg[9] += d2*(x0*g0z+x1*g1z+x2*g2z);
      }

      for (unsigned i = 0; i < 10; i++)
      {
        intg[i] *= mult[i];
      } 
      
      mass = intg[0];

      // center of mass
      massCenter.x = intg[1]/mass;
      massCenter.y = intg[2]/mass;
      massCenter.z = intg[3]/mass;
      
      // inertia tensor relative to center of mass
      ffloat ixx = intg[5]+intg[6]-mass*(massCenter.y * massCenter.y + massCenter.z * massCenter.z);
      ffloat iyy = intg[4]+intg[6]-mass*(massCenter.z * massCenter.z + massCenter.x * massCenter.x);
      ffloat izz = intg[4]+intg[5]-mass*(massCenter.x * massCenter.x + massCenter.y * massCenter.y);
      ffloat ixy = -(intg[7]-mass * massCenter.x * massCenter.y);
      ffloat iyz = -(intg[8]-mass * massCenter.y * massCenter.z);
      ffloat ixz = -(intg[9]-mass * massCenter.z * massCenter.x);

      inertiaTensor.data[0] = ixx;
      inertiaTensor.data[1] = ixy;
      inertiaTensor.data[2] = ixz;
      inertiaTensor.data[3] = ixy;
      inertiaTensor.data[4] = iyy;
      inertiaTensor.data[5] = iyz;
      inertiaTensor.data[6] = ixz;
      inertiaTensor.data[7] = iyz;
      inertiaTensor.data[8] = izz;
    }

    void Polyhedron::getInertiaTensor(ffloat mass, Matrix3 &inertiaTensor)
    {
      ffloat unitMass;
      Vector3 massCenter;
      Matrix3 tensor;
      calculateInertiaTensor(pointsLocal, indecies, unitMass, massCenter, tensor);
      tensor *= mass;
      inertiaTensor = tensor;
    }

    void Polyhedron::setPoints(Points &points, Indecies &indecies)
    {
      for(unsigned i = 0; i < points.size(); i++)
      {
        pointsLocal.emplace_back(points[i]);
        pointsWorld.emplace_back(Vector3::zero);
      }

      for(unsigned i = 0; i < indecies.size(); i++)
      {
        Polyhedron::indecies.emplace_back(indecies[i]);
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
