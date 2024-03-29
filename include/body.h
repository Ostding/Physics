#ifndef __BODY_H__
#define __BODY_H__

#include "types.h"
#include "vector3.h"
#include "quaternion.h"
#include "matrix3.h"
#include "matrix4.h"
#include "primitive.h"

namespace physics
{
  class RigidBody
  {
  public:
    // Damp of linear velocity
    ffloat linearDamp;
    ffloat powLinerDamp;

    // Damp of angular velocity
    ffloat angularDamp;
    ffloat powAngularDamp;

    // Hold inverse of mass, for divide optimize
    ffloat inverseMass;

    // Inertia tensor's inverse in body space
    Matrix3 iitBody;

    // Inertia tensor's inverse in world space
    Matrix3 iitWorld;

    // Body's direction
    Quaternion orientation;

    // Position in world space
    Vector3 position;

    // Linear movement velocity
    Vector3 velocity;

    // Angular movement velocity
    Vector3 rotation;

    // This matrix used for converting between local space and world space
    Matrix4 transformMatrix;

    // Linear accumulator generated by force
    Vector3 forceAcc;

    // Torque accumulator generated by force
    Vector3 torqueAcc;

    // Constant accululator which used to simulate gravity or other constant force
    Vector3 constantAcc;

    // This composite acceleration was generated during last frame update
    // consist of force generated acceleration and constant acceleration
    Vector3 lastFrameAcceleration;

    // This is recency-weighted average, used to put a body to sleep
    ffloat motion;
    bool isAwake;
    bool canSleep;

    // Primitive contain this rigidbody
    Primitive *primitive;

    static ffloat sleepEpsilon;
    static ffloat motionBias;
    static ffloat maxMotion;

  public:
    RigidBody(Primitive *primitive);

  public:
    void update(ffloat deltaTime);

    void setLinearDamp(ffloat damp);
    void setAngularDamp(ffloat damp);
    void setMass(const ffloat &mass);
    ffloat getMass() const;
    bool hasFiniteMass() const;
    void setInertiaTensor(const Matrix3 &tensor);

    void setPosition(const Vector3 &position);

    void setVelocity(const Vector3 &veloctiy);
    void addVelocity(const Vector3 &deltaVelocity);

    void setRotation(const Vector3 &rotation);
    void addRotation(const Vector3 &deltaRotation);

    void setOrientation(const Quaternion &orientation);

    Vector3 getPosInBodySpace(const Vector3 &pos) const;
    Vector3 getPosInWorldSpace(const Vector3 &pos) const;

    Vector3 getDirectionInBodySpace(const Vector3 &dir) const;
    Vector3 getDirectionInWorldSpace(const Vector3 &dir) const;

    void setAwake(const bool awake = true);
    void enableSleep(const bool canSleep = true);

    void clearAccumulators();
    void updateDerivedData();

    void addForce(const Vector3 &force);
    void addForceAtWorldPos(const Vector3 &force, const Vector3 &pos);
    void addForceAtBodyPos(const Vector3 &force, const Vector3 &pos);

    void addTorque(const Vector3 &torque);
    void setConstAccumulator(const Vector3 &accumulator);
    void fillTransformArray(float matrix[16]) const;
  };
}

#endif