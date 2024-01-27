#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include "Scene.h"
#include "StaticBody.h"
#include "Graph.h"

using namespace std;

class Turret;

enum class Direction {
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
};

#endif

