#include "contact_generator.h"

namespace physics
{
  ContactGenerator::ContactGenerator(const Vector3 &min, const Vector3 &max)
	{
#ifdef _OPTIM_TIME_
		initTimer();
#endif

		// AABB *a = new AABB(min, max);
		// octreeRoot = new OCTreeNode(a, 0, 0, NULL);
	}

  ContactGenerator::~ContactGenerator() 
  {
		// OCTreeNode::destroy(octreeRoot);
		// octreeRoot = NULL;
	}

  void ContactGenerator::fillContactParam(Primitive *cpa, Primitive *cpb, CollisionData *cData)
	{
		ffloat friction = getContactFriction(cpa->tProfile, cpb->tProfile);
		cData->friction = (friction == -ffone) ? cData->defFriction : friction;

		ffloat restitution = getContactRestitution(cpa->tProfile, cpb->tProfile);
		cData->restitution = (restitution == -ffone) ? cData->defRestitution : restitution;
	}
}