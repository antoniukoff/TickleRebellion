#include "StaticBody.h"

void StaticBody::Update(float deltaTime, KinematicSteeringOutput* steering) {

	Body::Update(deltaTime);
	NewOrientation();
	// set values based on steering output

	if (steering == nullptr) {
		vel = Vec3();
		rotation = 0.0f;
		return;
	}

	vel = steering->velocity;
	rotation = steering->rotation;

	if (VMath::mag(vel) > maxSpeed) { vel = VMath::normalize(vel) * maxSpeed; }


	// two outcome switch statement
	// query if rotation > maxRotation
	// if true, return maxRotation
	// if false, return rotation

	rotation = rotation > maxRotation ? maxRotation : rotation;
}

void StaticBody::NewOrientation() {

	if (VMath::mag(vel) > 0.0f) {

		orientation = atan2(-vel.y, vel.x) + M_PI/2;
			
	}
}