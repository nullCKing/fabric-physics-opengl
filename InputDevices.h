#pragma once
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <time.h>
#include "CameraContainer.h"
#include "FabricContainer.h"

class InputDevices
{
public:
	InputDevices();
	~InputDevices();
	void KeyboardCallback(GLFWwindow*, int, int, int, int, CameraContainer*, FabricContainer*);
	void MouseCallback(GLFWwindow*, double, double, float, float, float, float, CameraContainer*);
private:
};

