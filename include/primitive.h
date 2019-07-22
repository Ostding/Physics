#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "types.h"
#include "matrix4.h"
#include "aabb.h"

namespace physics
{
  class RigidBody;
  class OCTreeNode;

  enum PRIMITIVE_TYPE
  {
    PRIT_PLANE        = 1,
    PRIT_SPHERE       = 2,
    PRIT_QUBE         = 4,
    PRIT_CAPSULE      = 8,
    PRIT_POLYHEDRON   = 16,
  };

  class Primitive
  {
  public:
    int id;
    int tContact;
    int tProfile;

    PRIMITIVE_TYPE tPrimitive;

    AABB aabb;
    RigidBody *body;

    Matrix4 offset;
    Matrix4 transform;
    
    Primitive * parent;
    OCTreeNode *node;

    bool isStatic;
    bool isTrigger;

    int interCount;

  public:
    Primitive(PRIMITIVE_TYPE t);
    virtual ~Primitive();

  public:
    virtual void refreshAABB();
    virtual void render();
    virtual void updateTransform();
    
    Vector3 getTransformPos() const;
    Vector3 getColumnVector(unsigned index) const;

    void setPosition(const Vector3 &pos);
    void setOrientation(const Quaternion &dir);

    Vector3 getPosition();
    Quaternion getOrientation();
  };
}


#endif