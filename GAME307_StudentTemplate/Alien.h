#pragma once
#include "StaticBody.h"
#include "KinematicSteeringOutput.h"
class Scene;

static enum class Movement {
	RIGHT,
	LEFT,
	FORWARD,
	BACKWARD,
	IDLE
};
class Alien
{
public:
	Alien(Vec3 pos_, Scene* scene_, std::string filename);
	~Alien();
	void Update(float deltaTime, Body* target);
	void Render();
	void SteerToArrive(KinematicSteeringOutput*& steering, Body* target);
	bool setTextureWith(std::string file);
private:
	float scale;
	StaticBody* body;
	Scene* scene;
	Movement direction = Movement::IDLE;
};

