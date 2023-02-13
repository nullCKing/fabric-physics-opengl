#ifndef FABRICCONTAINER_H
#define FABRICCONTAINER_H

#include <vector>
#include <glm\gtx\transform.hpp>
#include <glm\matrix.hpp>
#include <SOIL.h>
#include "ParticleContainer.h"
#include "ConstraintContainer.h"
#include "CameraContainer.h"

struct FabricFace
{
	FabricFace(){};
	ParticleContainer* p1,* p2,* p3,* p4;
	glm::vec2 uv1, uv2, uv3, uv4;

	glm::vec3 normal;
	glm::vec3 tangent;
};

class FabricContainer
{
public:
	FabricContainer(int width, int height);
	~FabricContainer();

	void Update(float deltaTime);
	void Render(CameraContainer Camera, GLuint Shader);
	void GenerateWind(FabricFace Face, glm::vec3 direction);
	void Wind(glm::vec3 Direction);
	ParticleContainer* GetParticle(int Index);
	void AddTexture(std::string path);
	void AddNormalMap(std::string path);

	glm::vec3 ComputeForce(glm::vec3 normal, glm::vec3 direction);
	glm::vec3 ComputeNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	glm::vec3 ComputeNormalNormalized(ParticleContainer* a, ParticleContainer* b, ParticleContainer* c);
	int GetIndexFromGridCoord(int x, int y);

	int width;
	int height;

	int drawElements = 0;
	glm::vec3 windDirection;

	std::vector<ParticleContainer*> particleList;
private:
	enum{POSITION, ELEMENT, NORMAL, TEXTURE_COORDS, TANGENTS ,NUMOFBUFFERS};

	GLuint VertexArrayBuffer;
	GLuint BufferArrayObjects[NUMOFBUFFERS];

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tans;
	std::vector<glm::vec2> UVs;
	std::vector<FabricFace> faces;
	std::vector<unsigned int> indices;
	GLuint glTexture;
	GLuint normalMap;

	std::vector<ConstraintContainer*> constraintsList;
	int particleCount;
};
#endif