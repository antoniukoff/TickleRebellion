#ifndef SCENE1_H
#define SCENE1_H

#include <array>
#include <MMath.h>
#include <VMath.h>
#include "Gun.h"
#include "Level.h"
#include "Alien.h"
#include "Character.h"
#include "Spaceship.h"
#include "../core/Scene.h"
#include "../core/AudioManager.h"

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
	std::array<std::vector<Alien*>, 3> aliens;	
	Spaceship* spaceship;
	Level level;
	Gun* gun;
	std::vector<Bullet> bullets;
	bool isShooting = false;
	bool stopSpawn = false;
	bool pickedUpHands = false;
	bool reset = false;
	AudioEngine audioEngine;
	Vec3 mousePos{};

	SDL_Texture* bigHandTexture;
	SDL_Rect bigHandRect;	


	const int Waves[3] = { 30, 50, 70 };
	int currentWave = 0;

	static std::mt19937 mt;
	static std::uniform_real_distribution<float> distX;
	static std::uniform_real_distribution<float> distY;
	float timeToAdd = 50.0f, frameTime = 0.0f;

public:
	Scene1(SDL_Window* sdlWindow, GameManager* game_);
	~Scene1();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
    void HandleEvents(const SDL_Event &event);
	void updateBigHand(Vec3 mousePos);
	float getxAxis() { return xAxis; }
	float getyAxis() { return yAxis; }
	SDL_Window* getWindow() { return window; }
    Matrix4 getProjectionMatrix() { return projectionMatrix; }
	Matrix4 getInverseMatrix() { return inverseProjection; }
	float getWindowWidth() { return w; }
	float getWindowHeight() { return h; }
	void spawnAlien();
	bool hasPickedUpHands();
	void spawnBigHands(Alien* alien);
};

#endif
