#pragma once
#include "StaticBody.h"

class Scene;

class Alien
{
	Alien(Vec3 pos_, Scene* scene_, std::string filename);
	~Alien();
	void Update(float deltaTime);
	void Render(SDL_Renderer* renderer);
	void SteerToArrive(Vec3 target);
	bool setTextureWith(std::string file);
private:
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	float scale;
	StaticBody* body;
	Scene* scene;
};

