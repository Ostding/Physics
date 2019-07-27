#include <assert.h>
#include "body.h"

namespace physics
{

  ffloat RigidBody::sleepEpsilon = ffloat(30000000LL); //0.3f
  ffloat RigidBody::motionBias = ffpow(ffhalf, ffloat(2500000LL));
  ffloat RigidBody::maxMotion = fften * sleepEpsilon;

  static inline void updateTransformMatrix( Matrix4 &transform,
                                            const Vector3 &position,
                                            const Quaternion &orientation)
  {
    transform.data[0] = ffone - fftwo * orientation.j*orientation.j-fftwo * orientation.k*orientation.k;
    transform.data[1] = fftwo * orientation.i*orientation.j - fftwo * orientation.r*orientation.k;
    transform.data[2] = fftwo * orientation.i*orientation.k + fftwo * orientation.r*orientation.j;
    transform.data[3] = position.x;

    transform.data[4] = fftwo * orientation.i*orientation.j + fftwo * orientation.r*orientation.k;
    transform.data[5] = ffone - fftwo * orientation.i*orientation.i - fftwo * orientation.k*orientation.k;
    transform.data[6] = fftwo * orientation.j*orientation.k - fftwo * orientation.r*orientation.i;
    transform.data[7] = position.y;

    transform.data[8] = fftwo * orientation.i*orientation.k - fftwo * orientation.r*orientation.j;
    transform.data[9] = fftwo * orientation.j*orientation.k + fftwo * orientation.r*orientation.i;
    transform.data[10] = ffone - fftwo * orientation.i*orientation.i - fftwo * orientation.j*orientation.j;
    transform.data[11] = position.z;
  }

  static inline void updateInertiaTensor(Matrix3 &matWorld, const Quaternion &q,
                                           const Matrix3 &matBody, const Matrix4 &rotmat)
  {
    ffloat t4 = rotmat.data[0]*matBody.data[0]+
        rotmat.data[1]*matBody.data[3]+
        rotmat.data[2]*matBody.data[6];
    ffloat t9 = rotmat.data[0]*matBody.data[1]+
        rotmat.data[1]*matBody.data[4]+
        rotmat.data[2]*matBody.data[7];
    ffloat t14 = rotmat.data[0]*matBody.data[2]+
        rotmat.data[1]*matBody.data[5]+
        rotmat.data[2]*matBody.data[8];
    ffloat t28 = rotmat.data[4]*matBody.data[0]+
        rotmat.data[5]*matBody.data[3]+
        rotmat.data[6]*matBody.data[6];
    ffloat t33 = rotmat.data[4]*matBody.data[1]+
        rotmat.data[5]*matBody.data[4]+
        rotmat.data[6]*matBody.data[7];
    ffloat t38 = rotmat.data[4]*matBody.data[2]+
        rotmat.data[5]*matBody.data[5]+
        rotmat.data[6]*matBody.data[8];
    ffloat t52 = rotmat.data[8]*matBody.data[0]+
        rotmat.data[9]*matBody.data[3]+
        rotmat.data[10]*matBody.data[6];
    ffloat t57 = rotmat.data[8]*matBody.data[1]+
        rotmat.data[9]*matBody.data[4]+
        rotmat.data[10]*matBody.data[7];
    ffloat t62 = rotmat.data[8]*matBody.data[2]+
        rotmat.data[9]*matBody.data[5]+
        rotmat.data[10]*matBody.data[8];

    matWorld.data[0] = t4*rotmat.data[0]+
        t9*rotmat.data[1]+
        t14*rotmat.data[2];
    matWorld.data[1] = t4*rotmat.data[4]+
        t9*rotmat.data[5]+
        t14*rotmat.data[6];
    matWorld.data[2] = t4*rotmat.data[8]+
        t9*rotmat.data[9]+
        t14*rotmat.data[10];
    matWorld.data[3] = t28*rotmat.data[0]+
        t33*rotmat.data[1]+
        t38*rotmat.data[2];
    matWorld.data[4] = t28*rotmat.data[4]+
        t33*rotmat.data[5]+
        t38*rotmat.data[6];
    matWorld.data[5] = t28*rotmat.data[8]+
        t33*rotmat.data[9]+
        t38*rotmat.data[10];
    matWorld.data[6] = t52*rotmat.data[0]+
        t57*rotmat.data[1]+
        t62*rotmat.data[2];
    matWorld.data[7] = t52*rotmat.data[4]+
        t57*rotmat.data[5]+
        t62*rotmat.data[6];
    matWorld.data[8] = t52*rotmat.data[8]+
        t57*rotmat.data[9]+
        t62*rotmat.data[10];
  }

  RigidBody::RigidBody(Primitive *primitive)
    : linearDamp( ffzero )
    , powLinerDamp( ffzero )
    , angularDamp( ffzero )
    , powAngularDamp( ffzero )
    , inverseMass( ffzero )
    , motion( ffzero )
		, isAwake(true)
    , canSleep( true )
    , primitive( primitive )
    {
      constantAcc = Vector3(ffzero, ffloat(-9.8), ffzero);
    }

	 	void RigidBody::update(ffloat deltaTime)
		{
			if (!isAwake) return;
		  if (primitive->isStatic) return;

      lastFrameAcceleration = constantAcc;
      // a = f * (1/m)
      lastFrameAcceleration.addScaledVector(forceAcc, inverseMass);
      //About how to translate torque to acceleration refer to https://zh.wikipedia.org/wiki/%E5%8A%9B%E7%9F%A9
      // a = t / i 
      Vector3 angularAcceleration = iitWorld.transform(torqueAcc);

      velocity.addScaledVector(lastFrameAcceleration, deltaTime);
      rotation.addScaledVector(angularAcceleration, deltaTime);

      velocity *= powLinerDamp;
      rotation *= powAngularDamp;

      position.addScaledVector(velocity, deltaTime);
      orientation.addScaledVector(rotation, deltaTime);

      velocity *= powLinerDamp;
      rotation *= powAngularDamp;

      updateDerivedData();
      clearAccumulators();

      if(canSleep)
      {
        ffloat curMotion = velocity.dot(velocity) + rotation.dot(rotation);
        motion = motionBias * motion + (ffone - motionBias) * curMotion;

        if(motion <= sleepEpsilon)
          setAwake(false);
        else if(motion > maxMotion)
          motion = maxMotion;
      }
		}

    void RigidBody::setLinearDamp(ffloat damp)
    {
      linearDamp = damp;
      powLinerDamp = ffpow(linearDamp, ffloat(2500LL));
    }

    void RigidBody::setAngularDamp(ffloat damp)
    {
      angularDamp = damp;
      powAngularDamp = ffpow(angularDamp, ffloat(2500000LL));
    }

    void RigidBody::setMass(const ffloat &mass)
		{
			assert(mass != ffzero);
  		inverseMass = ffone / mass;
		}

		ffloat RigidBody::getMass() const
		{
			assert(inverseMass != ffzero);
			return ffone / inverseMass;			
		}

    bool RigidBody::hasFiniteMass() const
		{
			return inverseMass > ffzero;
		}

    void RigidBody::setInertiaTensor(const Matrix3 &tensor)
		{
			iitBody.setInverse(tensor);
		}

    void RigidBody::setPosition(const Vector3 &position)
		{
			RigidBody::position = position;
		}

    void RigidBody::setVelocity(const Vector3 &veloctiy)
		{
			RigidBody::velocity = velocity;
		}

    void RigidBody::addVelocity(const Vector3 &deltaVelocity)
		{
			velocity += deltaVelocity;
		}

    void RigidBody::setRotation(const Vector3 &rotation)
		{
			RigidBody::rotation = rotation;
		}

    void RigidBody::addRotation(const Vector3 & deltaRotation)
		{
			rotation += deltaRotation;
		}

    void RigidBody::setOrientation(const Quaternion &orientation)
		{
			RigidBody::orientation = orientation;
    	RigidBody::orientation.normalise();
		}
    
    Vector3 RigidBody::getPosInBodySpace(const Vector3 &pos) const
		{
			return transformMatrix.inverseTransform(pos);
		}
    Vector3 RigidBody::getPosInWorldSpace(const Vector3 &pos) const
		{
			return transformMatrix.transform(pos);
		}

    Vector3 RigidBody::getDirectionInBodySpace(const Vector3 &dir) const
		{
			return transformMatrix.inverseTransformDirection(dir);
		}
    Vector3 RigidBody::getDirectionInWorldSpace(const Vector3 &dir) const
		{
			return transformMatrix.transformDirection(dir);
		}

    void RigidBody::setAwake(const bool awake)
		{
			if (awake) {
        isAwake= true;
				motion = sleepEpsilon * fftwo;// Add a bit of motion to avoid it falling asleep immediately.
			} 
			else 
			{
        isAwake = false;
        velocity.clear();
        rotation.clear();
    	}
		}

    void RigidBody::enableSleep(const bool canSleep)
		{
			RigidBody::canSleep = canSleep;

    	if (!canSleep && !isAwake) 
			{
				setAwake();
			}
		}

    void RigidBody::clearAccumulators()
		{
			forceAcc.clear();
			torqueAcc.clear();
		}

   void RigidBody::updateDerivedData()
   {
      orientation.normalise();
			updateTransformMatrix(transformMatrix, position, orientation);
			updateInertiaTensor(iitWorld, orientation, iitBody, transformMatrix);
   }

    void RigidBody::addForce(const Vector3 &force)
		{
			forceAcc += force;
			isAwake = true;
		}

    void RigidBody::addForceAtWorldPos(const Vector3 &force, const Vector3 &pos)
		{
			Vector3 pt = pos;
			pt -= position;

      forceAcc += force;
      //扭矩，是力跟力臂的叉积
      //t = r x f
			torqueAcc += pt.cross(force);
			isAwake = true;
		}

    void RigidBody::addForceAtBodyPos(const Vector3 &force, const Vector3 &pos)
		{
			Vector3 pt = getPosInWorldSpace(pos);
			addForceAtWorldPos(force, pt);
		}

    void RigidBody::addTorque(const Vector3 &torque)
		{
			torqueAcc += torque;
    	isAwake = true;
		}

    void RigidBody::setConstAccumulator(const Vector3 &accumulator)
		{
			constantAcc = accumulator;
		}

    void RigidBody::fillTransformArray(float matrix[16]) const
    {
      matrix[0] = transformMatrix.data[0].to_d();
      matrix[1] = transformMatrix.data[4].to_d();
      matrix[2] = transformMatrix.data[8].to_d();
      matrix[3] = 0;

      matrix[4] = transformMatrix.data[1].to_d();
      matrix[5] = transformMatrix.data[5].to_d();
      matrix[6] = transformMatrix.data[9].to_d();
      matrix[7] = 0;

      matrix[8] = transformMatrix.data[2].to_d();
      matrix[9] = transformMatrix.data[6].to_d();
      matrix[10] = transformMatrix.data[10].to_d();
      matrix[11] = 0;

      matrix[12] = transformMatrix.data[3].to_d();
      matrix[13] = transformMatrix.data[7].to_d();
      matrix[14] = transformMatrix.data[11].to_d();
      matrix[15] = 1;
    }
}