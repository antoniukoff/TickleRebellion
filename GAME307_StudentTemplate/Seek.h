#ifndef SEEK_H
#define SEEK_H

#include "SteeringBehaviour.h"
class Seek :
    public SteeringBehaviour
{

protected:
    Body* target;

public:
    Seek(Body* NPC_, Body* target_);
    virtual ~Seek();
    SteeringOutput* GetSteering() override;

};

#endif SEEK_H

