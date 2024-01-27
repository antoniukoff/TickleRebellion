#include "KinematicArrive.h"

KinematicArrive::KinematicArrive(Body* character_, Body* target_) {
	character = character_;
	target = target_;
	timeToTarget = 0.25;
}

KinematicArrive::~KinematicArrive()
{
}

KinematicSteeringOutput* KinematicArrive::getSteering()
{
	result = new KinematicSteeringOutput();
	result->velocity = target->getPos() - character->getPos();
	float velocityLength = VMath::mag(result->velocity) - target->getRadius();

	if (velocityLength < target->getRadius()) {
		return nullptr;
	}
	else {
		result->velocity /= timeToTarget;
	}

	if (velocityLength > character->getMaxSpeed()) {
		result->velocity = VMath::normalize(result->velocity);
		result->velocity *= character->getMaxSpeed();
	}
	result->rotation = 0.0f;
	return result;
}
