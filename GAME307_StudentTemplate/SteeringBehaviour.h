#ifndef STEERING_BEHAVIOUR
#define STEERING_BEHAVIOUR


#include <VMath.h>
#include <iostream>
#include "KinematicBody.h"
#include "SteeringOutput.h"

// abstract class

class SteeringBehaviour
{
// protected so variables are available to child classes
protected:
	SteeringOutput* result;
	Body* npc;
	

public:
	SteeringBehaviour() {
		result = new SteeringOutput();
		npc = nullptr;
	}

	// virtual, so will be called when child destructor is called
	virtual ~SteeringBehaviour() {
		if (result) {
			delete result;
		}

	}

	// pure virtual, so has to be defined in child class
	// signified by virtual and = 0
	virtual SteeringOutput* GetSteering() = 0;




};

#endif STEERING_BEHAVIOUR
