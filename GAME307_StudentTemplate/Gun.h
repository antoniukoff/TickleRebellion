#pragma once
#include <string>
#include <vector>
#include "Bullet.h"
#include "Vector.h"

class Gun
{
public:
	Gun(std::string name,
		int firerate,
		int bulletspershot,
		float spread,
		float _bulletDamage,
		float bulleSpeed);
	~Gun();
	MATH::Vec3 rotate(const MATH::Vec3& vector, float angle);
	void update(bool isMouseDown,
		const MATH::Vec3& position,
		const MATH::Vec3& direction,
		std::vector<Bullet>& bullets,
		float deltaTime);
private:
	void fire(const MATH::Vec3 direction, const MATH::Vec3& position, std::vector<Bullet>& bullets);

	std::string _name;
	/// <summary>
	/// Gun Attributes
	/// </summary>
	int _fireRate; // in terms of frames
	int _bulletPerShot; //how many bullets are fired at a time
	float _spread; // accuracy
	float _bulletSpeed;
	float _bulletDamage;
	float _frameCounter;
};

