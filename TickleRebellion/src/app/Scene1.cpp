#include "Scene1.h"
#include <algorithm>
#include "../utils/ResourseManager.h"
#include "../core/Node.h"

std::mt19937 Scene1::mt = std::mt19937(std::random_device()());
std::uniform_real_distribution<float> Scene1::distX = std::uniform_real_distribution<float>();
std::uniform_real_distribution<float> Scene1::distY = std::uniform_real_distribution<float>();

Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	xAxis = 25.0f;
	yAxis = 15.0f;
}

Scene1::~Scene1(){}

bool Scene1::OnCreate() {
	
	SDL_GetWindowSize(window,&w,&h);
	
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	
	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);
	
	/// Map and initial character set up 
	level = Level("assets/levels/Level1.txt", this);
	level.loadMap(12, 11, "assets/sprites/tilemap.png");
	level.sortTiles();
	// Set player image to PacMan
	float floatMin= std::numeric_limits<float>::min();
	for (auto& tile : level.getTiles()) {
		if (tile->tileNode != nullptr && tile->tileNode->getPosition().y > floatMin)
		{
			floatMin = tile->tileNode->getPosition().y;
		}
	}

	yCap = floatMin;
	game->setEndBoundary(yCap);

	distX = std::uniform_real_distribution<float>(0.0f, xAxis);
	distY = std::uniform_real_distribution<float>(0.0f, game->getEndBoundary());

	myCharacter = new Character();
	myCharacter->OnCreate(this, "assets/sprites/human_base.png", Vec3{ 10.0f, 0.0f, 0.0f });

	spaceship = new Spaceship(renderer, "assets/sprites/NAVE.png");

	audioEngine.init();
	Music music = audioEngine.loadMusic("assets/sounds/OutThere.ogg");

	music.play(-1);

	gun = new Gun("pistol", 1, 1, 0.0f, 1.0f, 500.0f, audioEngine.loadSoundEffect("assets/sounds/powerup_02.wav"));

	currentWave = 0;

	return true;
}

void Scene1::OnDestroy() {
	audioEngine.destroy();	
}

void Scene1::Update(const float deltaTime) {
	// Calculate and apply any steering for npc's

	myCharacter->Update(deltaTime);

	for (int i = 0; i < aliens[currentWave].size(); i++)
	{
		aliens[currentWave][i]->Update(deltaTime, myCharacter->getBody(), aliens[currentWave], 3.0f, i);
	}

	spaceship->update();

	if (!stopSpawn)
	{
		spawnAlien();
	}
	else {
		hasPickedUpHands();
	}
	
	Vec3 bulletPos = getProjectionMatrix() * myCharacter->getBody()->getPos();
	bulletPos.x -= 47.0f;
	bulletPos.y -= 5.0f;

	if (isShooting) {
		Vec3 direction = MATH::VMath::normalize(mousePos - bulletPos);
		if (!pickedUpHands) {
			gun->update(true, bulletPos + (direction * 70.0f), direction, bullets, 0.0f);
			isShooting = false;
		}
		else
		{
			bigHandTexture = nullptr;
			gun->update(true, bulletPos + (direction * 70.0f), direction, bullets, 0.0f);
			isShooting = false;
		}
	}

	// First loop
	for (int i = 0; i < bullets.size();)
	{
		Vec3 yCaps = getProjectionMatrix() * Vec3 { 0.0f, this->yCap, 0.0f };
		if (!pickedUpHands) {
			if (bullets[i].update(deltaTime, yCaps.y+20.0f)) {
				bullets[i] = bullets.back();
				bullets.pop_back();
			}
			else {
				i++;
			}
		}
		else {
			
			bullets[i].updateSuperBullet(deltaTime, yCaps.y);

			int shipRadius = spaceship->getDims().w / 2;
			int bulletRadius = bullets[i].getRect().w / 2;
			Vec3 spaceshipPos = { (float)spaceship->getDims().x, (float)spaceship->getDims().y, 0.0f};
			Vec3 bulletPos = { (float)bullets[i].getRect().x, (float)bullets[i].getRect().y, 0.0f };
			int distance = VMath::distance(spaceshipPos, bulletPos);
			if (distance < shipRadius + bulletRadius) {
				spaceship->destroyShip();
				SoundEffect explosion = audioEngine.loadSoundEffect("assets/sounds/explosion1.ogg");
				explosion.play();
				bullets[i] = bullets.back();
				bullets.pop_back();
			}
			else if(bullets[i].getRect().y < 0){
				bullets[i] = bullets.back();
				bullets.pop_back();
				currentWave = 0;
				pickedUpHands = false;
				stopSpawn = false;
				reset = true;
				i++;
			}
			else {
				i++;
			}
		}
	}

	// Second loop
	for (int i = 0; i < bullets.size();)
	{
		bool bulletRemoved = false;
		for (int j = 0; j < aliens[currentWave].size() && !bulletRemoved; j++) {
			if (bullets[i].collideWithAgent(aliens[currentWave][j], this))
			{
				SoundEffect tickEffect = audioEngine.loadSoundEffect("assets/sounds/witch_cackle-1.ogg");
				tickEffect.play();
				bullets.erase(bullets.begin() + i);
				if (currentWave == 2  && aliens[currentWave].size() == 1) {
					if (!bigHandTexture) {
						spawnBigHands(aliens[currentWave].back());
						stopSpawn = true;
					}
				}
				aliens[currentWave].erase(aliens[currentWave].begin() + j);
				bulletRemoved = true;
			}
		}
		if (!bulletRemoved) {
			i++;
		}
	}

	if (spaceship->getState() == State::REMOVE)
	{
		delete spaceship;
		spaceship = nullptr;
		game->setLaunch(false);
	}

	
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);


	level.drawTiles();
	if (bigHandTexture)
	{
		if (pickedUpHands)
		{
			SDL_Rect rect;
			bigHandRect.x = myCharacter->getDestRect().x;
			bigHandRect.y = myCharacter->getDestRect().y;
			rect.x = myCharacter->getDestRect().x - bigHandRect.w / 2 + (myCharacter->getDestRect().w / 4);
			rect.y = myCharacter->getDestRect().y - (myCharacter->getDestRect().h /myCharacter->getScale()  / 2);
			rect.w = bigHandRect.w;
			rect.h = bigHandRect.h;
			SDL_RenderCopy(renderer, bigHandTexture, NULL, &rect);
		}
		else {

			SDL_RenderCopy(renderer, bigHandTexture, NULL, &bigHandRect);
		}
	}

	myCharacter->render();
	for (auto& alien : aliens[currentWave]) {
		alien->Render();
	}
	// render the player
	if(spaceship)
	spaceship->render();


	for (auto& bullet : bullets)
	{
		if (!pickedUpHands)
		{
		bullet.draw(renderer, this);
		} 
		else
		{
			bullet.drawSuperBullet(renderer, this);
		}
	}

	

	SDL_RenderPresent(renderer);
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to npc's as needed
	//level.levelHandleEvents(event);
	// send events to player as needed
	myCharacter->HandleEvents(event);	
	

	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		isShooting = true;
		mousePos = { (float)event.button.x, (float)event.button.y, 0.0f };
		break;
	case SDL_MOUSEBUTTONUP:
		isShooting = false;
		break;
	}

}

void Scene1::updateBigHand(Vec3 mousePos) {
	static Vec3 bigHandPos = getProjectionMatrix() * myCharacter->getBody()->getPos();
	bigHandPos.x -= 47.0f;
	bigHandPos.y -= 5.0f;
	Vec3 direction = MATH::VMath::normalize(mousePos - bigHandPos);
	bigHandRect.x = bigHandPos.x + (direction.x * 70.0f);
	bigHandRect.y = bigHandPos.y + (direction.y * 70.0f);
	isShooting = false;
}

void Scene1::spawnAlien()
{
	static int totalCount = 0;
	if (reset) {
		totalCount = 0;
		reset = false;
	}
	if (spaceship->getState() == State::STOP) {
	
		if (frameTime >= timeToAdd) {
			if (totalCount < Waves[currentWave])
			{
				Vec3 position = { distX(mt), distY(mt), 0.0f };
				Alien* alien = new Alien(position, this, "assets/sprites/SHEET.PNG");
				aliens[currentWave].push_back(alien);
				totalCount++;
			}

			if (totalCount == Waves[currentWave] && aliens[currentWave].size() == 0)
			{
				if (currentWave < 2) {
					currentWave++;
				}
				totalCount = 0;
			}
			frameTime = 0.0f;
		}
		else
			frameTime += 1.5f * (currentWave + 1);
	}
}

bool Scene1::hasPickedUpHands() {
	int playerRadius =  myCharacter->getDestRect().w / 2;
	int bigHandsRadius =  bigHandRect.w / 2;
	Vec3 bigHandPos = { (float)bigHandRect.x, (float)bigHandRect.y, 0.0f };
	int distance = VMath::distance((getProjectionMatrix() * myCharacter->getBody()->getPos()), bigHandPos);
	if (distance < playerRadius + bigHandsRadius) {
		pickedUpHands = true;
		return true;
	}
	return false;
}

void Scene1::spawnBigHands(Alien* alien) 
{
	bigHandTexture = ResourceManager::getTexture("assets/sprites/hands.png", renderer);
	int w, h;
	SDL_QueryTexture(bigHandTexture, NULL, NULL, &w, &h);
	bigHandRect = { (int)(getProjectionMatrix() * alien->getBody()->getPos()).x - (w / 4) / 2, (int)(getProjectionMatrix() * alien->getBody()->getPos()).y - (h / 4) / 2, w / 4 , h / 4 };
}
		
