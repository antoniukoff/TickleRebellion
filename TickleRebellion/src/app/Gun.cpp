#include "Gun.h"
#include <random>
#include <ctime>
#include "VMath.h"

const float DEG_TO_RAD = 3.14159265359f / 180.0f;
const float RAD_TO_DEG = 180.0f / 3.14159265359f;

Gun::Gun(std::string name, int firerate, int bulletspershot,
	float spread, float bulletDamage, float bulletSpeed, SoundEffect fireEffect) : _sound(fireEffect)
{
	_name = name;
	_fireRate = firerate;
	_bulletPerShot = bulletspershot;
	_spread = spread;
	_bulletDamage = bulletDamage;
	_bulletSpeed = bulletSpeed;
	_frameCounter = 0;
}

Gun::~Gun()
{
}

void Gun::update(bool isMouseDown, const MATH::Vec3& position, const MATH::Vec3& direction, std::vector<Bullet>& bullets, float deltaTime)
{
	_frameCounter += 1.0f;
	if (_frameCounter >= _fireRate && isMouseDown) {
		fire(direction, position, bullets);
		_frameCounter = 0;
	}
}

void Gun::fire(MATH::Vec3 direction, const MATH::Vec3& position, std::vector<Bullet>& bullets)
{
	static std::mt19937 randoEngine(time(nullptr));

	_sound.play();

	std::uniform_real_distribution<float> randRotate(-_spread, _spread);

	for (int i = 0; i < _bulletPerShot; i++) {
		bullets.emplace_back(position, rotate(direction, randRotate(randoEngine) * DEG_TO_RAD), _bulletDamage, _bulletSpeed);
	}
}


MATH::Vec3 Gun::rotate(const MATH::Vec3& vector, float angle)
{
	float x = vector.x * cos(angle) - vector.y * sin(angle);
	float y = vector.x * sin(angle) + vector.y * cos(angle);
	return MATH::Vec3(x, y, 0);
}