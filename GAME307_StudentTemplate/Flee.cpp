#include "Flee.h"

Flee::Flee(Body* npc_, Body* target_) {
	npc = npc_;
	target = target_;
}

Flee::~Flee()
{

}

SteeringOutput* Flee::GetSteering()
{
	// get direction to target

	result->linear = npc->getPos() - target->getPos();

	//give full accel
	result->linear = VMath::normalize(result->linear) * npc->getMaxAcceleration();
	result->angular = 0;

	return result;
}
