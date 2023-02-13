#include "ParticleContainer.h"

ParticleContainer::ParticleContainer(glm::vec3 _pos)
{
	pos = _pos;
	oldPos = pos;
}

ParticleContainer::~ParticleContainer()
{

}

//Second law of motion
void ParticleContainer::AddForce(glm::vec3 force)
{
	acceleration += force / mass;
}

void ParticleContainer::Update(float deltaTime)
{
	float DAMPING = 0.97f;

	if (moveable)
	{
		/*Verlet Integration*/
		glm::vec3 Temp = pos;
		pos = pos + (pos - oldPos) * DAMPING + acceleration * (deltaTime);
		oldPos = Temp;
		acceleration = glm::vec3(0, 0, 0);
	}
}

void ParticleContainer::SetPosition(glm::vec3 _Val)
{
	pos = _Val;
}

glm::vec3 ParticleContainer::GetPosition()
{
	return pos;
}
