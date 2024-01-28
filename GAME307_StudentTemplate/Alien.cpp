#include "Alien.h"
#include "Scene.h"
#include "KinematicArrive.h"
#include "KinematicSeperation.h"
#include "SpriteSheet.h"
#include <vector>
#include <random>

Alien::Alien(Vec3 pos_, Scene* scene_, std::string filename)
	: body(nullptr), scene(scene_)
{
	scale = 1.0f;

	std::mt19937 rng = std::mt19937(std::random_device()());
	std::uniform_real_distribution<float> dist(1.0f, 7.0f);

	if (!body)
	{
		float radius = 2.5f;
		float orientation = 0.0f;
		float rotation = 0.0f;
		float angular = 0.0f;
		float maxSpeed = dist(rng);
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


void Alien::Update(float deltaTime, Body* target, std::vector<Alien*> aliens, float threshhold, int index)
{
	// create a new overall steering output
	std::vector<KinematicSteeringOutput*> steering_outputs;
	KinematicSteeringOutput* steering = nullptr;
	SteerToArrive(steering, target, aliens, threshhold, index);
	body->Update(deltaTime, steering);

	delete steering;
}

void Alien::Render()
{
	scale = ((scene->getWindowHeight() / 4.0f) - (body->getPos().y * scene->getyAxis())) / (scene->getWindowHeight() / 4.0f) * 0.1f;

	SDL_Rect sourceRect;

	SDL_Renderer* renderer = scene->game->getRenderer();
	Matrix4 projectionMatrix = scene->getProjectionMatrix();

	SDL_Rect square;
	Vec3 screenCoords;
	int    w, h;

	// notice use of "body" in the following
	SDL_QueryTexture(body->getTexture(), nullptr, nullptr, &w, &h);
	w = static_cast<int>(w * scale);
	h = static_cast<int>(h * scale);
	screenCoords = projectionMatrix * body->getPos();
	square.y = static_cast<int>(screenCoords.y - 0.5 * h);
	square.x = static_cast<int>(screenCoords.x - 0.5 * w);
	square.w = w;
	square.h = h;

	// Convert character orientation from radians to degrees.

	int numFrames = 0;
	int FRAME_SPEED = 100;

	SpriteSheet::QuerySpriteSheet(8, 3, body->getTexture());

	if (body->getVel().x > 0.2f && abs(body->getVel().x) > abs(body->getVel().y)) {
		direction = Movement::RIGHT;
	}
	else if (body->getVel().x < -0.2f && abs(body->getVel().x) > abs(body->getVel().y)) {
		direction = Movement::LEFT;
	}
	else if (body->getVel().y > 0.2f && abs(body->getVel().y) > abs(body->getVel().x)) {
		direction = Movement::FORWARD;
	}
	else if (body->getVel().y < -0.2f && abs(body->getVel().y) > abs(body->getVel().x)) {
		direction = Movement::BACKWARD;
	}
	else {
		direction = Movement::IDLE;
	}
	int startPosX = 0;
	int tileIndexY = 0;
	int tileIndexX = 0;

	SpriteSheet::QuerySpriteSheet(6, 6, body->getTexture());
	switch (direction) {
	case Movement::RIGHT:
		numFrames = 2;
		startPosX = 0;
		tileIndexY = 4;
		tileIndexX = startPosX + ((SDL_GetTicks() / FRAME_SPEED) % numFrames);
		sourceRect = SpriteSheet::GetUVTile(tileIndexX, tileIndexY);
		break;

	case Movement::LEFT:
		numFrames = 2;
		startPosX = 0;
		tileIndexY = 4;
		tileIndexX = startPosX + ((SDL_GetTicks() / FRAME_SPEED) % numFrames);
		sourceRect = SpriteSheet::GetUVTile(tileIndexX, tileIndexY);
		break;

	case Movement::BACKWARD:
		numFrames = 2;
		startPosX = 0;
		tileIndexY = 4;
		tileIndexX = startPosX + ((SDL_GetTicks() / FRAME_SPEED) % numFrames);
		sourceRect = SpriteSheet::GetUVTile(tileIndexX, tileIndexY);
		break;

	case Movement::FORWARD:
		numFrames = 2;
		startPosX = 0;
		tileIndexY = 4;
		tileIndexX = startPosX + ((SDL_GetTicks() / FRAME_SPEED) % numFrames);
		sourceRect = SpriteSheet::GetUVTile(tileIndexX, tileIndexY);
		break;
	case Movement::IDLE:

		sourceRect = SpriteSheet::GetUVTile(1, 3);
		break;
	}
	SpriteSheet::drawPlayer(renderer, body->getTexture(), sourceRect, square, 1.0f, true);
}

void Alien::SteerToArrive(KinematicSteeringOutput*& steering, Body* target, std::vector<Alien*> aliens, float threshhold, int index)
{
	
	steering = new KinematicSteeringOutput();

	std::vector<StaticBody* > staticBodies;
	staticBodies.resize(aliens.size());
	for (uint32_t i = 0; i < aliens.size(); i++) {
		staticBodies[i] = aliens[i]->body;
	}

	std::vector<KinematicSteeringOutput*> steering_outputs;

	KinematicArrive* steering_algorithm = new KinematicArrive(body, target);
	steering_outputs.push_back(steering_algorithm->getSteering());

	KinematicSeperation* separation = new KinematicSeperation(staticBodies, 1.5f, index);
	steering_outputs.push_back(separation->GetSteering());

	for (int i = 0; i < steering_outputs.size(); i++) {
		if (steering_outputs[i]) {
			*steering += *steering_outputs[i];
		}
		delete steering_outputs[i];
	}
	if (steering_algorithm) {
		delete steering_algorithm;
	}
	if (separation) {
		delete separation;
	}
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