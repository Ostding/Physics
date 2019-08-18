#include "spring_force.h"

namespace physics
{
  SpringForce::SpringForce(const Vector3 &connectPoint, RigidBody *ohter, const Vector3 &otherConnectPoint,
                  const ffloat &ceofficient, const ffloat &length, const ffloat &maxForce)
  :connectPoint(connectPoint)
  ,other(other)
  ,otherConnectPoint(otherConnectPoint)
  ,ceofficient(ceofficient)
  ,maxForce(maxForce)
  ,length(length)
  {
  }
  
  void SpringForce::updateForce(RigidBody *body, const ffloat &duration)
  {
    Vector3 lws = body->getPosInWorldSpace(connectPoint);
	  Vector3 ows = other->getPosInWorldSpace(otherConnectPoint);

    Vector3 dir = ows - lws;
    
    ffloat mag = dir.mag();
	  ffloat f = mag * ceofficient;
	  if (maxForce != ffzero && f > maxForce)
		{
      f = maxForce;
    }
		
    //Below is equal with [ force = dir.normalise() * f ]
    Vector3 force = dir * (f / mag);

    body->addForceAtBodyPos(force, lws);
  }

}