#ifndef CAMERACONTAINER_H
#define CAMERACONTAINER_H

#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class CameraContainer			
{
public:
	CameraContainer(int _width, int _height);
	~CameraContainer();

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	void update(float deltaTime, float);

	glm::vec3 upDirection;
	glm::vec3 frontView;
	glm::vec3 CameraRight;
	glm::vec3 pos;

	bool mouseMove;

	float cameraSpeed;
	bool keyArray[256];

private:
};
#endif