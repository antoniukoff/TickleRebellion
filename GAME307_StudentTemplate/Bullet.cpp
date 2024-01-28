#include "Bullet.h"
#include "Level.h"
#include "Alien.h"
#include "Scene.h"
#include "SpriteSheet.h"
#include "ResourseManager.h"

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

void Bullet::draw(SDL_Renderer* renderer, Scene* scene)
{

	float offset = 0.1f * scene->getWindowHeight();
	float targetPositionForMaxScale = (scene->getWindowHeight() / 2.0f) - offset;
	float distanceFromTarget = std::abs(m_position.y - targetPositionForMaxScale);
	float scale = (distanceFromTarget / targetPositionForMaxScale);


	if (!m_texture) {
		m_texture = ResourceManager::getTexture("hands.png", renderer);
	}

	SDL_Rect rect = { m_position.x - 30.0f, m_position.y - 50.0f , 100.0f * scale, 100.0f * scale };
	m_rect = rect;
	SDL_RenderCopy(renderer, m_texture, nullptr, &rect);
}

bool Bullet::collideWithAgent(Alien* alien, Scene* scene)
{
	const float MIN_DISTANCE = alien->getRadius() + m_rect.w / 2.0f;

	MATH::Vec3 centerPosA = m_position;
	MATH::Vec3 centerPosB = scene->getProjectionMatrix() * alien->getBody()->getPos();	
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