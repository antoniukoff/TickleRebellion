#include "Scene1.h"
#include "KinematicSeek.h"


std::mt19937 Scene1::mt = std::mt19937(std::random_device()());
std::uniform_real_distribution<float> Scene1::distX = std::uniform_real_distribution<float>();
std::uniform_real_distribution<float> Scene1::distY = std::uniform_real_distribution<float>();

Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
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
	level = Level("Level1.txt", this);
	level.loadMap(12, 11, "tilemap.png");
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
	myCharacter->OnCreate(this, "human_base.png", Vec3{ 10.0f, 0.0f, 0.0f });
	

	Vec3 position = { distX(mt), distY(mt), 0.0f };
	Alien* alien = new Alien(position, this, "SHEET.PNG");

	aliens.push_back(alien);

	return true;
}

void Scene1::OnDestroy() {
	
}

void Scene1::Update(const float deltaTime) {
	// Calculate and apply any steering for npc's

	myCharacter->Update(deltaTime);

	for (int i = 0; i < aliens.size(); i++) {
		aliens[i]->Update(deltaTime, myCharacter->getBody(), aliens, 3.0f, i);
	}


	spawnAlien();
	// Update player
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);


	level.drawTiles();
	myCharacter->render();
	for (auto& alien : aliens) {
		alien->Render();
	}
	// render the player

	SDL_RenderPresent(renderer);
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to npc's as needed
	level.levelHandleEvents(event);
	// send events to player as needed
	myCharacter->HandleEvents(event);
}

void Scene1::spawnAlien()
{
	if (frameTime >= timeToAdd) {
		Vec3 position = { distX(mt), distY(mt), 0.0f };
		Alien* alien = new Alien(position, this, "SHEET.PNG");

		aliens.push_back(alien);
		frameTime = 0.0f;
	} else
		frameTime += distX(mt) * 0.1f;
}
