#include "ConstraintContainer.h"

ConstraintContainer::ConstraintContainer(ParticleContainer * _particleX, ParticleContainer * _particleY)
{
	particleX = _particleX;
	particleY = _particleY;
	rest = glm::length(_particleX->GetPosition() - _particleY->GetPosition());
}

ConstraintContainer::~ConstraintContainer()
{

}

/*
* Updates the forces applied to two particles. Calculates the distance between the particles and finds a correction force
* to maintain a certain distance between them, specified by the rest value. The correction force is added to one particle 
* and the inverse to the other to move them towards or away from each other until they reach the desired distance.
*/
void ConstraintContainer::Update(float deltaTime)
{
	for (unsigned int i = 0; i < 15; i++)
	{
		glm::vec3 pX_to_pB = particleY->GetPosition() - particleX->GetPosition();
		float distant = glm::length(pX_to_pB);

		glm::vec3 correctionVector = (pX_to_pB * (1 - rest / distant) * .5f);

		particleX->AddForce(correctionVector);
		particleY->AddForce(-correctionVector);
	}
}
