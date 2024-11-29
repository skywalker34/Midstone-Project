#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
class SceneManager  {
public:
	

	SceneManager();
	~SceneManager();
	void Run();
	bool Initialize(std::string name_, int width_, int height_);
	void HandleEvents();
	int currentSceneNumber;
	
	
	
private:
	//different scenes that can be loaded
	enum class SCENE_NUMBER {
		SCENEGAMEPLAY,
		SCENEMAINMENU,
		SCENEGAMEOVER
	};

	class Scene* currentScene; //scene taht is running
	class Timer* timer;
	class Window* window;
	
	unsigned int fps; //frame rate 
	bool isRunning; //whetehr teh game is running or noo
	bool fullScreen; //whther to run in fullscreen or not (this is a thing????)
	bool mainMenu = false; //whether the atcive scene is the main menu or not
	bool BuildNewScene(SCENE_NUMBER scene_);
};


#endif // SCENEMANAGER_H