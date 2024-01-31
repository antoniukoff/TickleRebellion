#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include "../core/Scene.h"
#include "../core/physics/StaticBody.h"

using namespace std;

class Turret;

static enum class Direction {
	RIGHT,
	LEFT,
	FORWARD,
	BACKWARD,
	IDLE
};
class Character
{
private:
	class PlayerBody* body;
	class Scene* scene;
	SDL_Rect sourceRect;
	SDL_Rect destRect;
	Direction direction = Direction::IDLE;
	float scale = 1.0f;


	class DesicionTreeNode* desicionTree;
	Turret* closestTurret;

	float maxHP;
	float curHP;
public:
	Character() :sourceRect{}
	{
		body = NULL;
		scene = NULL;
	};

	~Character();

	bool OnCreate(Scene* scene_, std::string filename, Vec3 pos);
	void OnDestroy() { /*delete body;*/ scene = nullptr; };
	bool setTextureWith(string file);
	void Update(float deltaTime);
	void render();
	void HandleEvents(const SDL_Event& event);


	void TakeDamage(float dmg) { curHP -= dmg; };
	bool isDead() { return (curHP <= 0) ? true : false; }
	void RenderUI();
	Body* getBody() { return body; }
	SDL_Rect getDestRect() const {
		return destRect;
	}
	float getScale() const {
		return scale;
	}
};

#endif

