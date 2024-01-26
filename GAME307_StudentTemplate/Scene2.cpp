#include "Scene2.h"

Scene2::Scene2(SDL_Window* sdlWindow, GameManager* game_)
{
	window = sdlWindow;
	game = game_;
	renderer = SDL_GetRenderer(window);
}

Scene2::~Scene2()
{
}

bool Scene2::OnCreate()
{
	printf("hello scene2\n");

	int count = 5;
	sceneNodes.resize(count);

	for (int i = 0; i < count; i++) {
		sceneNodes[i] = new Node(i);
	}
	graph = new Graph();
	if (!graph->OnCreate(sceneNodes)) {
		std::cerr << "couldn't create graph" << std::endl;
		return false;
	}

	//				0
	//				|
	//   1 -------- 2 --------- 3
	//				|
	//				4

	graph->AddWeightedConnection(0, 2, 1.0f);
	graph->AddWeightedConnection(1, 2, 1.0f);
	graph->AddWeightedConnection(2, 1, 1.0f);
	graph->AddWeightedConnection(2, 0, 1.0f);
	graph->AddWeightedConnection(2, 3, 1.0f);
	graph->AddWeightedConnection(2, 4, 1.0f);
	graph->AddWeightedConnection(3, 2, 1.0f);
	graph->AddWeightedConnection(4, 2, 1.0f);


	std::cout << "Scene 2" << std::endl;

	int myNode = 2;

	std::cout << "Neighbours of " << myNode << std::endl;

	for (int nodeLabel : graph->Neighbours(myNode)) {
		std::cout << "node " << nodeLabel << std::endl;
	}





	return true;
}

void Scene2::OnDestroy()
{
	// cleanup mem
}

void Scene2::Update(const float time)
{
}

void Scene2::Render()
{
}

void Scene2::HandleEvents(const SDL_Event& event)
{
}
