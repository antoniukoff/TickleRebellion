#ifndef SCENE1_H
#define SCENE1_H

#include <MMath.h>
#include <VMath.h>
#include "Scene.h"
#include "Character.h"
#include "Alien.h"
#include "StaticBody.h"
#include "Level.h"

using namespace MATH;
class Scene1 : public Scene {
private:
	SDL_Window *window;
	int w, h;
	float yCap = 0.0f;
	float xAxis = 25.0f;
	float yAxis = 15.0f;
	SDL_Renderer* renderer;
	Matrix4 projectionMatrix;
    Matrix4     inverseProjection;
	Character* myCharacter;
	std::vector<Alien*> aliens;	
	Level level;

	static std::mt19937 mt;
	static std::uniform_real_distribution<float> distX;
	static std::uniform_real_distribution<float> distY;

public:
	Scene1(SDL_Window* sdlWindow, GameManager* game_);
	~Scene1();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
    void HandleEvents(const SDL_Event &event);
	float getxAxis() { return xAxis; }
	float getyAxis() { return yAxis; }
	SDL_Window* getWindow() { return window; }
    Matrix4 getProjectionMatrix() { return projectionMatrix; }
	Matrix4 getInverseMatrix() { return inverseProjection; }
	float getWindowWidth() { return w; }
	float getWindowHeight() { return h; }
	
};

#endif
