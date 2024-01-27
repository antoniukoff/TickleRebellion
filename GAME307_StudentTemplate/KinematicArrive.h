#pragma once
#include <Vector.h>
#include <VMath.h>
#include "Body.h"
#include "KinematicSteeringOutput.h"

using namespace MATH;

class KinematicArrive
{
protected:
	KinematicSteeringOutput* result;
	Body* character;
	Body* target;
	float timeToTarget;
public:
	KinematicArrive(Body* character_, Body* target_);
	virtual  ~KinematicArrive();
	KinematicSteeringOutput* getSteering();
};

