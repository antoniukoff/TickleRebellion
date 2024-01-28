#pragma once
#include <vector>
#include <string>
#include "Vector.h"
#include "SDL.h"
class Alien;
class Scene;
class Bullet
{
public:
	Bullet(MATH::Vec3 position, MATH::Vec3 direction, float damage, float speed);
	~Bullet();

	//when update returns true, delete bullet
	bool update(float deltaTime, float yDistance);
	void setTexture(SDL_Renderer* renderer, std::string filename);
	void draw(SDL_Renderer* renderer, Scene* scene);
	bool collideWithAgent(Alien* agent);
	bool collideWithLevel(float yDistance);
	float getDamage() { return m_damage; }
	MATH::Vec3 getPosition() const { return m_position; }
private:
	
	float m_damage;
	MATH::Vec3 m_position;
	MATH::Vec3 m_direction;
	float m_speed;
	SDL_Texture* m_texture;
};

