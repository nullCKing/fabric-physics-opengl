#ifndef CONSTRAINTCONTAINER_H
#define CONSTRAINTCONTAINER_H

#include <vector>
#include <glm\gtx\transform.hpp>
#include <glm\matrix.hpp>
#include <glew.h>
#include <glfw3.h>
#include <sstream>
#include "ParticleContainer.h"
#include "CameraContainer.h"

class ConstraintContainer
{
public:
	ConstraintContainer(ParticleContainer* pA, ParticleContainer* pB);
	~ConstraintContainer();

	void Update(float deltaTime);

	float rest = 1.f;

private:
	ParticleContainer* particleX;
	ParticleContainer* particleY;
};
#endif