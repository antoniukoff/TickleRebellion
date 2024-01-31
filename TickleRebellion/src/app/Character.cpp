#include "Character.h"
#include "../utils/SpriteSheet.h"
#include "../core/physics/PlayerBody.h"
#include <random>

static std::mt19937 randomEngine(time(nullptr));
static std::uniform_real_distribution<float> scaleGenerator(0.8f, 1.0f);

Character::~Character()
{
}

bool Character::OnCreate(Scene* scene_, std::string filename, Vec3 pos)
{
	scene = scene_;
	scale = 1.0f;
	maxHP = 8.0f;
	curHP = maxHP;

	if (!body)
	{
		float radius = 0.0f;
		float orientation = 0.0f;
		float rotation = 0.0f;
		float angular = 0.0f;
		float maxSpeed = 5.0f;
		float maxAcceleration = 10.0f;
		float maxRotation = 2.0f;
		float maxAngular = 10.0f;
		body = new PlayerBody(pos, Vec3(), Vec3(), 1.0f, radius, orientation, rotation, angular, maxSpeed, maxAcceleration, maxRotation, maxAngular, scene->game);
	}

	if (!body)
	{
		return false;
	}

	setTextureWith(filename);

	return true;
}

bool Character::setTextureWith(string file)
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


void Character::Update(float deltaTime)
{

	body->Update(deltaTime);

}

void Character::RenderUI()
{
	SDL_Renderer* renderer = scene->game->getRenderer();
	Matrix4 projectionMatrix = scene->getProjectionMatrix();

	SDL_Rect HPoutline;
	SDL_Rect HPfill;
	Vec3 screenCoords;
	float    w = 40, h = 10;

	// notice use of "body" in the following

	w = static_cast<int>(w);
	h = static_cast<int>(h);
	screenCoords = projectionMatrix * body->getPos();

	HPoutline.y = static_cast<int>(screenCoords.y - 2.8f * h * scale);
	HPoutline.x = static_cast<int>(screenCoords.x - 1.5f * w);
	HPoutline.w = w;
	HPoutline.h = h;

	HPfill = HPoutline;
	HPfill.w = curHP / maxHP * w;
	// red for fill
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &HPfill);


	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &HPoutline);
}

void Character::render()
{

	scale = ((scene->getWindowHeight() / 4.0f) - (body->getPos().y * scene->getyAxis())) / (scene->getWindowHeight() / 4.0f);

	SDL_Renderer* renderer = scene->game->getRenderer();
	Matrix4 projectionMatrix = scene->getProjectionMatrix();

	Vec3 screenCoords;
	int    w, h;

	// notice use of "body" in the following
	SDL_QueryTexture(body->getTexture(), nullptr, nullptr, &w, &h);
	w = static_cast<int>(w * scale);
	h = static_cast<int>(h * scale);
	screenCoords = projectionMatrix * body->getPos();
	destRect.y = static_cast<int>(screenCoords.y - 0.5 * h);
	destRect.x = static_cast<int>(screenCoords.x - 0.5 * w);
	destRect.w = w;
	destRect.h = h;

	// Convert character orientation from radians to degrees.

	int numFrames = 0;
	int FRAME_SPEED = 50;

	SpriteSheet::QuerySpriteSheet(8, 3, body->getTexture());

	if (body->getVel().x > 0.2f && abs(body->getVel().x) > abs(body->getVel().y)) {
		direction = Direction::RIGHT;
	}
	else if (body->getVel().x < -0.2f && abs(body->getVel().x) > abs(body->getVel().y)) {
		direction = Direction::LEFT;
	}
	else if (body->getVel().y > 0.2f && abs(body->getVel().y) > abs(body->getVel().x)) {
		direction = Direction::FORWARD;
	}
	else if (body->getVel().y < -0.2f && abs(body->getVel().y) > abs(body->getVel().x)) {
		direction = Direction::BACKWARD;
	}
	else {
		direction = Direction::IDLE;
	}
	int startPosX = 0;
	int tileIndexY = 0;
	int tileIndexX = 0;

	SpriteSheet::QuerySpriteSheet(9, 4, body->getTexture());
	switch (direction) {
	case Direction::RIGHT:
		numFrames = 3;
		startPosX = 0;
		tileIndexY = 2;
		tileIndexX = startPosX + ((SDL_GetTicks() / FRAME_SPEED) % numFrames);
		sourceRect = SpriteSheet::GetUVTile(tileIndexX, tileIndexY);
		break;

	case Direction::LEFT:
		numFrames = 3;
		startPosX = 0;
		tileIndexY = 0;
		tileIndexX = startPosX + ((SDL_GetTicks() / FRAME_SPEED) % numFrames);
		sourceRect = SpriteSheet::GetUVTile(tileIndexX, tileIndexY);
		break;

	case Direction::BACKWARD:
		numFrames = 3;
		startPosX = 0;
		tileIndexY = 1;
		tileIndexX = startPosX + ((SDL_GetTicks() / FRAME_SPEED) % numFrames);
		sourceRect = SpriteSheet::GetUVTile(tileIndexX, tileIndexY);
		break;

	case Direction::FORWARD:
		numFrames = 3;
		startPosX = 0;
		tileIndexY = 3;
		tileIndexX = startPosX + ((SDL_GetTicks() / FRAME_SPEED) % numFrames);
		sourceRect = SpriteSheet::GetUVTile(tileIndexX, tileIndexY);
		break;
	case Direction::IDLE:

		sourceRect = SpriteSheet::GetUVTile(1, 3);
		break;
	}
	SpriteSheet::drawPlayer(renderer, body->getTexture(), sourceRect, destRect, 1.0f, true);
}

void Character::HandleEvents(const SDL_Event& event)
{
	body->HandleEvents(event);
}

