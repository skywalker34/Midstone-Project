#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "SceneUI2.h"
#include <MMath.h>
#include <QMath.h>
#include <VMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include "stb_image.h"

bool LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Upload pixels into texture
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

// Open and read a file, then forward to LoadTextureFromMemory()
bool LoadTextureFromFile(const char* file_name, GLuint* out_texture, int* out_width, int* out_height)
{
	FILE* f;
	errno_t err;
	if ((err = fopen_s(&f, file_name, "rb")) != 0)
	{
	}
	else
	{
		fseek(f, 0, SEEK_END);
		size_t file_size = (size_t)ftell(f);
		if (file_size == -1)
			return false;
		fseek(f, 0, SEEK_SET);
		void* file_data = IM_ALLOC(file_size);
		fread(file_data, 1, file_size, f);
		bool ret = LoadTextureFromMemory(file_data, file_size, out_texture, out_width, out_height);
		IM_FREE(file_data);
		return ret;
	}
	if (f == NULL)
		return false;
}

SceneUI2::SceneUI2(Window* window_) : drawInWireMode{ true }, show_demo_window {true} {
	Debug::Info("Created Scene2g: ", __FILE__, __LINE__);
	window = window_;

	// ImGUI stuff for initialize from Scotties Vid.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//Setup Dear ImGui Style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window->getWindow(), window->getContext());
	ImGui_ImplOpenGL3_Init("#version 450");
}

SceneUI2::~SceneUI2() {
	Debug::Info("Deleted Scene2g: ", __FILE__, __LINE__);
}

bool SceneUI2::OnCreate() {
	Debug::Info("Loading assets Scene2g: ", __FILE__, __LINE__);

	printf("On Create finished!!!!!");
	return true;


}

void SceneUI2::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void SceneUI2::HandleEvents(const SDL_Event& sdlEvent) 
{
	ImGui_ImplSDL2_ProcessEvent(&sdlEvent); // ImGui HandleEvents
}

void SceneUI2::Update(const float deltaTime) 
{
}

void SceneUI2::Render() const 
{
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// IMGUI STUFF
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// Get window size and position
	ImVec2 window_pos = ImGui::GetWindowPos();
	ImVec2 window_size = ImGui::GetWindowSize();
	int my_image_width = 0;
	int my_image_height = 0;
	GLuint my_image_texture = 0;
	bool ret = LoadTextureFromFile("./textures/StartPhoto.jpg", &my_image_texture, &my_image_width, &my_image_height);
	//IM_ASSERT(ret);
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	ImVec2 image_pos = ImVec2(0, 0); // Set image position
	drawList->AddImage((ImTextureID)(intptr_t)my_image_texture, image_pos, ImVec2(my_image_width / 1.5, my_image_height / 1.5));
	
	ImGui::Begin("A START BUTTON MAYBE?");
	static int clicked = 0;
	if (ImGui::Button("START?"))
		clicked++;
	if (clicked & 1)
	{
		ImGui::SameLine();
		ImGui::Text("Thanks for clicking me!");
	}
	ImGui::End();

	ImGui::ShowDemoWindow();
	ImGui::Render(); // Calling This before CurrentScene render wont work
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glUseProgram(0);
}




