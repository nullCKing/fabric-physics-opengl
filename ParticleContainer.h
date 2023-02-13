#ifndef PARTICLECONTAINER_H
#define PARTICLECONTAINER_H

#include <glm.hpp>
#include <vector>

class ParticleContainer
{
public:
	ParticleContainer(glm::vec3 pos);
	~ParticleContainer();

	void AddForce(glm::vec3 force);
	void Update(float deltaTime);
	bool moveable = true;
	void SetPosition(glm::vec3 _Val);
	glm::vec3 GetPosition();

private:
	glm::vec3 pos;
	glm::vec3 oldPos;
	glm::vec3 acceleration = glm::vec3(0,0,0);

	float mass = 1.f;
};
#endif