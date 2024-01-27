#include "Scene1.h"
#include "KinematicSeek.h"


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

	SDL_Surface* image;
	SDL_Texture* texture;

	myCharacter = new Character();
	myCharacter->OnCreate(this, "human_base.png", Vec3{ 10.0f, 0.0f, 0.0f });
	image = IMG_Load("pacman.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);


	float orientation_ = 0.0f;
	float maxSpeed_ = 5.0f;
	float maxRotation_ = 1.0f;
	Vec3 position_(5.0f, 5.0f, 0.0f);


	if (image == nullptr){
		std::cerr << "Can't open Clyde.png" << endl;
		return false;
	}
	if (texture == nullptr) {
		std::cerr << "Can't create Clyde texture" << endl;
		return false;
	}

	SDL_FreeSurface(image);


	// end of character set ups

	return true;
}

void Scene1::OnDestroy() {
	
}

void Scene1::Update(const float deltaTime) {
	// Calculate and apply any steering for npc's

	myCharacter->Update(deltaTime);

	// Update player
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);



	level.drawTiles();
	myCharacter->render();
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
