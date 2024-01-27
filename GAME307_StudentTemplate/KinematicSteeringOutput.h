#ifndef KINEMATIC_STEERING_OUTPUT_H
#define KINEMATIC_STEERING_OUTPUT_H

#include <Vector.h>
using namespace MATH;

class KinematicSteeringOutput
{
public:
	// Yes these are public! 
	// The steering algorithms will be creating instances and then setting the values.
	// There might be a better way, but I can't think of it right now.

	Vec3 velocity;
	float rotation;

	KinematicSteeringOutput();
	~KinematicSteeringOutput();

	KinematicSteeringOutput(Vec3 vel_, float rot_);

	inline const KinematicSteeringOutput operator + (const KinematicSteeringOutput& v) const {
		return KinematicSteeringOutput(velocity + v.velocity, rotation + v.rotation);
	}
	inline KinematicSteeringOutput& operator += (const KinematicSteeringOutput& v) {
		velocity += v.velocity;
		rotation += v.rotation;
		return *this;
	}

	// This class does not have any behaviours, just constructors!

};
#endif

