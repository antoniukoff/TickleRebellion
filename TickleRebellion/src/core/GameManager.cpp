#include "GameManager.h"
#include "../app/Scene1.h"
//#include "MemoryMonitor.h"

GameManager::GameManager() {
	windowPtr = nullptr;
	timer = nullptr;
	isRunning = true;
	currentScene = nullptr;
    player = nullptr;
    changeSceneEventType = 0;
}

bool GameManager::OnCreate() {
    // My display is 1920 x 1080 but the following seems to work best to fill the screen.
    //const int SCREEN_WIDTH = 1540;
    //const int SCREEN_HEIGHT = 860;

    // Use 1000x600 for less than full screen
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    windowPtr = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (windowPtr == nullptr) {
		OnDestroy();
		return false;
	}
	if (windowPtr->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		OnDestroy();
		return false;
	}

    // select scene for specific assignment

    currentScene = new Scene1(windowPtr->GetSDL_Window(), this);

    // need to create Player before validating scene
    if (!ValidateCurrentScene()) {
        OnDestroy();
        return false;
    }
    
    // create some user defined event
    changeSceneEventType = SDL_RegisterEvents(1);
    if ( changeSceneEventType == ((Uint32) - 1 ) )
    {
        OnDestroy();
        return false;
    }
       
	return true;
}


/// Here's the whole game loop
void GameManager::Run() {
    
	timer->Start();
    // control if current scene's update() is called each tick
    
	while (isRunning)
	{
            
		handleEvents();
        
		timer->UpdateFrameTicks();
		if ( launched )
		{
		    currentScene->Update(timer->GetDeltaTime());
		    // launched boolean just helps user stop and start action
		    // and useful for debugging and teaching
     

		}
		currentScene->Render();

		/// Keep the event loop running at a proper rate
		SDL_Delay(timer->GetSleepTime(60)); ///60 frames per sec
	}
    //std::cout << "endMem: " << curMem << std::endl;
    //std::cout << "maxMem: " << maxMem << std::endl;
    //std::cout << "allocs: " << totalAllocations << std::endl;
    //std::cout << "deallocs: " << totalDeallocations << std::endl;

}

void GameManager::handleEvents()
{
	SDL_Event event;

        // Let's add mouse movement and position
        // https://wiki.libsdl.org/SDL_GetMouseState

        SDL_PumpEvents();  // make sure we have the latest mouse state.

        //https://www.youtube.com/watch?v=SYrRMr4BaD4&list=PLM7LHX-clszBIGsrh7_3B2Pi74AhMpKhj&index=3
        while( SDL_PollEvent(&event) )
        {
            if( event.type == SDL_QUIT )
            {
                isRunning = false;
            }
            else if( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.scancode )
                {
                    case SDL_SCANCODE_ESCAPE:
                        isRunning = false;
                        break;
                    case SDL_SCANCODE_Q:
                        isRunning = false;
                        break;
                    case SDL_SCANCODE_DELETE:
                        isRunning = false;
                        break;
                    case SDL_SCANCODE_L:
                        //toggle launched with L key)
                        launched = !launched;
                        break;
                    case SDL_SCANCODE_1:
                        LoadScene( 1 );
                        break;
                    case SDL_SCANCODE_2:
                        LoadScene(2);
                        break;
                    default:
                        break;
                }
            }
            currentScene->HandleEvents( event );
        }
}

GameManager::~GameManager() {}

void GameManager::OnDestroy(){
	if (windowPtr) delete windowPtr;
	if (timer) delete timer;
	if (currentScene) delete currentScene;
}

float GameManager::getSceneHeight()
{ return currentScene->getyAxis(); }

float GameManager::getSceneWidth()
{ return currentScene->getxAxis(); }

Matrix4 GameManager::getProjectionMatrix() 
{ return currentScene->getProjectionMatrix(); }

SDL_Renderer* GameManager::getRenderer()
{
    // [TODO] might be missing some SDL error checking
    SDL_Window* window = currentScene->getWindow();
    SDL_Renderer* renderer = SDL_GetRenderer(window);
    return renderer;
}

void GameManager::RenderPlayer(float scale)
{
    player->Render(scale);
}

void GameManager::LoadScene( int i )
{
    // cleanup of current scene before loading anothe one
    currentScene->OnDestroy();
    delete currentScene;
    switch ( i )
    {
        case 1:
            currentScene = new Scene1( windowPtr->GetSDL_Window(), this);
            break;
        case 2:
            break;
        default:
            currentScene = new Scene1( windowPtr->GetSDL_Window(), this );
            break;
    }
    // using ValidateCurrentScene() to safely run OnCreate
    if (!ValidateCurrentScene())
    {
        isRunning = false;
    }
    launched = true;
}

bool GameManager::ValidateCurrentScene()
{
    if (currentScene == nullptr) {
        return false;
    }
    if (currentScene->OnCreate() == false) {
        return false;
    }
    return true;
}
