#include <SDL.h>
#include "SceneManager.h"
#include "Timer.h"
#include "Window.h"
#include "Scene0g.h"
#include "Scene0p.h"
#include "Scene1g.h"
#include "SceneUI.h"
#include "SceneMainMenu.h"
#include "Scene2g.h"
#include "SceneGameplay.h"
#include "Scene5g.h"
#include "SceneGameOver.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//SceneUI ans;


SceneManager::SceneManager(): 
	currentScene{nullptr}, window{nullptr}, timer{nullptr},
	fps(60), isRunning{false}, fullScreen{false} {
	Debug::Info("Starting the SceneManager", __FILE__, __LINE__);
}

SceneManager::~SceneManager() {
	Debug::Info("Deleting the SceneManager", __FILE__, __LINE__);

	if (currentScene) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}
	
	if (timer) {
		delete timer;
		timer = nullptr;
	}

	if (window) {
		delete window;
		window = nullptr;
	}
	
}

bool SceneManager::Initialize(std::string name_, int width_, int height_) {

	window = new Window();
	if (!window->OnCreate(name_, width_, height_)) {
		Debug::FatalError("Failed to initialize Window object", __FILE__, __LINE__);
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		Debug::FatalError("Failed to initialize Timer object", __FILE__, __LINE__);
		return false;
	}

	/********************************   Default first scene   ***********************/


	BuildNewScene(SCENE_NUMBER::SCENEMAINMENU); 


	/********************************************************************************/
	return true;
}

/// This is the whole game
void SceneManager::Run() {
	timer->Start();
	isRunning = true;
	

	while (isRunning) 
	{
		HandleEvents();
		timer->UpdateFrameTicks();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();
		currentScene->RenderIMGUI();
		SDL_GL_SwapWindow(window->getWindow());

		//use "flagspatchers" to know when to switch scenes 
		if (mainMenu && currentScene->switchButton)
		{
			currentScene->switchButton = false;
			BuildNewScene(SCENE_NUMBER::SCENEGAMEPLAY);
			mainMenu = false;
		}
		if (!mainMenu && currentScene->switchButton)
		{
			currentScene->switchButton = false;
			BuildNewScene(SCENE_NUMBER::SCENEMAINMENU);
			mainMenu = true;
		}
		if (!mainMenu && currentScene->gameOverBool)
		{
			currentScene->gameOverBool = false;
			BuildNewScene(SCENE_NUMBER::SCENEGAMEOVER);
			mainMenu = false;
		}
		if (!mainMenu && currentScene->restartBool)
		{
			currentScene->restartBool = false;
			BuildNewScene(SCENE_NUMBER::SCENEGAMEPLAY);
			mainMenu = false;
		}

		SDL_Delay(timer->GetSleepTime(fps));
	}

}

void SceneManager::HandleEvents() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) { /// Loop over all events in the SDL queue
		if (sdlEvent.type == SDL_EventType::SDL_QUIT) {
			isRunning = false;
			return;
		}
		else if (sdlEvent.type == SDL_KEYDOWN) {
			switch (sdlEvent.key.keysym.scancode) {
			[[fallthrough]]; /// C17 Prevents switch/case fallthrough warnings
			case SDL_SCANCODE_ESCAPE:
			case SDL_SCANCODE_Z:
				isRunning = false;
				return;
			case SDL_SCANCODE_F3:
			case SDL_SCANCODE_F4:
			case SDL_SCANCODE_F5:
			default:
				break;
			}
		}
		if (currentScene == nullptr) { /// Just to be careful
			Debug::FatalError("No currentScene", __FILE__, __LINE__);
			isRunning = false;
			return;
		}
		currentScene->HandleEvents(sdlEvent);
	}
}

bool SceneManager::BuildNewScene(SCENE_NUMBER scene) {
	bool status; 

	if (currentScene != nullptr) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}

	switch (scene) {
	
	case SCENE_NUMBER::SCENEGAMEPLAY:
		currentScene = new SceneGameplay(window);
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENEMAINMENU:
		currentScene = new SceneMainMenu(window);
		mainMenu = true;
		break;
	case SCENE_NUMBER::SCENEGAMEOVER:
		currentScene = new SceneGameOver(window);
		status = currentScene->OnCreate();
		break;

	default:
		Debug::Error("Incorrect scene number assigned in the manager", __FILE__, __LINE__);
		currentScene = nullptr;
		return false;
	}
	return true;
}


