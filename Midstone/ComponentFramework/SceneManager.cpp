#include <SDL.h>
#include "SceneManager.h"
#include "Timer.h"
#include "Window.h"
#include "Scene0g.h"
#include "Scene0p.h"
#include "Scene1g.h"
#include "SceneUI.h"
#include "SceneUI2.h"
#include "Scene2g.h"
#include "Scene3g.h"
#include "Scene5g.h"
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


	BuildNewScene(SCENE_NUMBER::SCENEUI2); 


	/********************************************************************************/
	return true;
}

/// This is the whole game
void SceneManager::Run() {
	timer->Start();
	isRunning = true;
	

	/*while (isRunning && mainMenu) {
		HandleEvents();
		
		timer->UpdateFrameTicks();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();

		SDL_Delay(timer->GetSleepTime(fps));
	}*/
	while (isRunning) 
	{
		HandleEvents();
		timer->UpdateFrameTicks();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();
		
		if (!mainMenu) {
			SDL_GL_SwapWindow(window->getWindow());
		}
		else if(currentScene->switchButton) {
			currentScene->switchButton = false;
			BuildNewScene(SCENE_NUMBER::SCENE3g);
			mainMenu = false;
		}
		

		SDL_Delay(timer->GetSleepTime(fps));
	}
	/*while (BuildNewScene(SCENE_NUMBER::SCENEUI))
	{
		if (ans.switchButton == true)
		{
			BuildNewScene(SCENE_NUMBER::SCENE0g);
			
		}
	}*/
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
			case SDL_SCANCODE_Q:
				isRunning = false;
				return;
				

			case SDL_SCANCODE_F1:
				currentSceneNumber += 1;
				BuildNewScene(SCENE_NUMBER::SCENE0g);
				break;
			case SDL_SCANCODE_F2:
				currentSceneNumber -= 1;
				BuildNewScene(SCENE_NUMBER::SCENE1g);
				break;
			case SDL_SCANCODE_F3:
			case SDL_SCANCODE_F4:
			case SDL_SCANCODE_F5:
		
				
				break;

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
	case SCENE_NUMBER::SCENE0g:
		currentScene = new Scene0g();
		status = currentScene->OnCreate();
		//if (currentSceneNumber == 0) break;
		break;
	case SCENE_NUMBER::SCENE0p:
		currentScene = new Scene0p();
		status = currentScene->OnCreate();
		//if (currentSceneNumber == 1) break;
		break;
	case SCENE_NUMBER::SCENE1g:
		currentScene = new Scene1g();
		status = currentScene->OnCreate();
		//if (currentSceneNumber == 2) break;
		break;

	/*case SCENE_NUMBER::SCENE1g:
		currentScene = new Scene1g();
		status = currentScene->OnCreate();
		break;*/

	case SCENE_NUMBER::SCENE2g:
		currentScene = new Scene2g(window);
		status = currentScene->OnCreate();
		//if (currentSceneNumber == 2) break;
		break;

	case SCENE_NUMBER::SCENE3g:
		currentScene = new Scene3g();
		status = currentScene->OnCreate();
		//if (currentSceneNumber == 2) break;
		break;

	case SCENE_NUMBER::SCENEUI:
		currentScene = new SceneUI(window->getWindow());
		mainMenu = true;
		status = currentScene->OnCreate();
		//if (currentSceneNumber == 2) break;
		 
		break;
	case SCENE_NUMBER::SCENEUI2:
		currentScene = new SceneUI2(window);
		status = currentScene->OnCreate();
		//if (currentSceneNumber == 2) break;

		break;

	case SCENE_NUMBER::SCENE5g:
		currentScene = new Scene5g();
		status = currentScene->OnCreate();
		//if (currentSceneNumber == 2) break;

		break;

	default:
		Debug::Error("Incorrect scene number assigned in the manager", __FILE__, __LINE__);
		currentScene = nullptr;
		return false;
	}
	return true;
}


