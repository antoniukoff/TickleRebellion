#include "Spaceship.h"
#include "SpriteSheet.h"
Spaceship::Spaceship(SDL_Renderer* renderer, const char* path)
	: renderer(renderer)
{
	loadTexture(path, renderer);
	loadBeamTexture("Luz nave.png", renderer);
}

Spaceship::~Spaceship()
{
}

void Spaceship::loadTexture(const char* path, SDL_Renderer* renderer)
{
	SDL_Surface* surface = IMG_Load(path);
	if (surface == nullptr) {
		SDL_Log("Failed to create surface from image %s! SDL Error: %s", path, SDL_GetError());
		return;
	}
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == nullptr) {
		SDL_Log("Failed to create texture from surface! SDL Error: %s", SDL_GetError());
		return;
	}
	SDL_FreeSurface(surface);
}

void Spaceship::loadBeamTexture(const char* path, SDL_Renderer* renderer)
{
	SDL_Surface* surface = IMG_Load(path);
	if (surface == nullptr) {
		SDL_Log("Failed to create surface from image %s! SDL Error: %s", path, SDL_GetError());
		return;
	}
	beamTexture = SDL_CreateTextureFromSurface(renderer, surface);
	if (beamTexture == nullptr) {
		SDL_Log("Failed to create texture from surface! SDL Error: %s", SDL_GetError());
		return;
	}
	SDL_FreeSurface(surface);
}

void Spaceship::render()
{
	int w, h; 
	int w2, h2;
	float scale = 0.5f;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect rect = { position.x,  position.y, w * scale, h * scale };
	if (direction == State::STOP)
	{
		SDL_QueryTexture(texture, NULL, NULL, &w2, &h2);
		SDL_Rect beamRect = { position.x , position.y + 40, w2 * scale, h2 * scale + 50.0f };
		float wave = cos(velocityModifier * 2.0f * M_PI * 0.7f) * 0.2f; // This will oscillate between -1 and 1

		wave = (wave + 1.0f) / 2.0f;
		float alpha = wave * 245 + 10;
		SDL_SetTextureAlphaMod(beamTexture, static_cast<Uint8>(alpha));
		SDL_RenderCopyEx(renderer, beamTexture, NULL, &beamRect, 0, NULL, SDL_FLIP_NONE);
	}
	SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
	
}

void Spaceship::update()
{
	

	if (direction == State::OFFSCREEN) {
		stopTimer++;
		velocity.x = 0.0f;
		targetAngle = 0.0f;	
		if (stopTimer == 300.0f) {
			velocity.x = position.x <= 100.0f ? 5.0f : -5.0f;
			targetAngle = position.x <= 100.0f ? 10.0f : -10.0f;
			toggleSide = !toggleSide;
			direction = State::MOVING;
		}
	}

	if (position.x > 300.0f && position.x <= 500.0f && !toggleSide) {
		targetAngle = -10.0f;
		float factor = (505.0f - position.x) / 150.0f;
		velocity.x = 5.0f * factor;
	}

	if ((int)position.x == 500.0f)
	{
		targetAngle = 0.0f;
		velocity.x = 0.0f;
		if ((int)angle == targetAngle)
		{
			float a = sin((velocityModifier += 0.05f)) * 1.0f;
			position.y += a;
			stopTimer++;
			direction = State::STOP;
			if (stopTimer == departTimer) {
				direction = State::MOVING;
				if (toggleSide) {
					velocity.x = -5.0f;
					targetAngle = -10.0f;
				}
				else {
					velocity.x = 5.0f;
					targetAngle = 10.0f;
				}
				stopTimer = 0.0f;
			}
		}
	}

	if (angle != targetAngle)
	{
		angle = lerp(angle, targetAngle, 0.05f);
	}

	position += velocity;

	if (position.x <= -100.0f || position.x >= 820.0f) 
	{
		direction = State::OFFSCREEN;
	}
}


float Spaceship::lerp(float a, float b, float f)
{
	return a * (1.0 - f) + (b * f);
}	