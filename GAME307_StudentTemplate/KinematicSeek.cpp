#include "KinematicSeek.h"

KinematicSeek::KinematicSeek(Body* character_, Body* target_)
{
    character = character_;
    target = target_;
    result = new KinematicSteeringOutput();
}

KinematicSteeringOutput* KinematicSeek::GetSteering()
{
    result->velocity = target->getPos() - character->getPos();

    // go full speed
    result->velocity = VMath::normalize(result->velocity) * character->getMaxSpeed();
    /*if (VMath::mag(result->velocity) > 0.0f) {
        result->rotation = atan2(-result->velocity.y, result->velocity.x) + (M_PI / 2);
    }
    else {

        result->rotation = 0.0f;
    }*/
    result->rotation = 0.0f;
    return result;

}


KinematicSeek::~KinematicSeek()
{
    if (result) {
        delete result;
    }
}
