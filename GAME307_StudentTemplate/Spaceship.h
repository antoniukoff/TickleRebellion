#pragma once
#include <SDL.h>
#include "Vector.h"
#include <SDL_image.h>


enum class State
{
	MOVING,
	STOP,
	OFFSCREEN
};
class Spaceship
{
public:
	Spaceship(SDL_Renderer* renderer, const char* path);
	~Spaceship();
	void loadTexture(const char* path, SDL_Renderer* renderer);
	void loadBeamTexture(const char* path, SDL_Renderer* renderer);
	void render();
	void update();
	float lerp(float a, float b, float f);
	State getState() { return direction; }
private:
	MATH::Vec3 position = {- 100.0f, 0.0f, 0.0f};
	MATH::Vec3 velocity = {5.0f, 0.0f,0.0f};
	float acceleration = 1.0f;
	float angle = 10.0f;
	float targetAngle = 10.0f;
	float velocityModifier = 0;
	SDL_Texture* texture;
	SDL_Texture* beamTexture;

	bool toggleSide = false;

	State direction = State::MOVING;
	SDL_Renderer* renderer;
	int stopTimer = 0;
	int departTimer = 1000;

};

