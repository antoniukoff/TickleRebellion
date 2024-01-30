#include "KinematicSteeringOutput.h"

KinematicSteeringOutput::KinematicSteeringOutput() {

}

KinematicSteeringOutput::KinematicSteeringOutput(Vec3 vel_, float rot_) {
	velocity = vel_;
	rotation = rot_;

}


KinematicSteeringOutput::~KinematicSteeringOutput() {

}
