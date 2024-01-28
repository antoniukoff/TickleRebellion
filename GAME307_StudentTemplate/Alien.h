#pragma once
#include "StaticBody.h"
#include "KinematicSteeringOutput.h"
#include <vector>
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
	void Update(float deltaTime, Body* target, std::vector<Alien*> aliens, float threshhold, int index);
	void Render();
	StaticBody* getBody() { return body; }
	float getRadius() { return destRect.w / 2.0f; }
private:
	void SteerToArrive(KinematicSteeringOutput*& steering, Body* target, std::vector<Alien*> aliens, float threshhold, int index);
	bool setTextureWith(std::string file);
	float scale;
	StaticBody* body;
	Scene* scene;
	Movement direction = Movement::IDLE;
	SDL_Rect destRect;
};

