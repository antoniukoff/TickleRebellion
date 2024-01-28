#include "Bullet.h"
#include "Level.h"
#include "Alien.h"
#include "Scene.h"

Bullet::Bullet(MATH::Vec3 position, MATH::Vec3 direction, float damage, float speed)
{
	m_position = position;
	m_direction = direction;
	m_damage = damage;
	m_speed = speed;
	m_texture = nullptr;
}

Bullet::~Bullet()
{
}

bool Bullet::update(float deltaTime, float yDistance)
{
	m_position += m_direction * m_speed * deltaTime;

	return collideWithLevel(yDistance);
}

void Bullet::setTexture(SDL_Renderer* renderer, std::string filename)
{
	SDL_Surface* surface = IMG_Load(filename.c_str());
	if (surface == nullptr) {
		std::cout << "Error: " << SDL_GetError() << std::endl;
	}
	else {
		m_texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (m_texture == nullptr) {
			std::cout << "Error: " << SDL_GetError() << std::endl;
		}
	}
	SDL_FreeSurface(surface);
}

void Bullet::draw(SDL_Renderer* renderer, Scene* scene)
{

	float offset = 0.1f * scene->getWindowHeight();
	float targetPositionForMaxScale = (scene->getWindowHeight() / 2.0f) - offset;
	float distanceFromTarget = std::abs(m_position.y - targetPositionForMaxScale);
	float scale = (distanceFromTarget / targetPositionForMaxScale);


	if (!m_texture) {
		setTexture(renderer, "Bullet.png");
	}

	SDL_Rect rect = { m_position.x + 5.0f, m_position.y + 5.0f, 15.0f * scale, 15.0f * scale };

	SDL_RenderCopyEx(renderer, m_texture, nullptr, &rect, 0.0f, nullptr, SDL_FLIP_NONE);
}

bool Bullet::collideWithAgent(Alien* alien)
{
	const float MIN_DISTANCE = alien->getBody()->getRadius() + 5.0f;

	MATH::Vec3 centerPosA = m_position;
	MATH::Vec3 centerPosB = alien->getBody()->getPos() + MATH::Vec3(alien->getBody()->getRadius(), alien->getBody()->getRadius(), 0);
	MATH::Vec3 distVec = centerPosA - centerPosB;

	float distance = MATH::VMath::mag(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0) {

		return true;
	}

	return false;
}

bool Bullet::collideWithLevel(float yDistance)
{
	if (m_position.y < yDistance) {
		return true;
	}
	return false;
}