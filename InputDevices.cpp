#include "InputDevices.h"

InputDevices::InputDevices()
{
}

InputDevices::~InputDevices()
{
}

void InputDevices::KeyboardCallback(GLFWwindow* mainInstance, int key, int scancode, int action, int mods, CameraContainer* Camera, FabricContainer* Fabric)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(mainInstance,true);
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		Camera->keyArray['w'] = true;
	}
	else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		Camera->keyArray['w'] = false;
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		Camera->keyArray['s'] = true;
	}
	else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		Camera->keyArray['s'] = false;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		Camera->keyArray['a'] = true;
	}
	else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	{
		Camera->keyArray['a'] = false;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		Camera->keyArray['d'] = true;
	}
	else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		Camera->keyArray['d'] = false;
	}

	//Move camera down
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		Camera->keyArray['e'] = true;
	}
	else if (key == GLFW_KEY_E && action == GLFW_RELEASE)
	{
		Camera->keyArray['e'] = false;
	}

	//Move camera up
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		Camera->keyArray['q'] = true;
	}
	else if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
	{
		Camera->keyArray['q'] = false;
	}
	
	//Freeze camera movement
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		Camera->mouseMove = false;
	}
	else if (key == GLFW_KEY_T && action == GLFW_RELEASE)
	{
		Camera->mouseMove = true;
	}

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		Fabric->drawElements = 0;
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		Fabric->drawElements = 1;
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		Fabric->drawElements = 2;
	}

}

void InputDevices::MouseCallback(GLFWwindow* _window, double _xPos, double _yPos, float lastXpos, float lastYpos, float yaw, float pitch, CameraContainer* Camera)
{
	float xOffset = _xPos - lastXpos;
	float yOffset = lastYpos - _yPos;
	lastXpos = _xPos;
	lastYpos = _yPos;

	float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Camera->frontView = glm::normalize(front);
}
