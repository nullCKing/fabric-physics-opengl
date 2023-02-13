#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <time.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "InputDevices.h"
#include "ShaderInit.h"
#include "FabricContainer.h"
#include "ShaderContainer.h"

ShaderContainer SC;
GLFWwindow* MAIN_WINDOW;
FabricContainer* Fabric;
InputDevices* input_devices;

//Delta time
double dT = 1.f/60.f;
double oldTime;
double newTime;

//Load our camera
CameraContainer* Camera;
float lastXpos = 600;
float lastYpos = 400;
float yaw = 270;
float pitch = 0;
bool freezeCamera = false;

void Shutdown()
{
	glfwTerminate();
	delete Fabric;
	delete Camera;
}

void AutoFullscreen()
{
	float width = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
	float height = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
	glfwSetWindowMonitor(MAIN_WINDOW, glfwGetWindowMonitor(MAIN_WINDOW), 0, 0, width, height, 60);
}

//Press T to stop camera movement
bool CamFreezeHandler()
{
	if (Camera->mouseMove == false)
	{
		return false;
	}
	else if (Camera->mouseMove == true)
	{
		return true;
	}
}

//Wrapping these functions, decrease main bloating, hopefully this is straightforward.
void key_callback_wrapper(GLFWwindow* window, int key, int scancode, int action, int mods) {
	input_devices = reinterpret_cast<InputDevices*>(glfwGetWindowUserPointer(window));
	input_devices->KeyboardCallback(window, key, scancode, action, mods, Camera, Fabric);
}

void mouse_callback_wrapper(GLFWwindow* _window, double _xPos, double _yPos) {
	if (CamFreezeHandler() != false)
	{
		input_devices = reinterpret_cast<InputDevices*>(glfwGetWindowUserPointer(_window));
		input_devices->MouseCallback(_window, _xPos, _yPos, lastXpos, lastYpos, yaw, pitch, Camera);
	}
}



int main()
{

	glfwInit();

	// Decide GL+GLSL versions

	#if defined(IMGUI_IMPL_OPENGL_ES2)
		// GL ES 2.0 + GLSL 100
		const char* glsl_version = "#version 100";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	#elif defined(__APPLE__)
		// GL 3.2 + GLSL 150
		const char* glsl_version = "#version 150";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
	#else
		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

	#endif

	//Create a window
	glfwWindowHint(GLFW_SAMPLES, 4);
	MAIN_WINDOW = glfwCreateWindow(1920, 1080, "Fabric Simulation", NULL, NULL);

	AutoFullscreen();

	//make this window the current context
	glfwMakeContextCurrent(MAIN_WINDOW);

	//initailize GLEW library
	GLenum glewChk = glewInit();

	//Set callbacks
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(MAIN_WINDOW, GLFW_CURSOR, GLFW_CURSOR);
	glfwSetKeyCallback(MAIN_WINDOW, key_callback_wrapper);
	glfwSetCursorPosCallback(MAIN_WINDOW, mouse_callback_wrapper);

	//Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(MAIN_WINDOW, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	srand(time(NULL));
	glPointSize(5.0f);
	
	ShaderInit SL;
	SC.flatShader = SL.getShaders("Assets/Shaders/flat.vs", "Assets/Shaders/flat.fs");
	SC.litShader = SL.getShaders("Assets/Shaders/lit.vs", "Assets/Shaders/lit.fs");
	SC.backgroundShader = SL.getShaders("Assets/Shaders/backdrop.vs", "Assets/Shaders/backdrop.fs");

	//Load the initial fabric.
	Fabric = new FabricContainer(32, 32);
	Fabric->AddTexture("Assets/Textures/2.jpg");
	Fabric->AddNormalMap("Assets/Textures/n2.png");
	Camera = new CameraContainer(1920, 1080);

	//show_window and lock_camera. need to implement further to switch between Imgui interface and main OpenGL window
	bool show_window = true;
	bool lock_camera = false;
	int key;
	int action;
	static float speed = 0;
	static float width = 8;
	static float height = 8;
	static float widthTrack = 8;
	static float heightTrack = 8;

	//Probably a more effecient way to do this, but this tracks if the texture file needs to be reloaded
	char fileBuffer[256] = { "Assets/Textures/Fabric/2.jpg" };
	char fileBufferTrack[256] = { "Assets/Textures/Fabric/2.jpg" };
	
	//Wind forces
	static float dirI = 0;
	static float dirJ = 0;
	static float dirK = 0;

	while (!glfwWindowShouldClose(MAIN_WINDOW))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_window)
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Fabric Physics Simulator");						

			ImGui::SliderFloat("CameraSpeed", &speed, 0.0f, 100.0f);           
			ImGui::SliderFloat("FabricWidth", &width, 8.0f, 128.0f);
			ImGui::SliderFloat("FabricHeight", &height, 8.0f, 128.0f);
			ImGui::SliderFloat("WindForceI", &dirI, 0.0f, 15.0f);
			ImGui::SliderFloat("WindForceJ", &dirJ, 0.0f, 15.0f);
			ImGui::SliderFloat("WindForceK", &dirK, 0.0f, 15.0f);

			if ((widthTrack != width) || (heightTrack != height))
			{
				delete Fabric;

				Fabric = new FabricContainer(width, height);
				Fabric->AddTexture("Assets/Textures/Fabric/2.jpg");
				Fabric->AddNormalMap("Assets/Textures/Fabric/n2.png");
				widthTrack = width;
				heightTrack = height;
			}

			if (ImGui::Button("Disable Cursor")) {
				printf("Cursor Disabled\n");
				glfwSetInputMode(MAIN_WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}

			if (ImGui::Button("Default")) {
				Fabric->drawElements = 0;
			}

			ImGui::SameLine();
			if (ImGui::Button("Wireframe")) {
				Fabric->drawElements = 1;
			}
			ImGui::SameLine();
			if (ImGui::Button("Dot grid")) {
				Fabric->drawElements = 2;
			}

			ImGui::Text("Change fabric texture");
			ImGui::SameLine();
			ImGui::InputText("Text Entry", fileBuffer, 256);

			if (strcmp(fileBuffer, fileBufferTrack) != 0)
			{
				delete Fabric;

				Fabric = new FabricContainer(width, height);
				Fabric->AddTexture(fileBuffer);
				Fabric->AddNormalMap("Assets/Textures/Fabric/n2.png");
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		Fabric->windDirection = glm::normalize(glm::vec3(rand() % 10 - dirI + 1, rand() % 10 - dirJ + 1, rand() % 10 - dirK + 1));
		Fabric->windDirection *= 0.8f;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_TEST);

		Fabric->Update(dT);
		Fabric->Render(*Camera, SC.litShader);

		ImGui_ImplGlfw_NewFrame();

		ImGui::Render();
		Camera->update(dT, speed);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(MAIN_WINDOW);
		glfwPollEvents();
	}

	//Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	Shutdown();
}