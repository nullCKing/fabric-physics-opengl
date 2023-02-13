#include "CameraContainer.h"

CameraContainer::CameraContainer(int _width, int _height)
{
	mouseMove = true;
	/*
	* Sets up the initial position and orientation of a camera in a 3D environment. 
	* The camera is positioned at (0, 0, 10) and is looking towards the origin (0, 0, 0). 
	* The camera's up direction is set to be along the positive y-axis (0, 1, 0) and 
	* its front direction is calculated as the direction from its position to the origin, normalized to a unit vector.
	*/

	pos = glm::vec3(0, 0 , 10);
	cameraSpeed = 0;

	for (unsigned int i = 0; i < 256; i++)
	{
		keyArray[i] = false;
	}

	upDirection = glm::vec3(0, 1, 0);
	frontView = glm::normalize(glm::vec3(0, 0, 0) - pos);

	/*
	* sets up the projection and view matrices for the camera. Projection uses the glm::perspective function
	* and specifies the field of view, aspect ratio, near and far clipping plane distances. 
	*/
	projectionMatrix = glm::perspective(glm::radians(45.0f), (float)_width / (float)_height, 0.1f, 100000.0f);

	/*
	* View matrix determines the position, direction, and up direction of the camera.  
	* The camera is initialized with it pointed at the origin, (0,0,0). 
	*/
	viewMatrix = glm::lookAt(pos, glm::vec3(0, 0, 0), upDirection);
}

CameraContainer::~CameraContainer()
{

}

void CameraContainer::update(float deltaTime, float speed)
{
	if (mouseMove == true)
	{
		glm::vec3 normal = glm::normalize(glm::cross(frontView, upDirection));
		float deltaSpeed = cameraSpeed * deltaTime;

		if (keyArray['w'] == true)
		{
			pos += deltaSpeed * frontView;
		}
		else if (keyArray['s'] == true)
		{
			pos += -deltaSpeed * frontView;
		}

		if (keyArray['a'] == true)
		{
			pos -= deltaSpeed * normal;
		}
		else if (keyArray['d'] == true)
		{
			pos += deltaSpeed * normal;
		}

		if (keyArray['e'] == true)
		{
			pos += deltaSpeed * upDirection;
		}
		else if (keyArray['q'] == true)
		{
			pos += -deltaSpeed * upDirection;
		}

	}

	viewMatrix = glm::lookAt(pos, (pos + frontView), upDirection);
	cameraSpeed = speed;
}
