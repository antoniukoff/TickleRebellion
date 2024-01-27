#include "Alien.h"
#include "Scene.h"

Alien::Alien(Vec3 pos_, Scene* scene_, std::string filename)
	: destRect({ 0,0,0,0 }), srcRect({ 0,0,0,0 }), body(nullptr), texture(nullptr), scene(scene_)
{
	scale = 1.0f;

	if (!body)
	{
		float radius = 2.5;
		float orientation = 0.0f;
		float rotation = 0.0f;
		float angular = 0.0f;
		float maxSpeed = 5.0f;
		float maxAcceleration = 10.0f;
		float maxRotation = 2.0f;
		float maxAngular = 10.0f;
		body = new StaticBody(pos_, orientation, maxSpeed, maxRotation);
	}

	if (!body)
	{
		throw std::runtime_error("Failed to create body");
	}

	setTextureWith(filename);
}

Alien::~Alien()
{
}


void Alien::Update(float deltaTime)
{
}

void Alien::Render(SDL_Renderer* renderer)
{
}

void Alien::SteerToArrive(Vec3 target)
{
}

bool Alien::setTextureWith(std::string file)
{
	SDL_Surface* image = IMG_Load(file.c_str());
	if (image == nullptr) {
		throw std::runtime_error("Incorrect filepath");
		return false;
	}
	SDL_Window* window = scene->getWindow();
	SDL_Renderer* renderer = SDL_GetRenderer(window);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
	if (!texture)
	{
		throw std::runtime_error("Failed to create texture");
		return false;
	}
	body->setTexture(texture);
	return true;
}