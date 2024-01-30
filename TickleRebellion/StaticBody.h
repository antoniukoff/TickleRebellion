#ifndef STATIC_BODY_H
#define STATIC_BODY_H

#include "Body.h"
#include "steering/SteeringOutput.h"

class StaticBody :
	public Body
{
protected:

public:
	StaticBody() : Body{}
	{}

	StaticBody(
		Vec3 pos_, 
		float orientation_,
		float maxSpeed_,
		float maxRotation_

	) : Body{ pos_, Vec3(), Vec3(), 1.0f
		, 1.0f
		, orientation_
		, 0.0f
		, 0.0f
		, maxSpeed_
		, 0.0f
		, maxRotation_
		, 0.0f
	}
	{}

	void Update(float deltaTime, KinematicSteeringOutput* steering);
	// change to KinematicSteeringOutput
	void NewOrientation();
};

#endif
