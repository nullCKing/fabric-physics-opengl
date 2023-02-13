#include "FabricContainer.h"

FabricContainer::FabricContainer(int width, int height)
{
	this->height = height;
	this->width = width;

	particleCount = height * width;
	vertices.resize(particleCount);
	normals.resize(particleCount);
	tans.resize(particleCount);

	//Define texture coords
	float xPos = 1.f / width;
	float yPos = 1.f / height;

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			ParticleContainer* Particle;
			Particle = new ParticleContainer(glm::vec3(((float)x / 8) - (width / 16), ((float)y / 8) - (height / 16), 0));
			particleList.push_back(Particle);

			UVs.push_back(glm::vec2(xPos * float(x), -yPos * (float)y));
		}
	}

	/*
	* The loop iterates through a 2D grid of width and height, with each iteration defining a particle container.
	* It then creates constraints between the current particleand other particles in the grid, based on its position.This includes stretch constraints between horizontally or vertically adjacent particles, shear constraints between diagonal particles, and bend constraints between more distant particles.
	* The GetIndexFromGridCoord function is used to convert the grid coordinates into an index for accessing the correct particle from the particleList.The created constraints are then added to the constraintsList.
	*/
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			ParticleContainer* particleX = particleList[GetIndexFromGridCoord(x, y)];
			
			//Stretch
			if (x + 1 < width)
			{
				ParticleContainer* particleY = particleList[GetIndexFromGridCoord(x + 1, y)];
				constraintsList.push_back(new ConstraintContainer(particleX, particleY));
			}

			if (y + 1 < height)
			{
				ParticleContainer* particleY = particleList[GetIndexFromGridCoord(x, y + 1)];
				constraintsList.push_back(new ConstraintContainer(particleX, particleY));
			}

			//Shear
			if (x + 1 < width && y + 1 < height)
			{
				ParticleContainer* particleY = particleList[GetIndexFromGridCoord(x + 1, y + 1)];
				constraintsList.push_back(new ConstraintContainer(particleX, particleY));
			}

			if (x - 1 >= 0 && y + 1 < height)
			{
				ParticleContainer* particleY = particleList[GetIndexFromGridCoord(x - 1, y + 1)];
				constraintsList.push_back(new ConstraintContainer(particleX, particleY));
			}

			//Bend
			if (x + 2 < width && y + 2 < height)
			{
				ParticleContainer* particleY = particleList[GetIndexFromGridCoord(x + 2, y + 2)];
				constraintsList.push_back(new ConstraintContainer(particleX, particleY));
			}

			if (x + 2 < width && y < height)
			{
				ParticleContainer* particleY = particleList[GetIndexFromGridCoord(x + 2, y)];
				constraintsList.push_back(new ConstraintContainer(particleX, particleY));
			}

			if (x < width && y + 2 < height)
			{
				ParticleContainer* particleY = particleList[GetIndexFromGridCoord(x, y + 2)];
				constraintsList.push_back(new ConstraintContainer(particleX, particleY));
			}

			if (x + 2 < width && y + 2 < height)
			{
				ParticleContainer* particleY = particleList[GetIndexFromGridCoord(x, y + 2)];
				constraintsList.push_back(new ConstraintContainer(particleList[GetIndexFromGridCoord(x, y + 2)], particleList[GetIndexFromGridCoord(x + 2, y)]));
			}
		}
	}

	/*
	* This code is setting the moveable attribute of particles in the bottom row of a grid to false. 
	* The grid has a width of width and a height of height, and each particle in the grid is stored in an array called particleList.
	*/
	for (unsigned int i = 0; i < width; i++)
	{
		particleList[GetIndexFromGridCoord(i, height - 1)]->moveable = false;
	}

	//Setup buffers
	glGenVertexArrays(1, &VertexArrayBuffer);
	glGenBuffers(NUMOFBUFFERS, BufferArrayObjects);

	/*
	* The indices are being used to define a triangular mesh over the grid of particles. 
	* Each particle in the grid is represented by a single vertex in the mesh.
	* For each pair of column and row, two triangles are created and their indices are added to the indices vector.
	*/
	for (unsigned int x = 0; x < width - 1; x++)
	{
		for (unsigned int z = 0; z < height - 1; z++)
		{
			//First triangle
			indices.push_back(x * height + z);
			indices.push_back(x * height + z + 1);
			indices.push_back((x + 1) * height + z);

			//Second triangle
			indices.push_back((x + 1) * height + z);
			indices.push_back(x * height + z + 1);
			indices.push_back((x + 1) * height + z + 1);
		}
	}

	/*
	* Creates faces for the fabric
	* Loops through the width+height of grid, creating faces made of 4 particles and their respective UV coordinates. 
	* For each iteration of the loop, a FabricFace object is created, and its properties (p1, uv1, p2, uv2, p3, uv3, p4, and uv4) 
	* are set to the particle and UV coordinate values corresponding to the current grid coordinates (x and y). 
	* The FabricFace object is then added to the 'faces' vector. Faces will be used for simulating wind displacements/reactions.
	*/
	for (int x = 0; x < width - 1; x++)
	{
		for (int y = 0; y < height - 1; y++)
		{
			FabricFace Face;
			Face.p1 = particleList[GetIndexFromGridCoord(x, y)];
			Face.uv1 = UVs[GetIndexFromGridCoord(x, y)];

			Face.p2 = particleList[GetIndexFromGridCoord(x + 1, y)];
			Face.uv2 = UVs[GetIndexFromGridCoord(x + 1, y)];

			Face.p3 = particleList[GetIndexFromGridCoord(x + 1, y + 1)];
			Face.uv3 = UVs[GetIndexFromGridCoord(x + 1, y + 1)];

			Face.p4 = particleList[GetIndexFromGridCoord(x, y + 1)];
			Face.uv4 = UVs[GetIndexFromGridCoord(x, y + 1)];

			faces.push_back(Face);
		}
	}
}

FabricContainer::~FabricContainer()
{
	for (size_t i = 0; i < particleCount; i++)
	{
		delete particleList[i];
	}

	for (size_t i = 0; i < constraintsList.size(); i++)
	{
		delete constraintsList[i];
	}
}

void FabricContainer::Update(float deltaTime)
{
	//Gravitational forces are applied to each particle.
	for (size_t i = 0; i < particleList.size(); i++)
	{
		particleList[i]->AddForce(glm::vec3(0, -0.04f, 0));
		particleList[i]->Update(deltaTime);
	}

	for (size_t i = 0; i < constraintsList.size(); i++)
	{
		constraintsList[i]->Update(deltaTime);
		
	}
	Wind(windDirection);

	//Input each particle position into vertices list.
	for (unsigned int i = 0; i < particleList.size(); i++)
	{
		vertices[i] = particleList[i]->GetPosition();
	}

	/* 
	* The normal and tangent vectors of each face in the faces array are updated.
	* This is done by first computing the normal vector of the face and then computing the tangent vector
	* by considering the change in positionand texture coordinates of the particles that make up the face.
	*/
	for (size_t i = 0; i < faces.size(); i++)
	{
		faces[i].normal = ComputeNormalNormalized(faces[i].p1, faces[i].p2, faces[i].p3);
			
		glm::vec3 deltaPos1 = faces[i].p2->GetPosition() - faces[i].p1->GetPosition();
		glm::vec3 deltaPos2 = faces[i].p3->GetPosition() - faces[i].p1->GetPosition();

		glm::vec2 deltaUV1 = faces[i].uv2 - faces[i].uv1;
		glm::vec2 deltaUV2 = faces[i].uv3 - faces[i].uv1;

		float r = 1 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

		faces[i].tangent = glm::normalize((deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r);
	}

	/*
	* Goes through every face of each particle, checking if the particle and face are connnected.
	* If the current particle is connected to the face (either as the p1, p2, p3, or p4 particle of the face)
	* the normal and tangent vectors of that face are added to the normal and tangent vectors of the current particle. 
	* After iterating through all the faces, the normal and tangent vectors for the current particle are stored in the normals and tans lists, respectively.
	*/
	for (size_t i = 0; i < particleList.size(); i++)
	{
		ParticleContainer* particle = particleList[i];
		glm::vec3 n(0, 0, 0);
		glm::vec3 t(0, 0, 0);

		for (unsigned int k = 0; k < faces.size(); k++)
		{
			if (particle == faces[k].p1 || particle == faces[k].p2 || particle == faces[k].p3 || particle == faces[k].p4)
			{
				n += faces[k].normal;
				t += faces[k].tangent;
			}
		}

		normals[i] = n;
		tans[i] = t;
	}

	/*
	* GL boilerplate code; initializes the buffers required for vertex attributes such as position, normals, and tangents. 
	*It is used to specify how vertex data is stored and accessed in memory so that it can be sent to the graphics card to be rendered.
	*/

	glBindVertexArray(VertexArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,BufferArrayObjects[POSITION]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferArrayObjects[ELEMENT]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, BufferArrayObjects[NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, BufferArrayObjects[TANGENTS]);
	glBufferData(GL_ARRAY_BUFFER, tans.size() * sizeof(glm::vec3), &tans[0], GL_STATIC_DRAW);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
}

/*
* sets up and uses a shader program, sets the model, view, and projection matrices for the scene, sets up textures, 
* and draws elements using a vertex array buffer. The type of drawing is determined by a draw type integer (iDrawType),
* and a normal mapping flag (bNormalMapping) is also checked to determine which textures to bind. The final draw call is done using glDrawElements. 
*/
void FabricContainer::Render(CameraContainer Camera, GLuint Shader)
{
	glUseProgram(Shader);

	//Lets make our models matrix first
	glm::mat4 positionMatrix = glm::translate(glm::vec3(0, 0, 0));
	glm::mat4 scaleMatrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 modelMatrix = positionMatrix * scaleMatrix;

	glUniformMatrix4fv(glGetUniformLocation(Shader, "Model"), 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Shader, "View"), 1, GL_FALSE, &Camera.viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Shader, "Projection"), 1, GL_FALSE,&Camera.projectionMatrix[0][0]);
	glUniform1i(glGetUniformLocation(Shader, "Texture01"), 0);
	glUniform1i(glGetUniformLocation(Shader, "Texture02"), 1);

	/*
	* By binding a VAO, we can ensure that all of the vertex attribute states needed to draw our geometry are properly set up, 
	* without having to repeatedly set them up every time we want to draw something. glBindVertexArray is being used to bind the VAO 
	* stored in the variable VertexArrayBuffer. This sets the VAO as the current vertex array object, so that any subsequent vertex 
	* attribute setup calls will modify the VAO's state.
	*/
	glBindVertexArray(VertexArrayBuffer);

	/*
	* Fairly straightforward: we are binding the 'base' texture pattern as well as our normal map texture.
	* Normal map adds surface detail and provides more detailed lighting calculations, without adding additional geometry.
	*/
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, glTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap);

	//GL_LINES will produce a wireframe rendering. GL_POINTS will produce a dot grid rendering. GL_TRIANGLES is our default state.
	switch (drawElements)
	{
	case 0:
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
		break;

	case 1:
		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, (void*)0);
		break;

	case 2:
		glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, (void*)0);
		break;

	default:
		break;
	}
	

	glBindVertexArray(0);
}


// Calculates the necessary force to apply to the particles of the face, generating a wind effect.
void FabricContainer::GenerateWind(FabricFace Face, glm::vec3 direction)
{
	glm::vec3 Normal = ComputeNormal(Face.p1->GetPosition(), Face.p2->GetPosition(), Face.p3->GetPosition());
	glm::vec3 force = ComputeForce(Normal, direction);

	Face.p1->AddForce(force);
	Face.p2->AddForce(force);
	Face.p3->AddForce(force);
	Face.p4->AddForce(force);
}


void FabricContainer::Wind(glm::vec3 direction)
{
	for (size_t i = 0; i < faces.size(); i++)
	{
		GenerateWind(faces[i], direction);
	}
}

ParticleContainer * FabricContainer::GetParticle(int Index)
{
	return particleList[Index];
}

void FabricContainer::AddTexture(std::string path)
{
	glGenTextures(1, &glTexture);
	glBindTexture(GL_TEXTURE_2D, glTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height,channels;
	unsigned char* data = SOIL_load_image(path.c_str(), &width, &height, &channels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(data);

	glBindVertexArray(VertexArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, BufferArrayObjects[TEXTURE_COORDS]);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

void FabricContainer::AddNormalMap(std::string path)
{
	glGenTextures(1, &normalMap);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels;
	unsigned char* data = SOIL_load_image(path.c_str(), &width, &height, &channels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(data);
}

/*
* Calculates and returns the force applied on an object given its normal and direction vectors. 
* It first normalizes the normal vector and then calculates the force by taking the dot product of the normalized normal
* and direction vectors and multiplying it with the normal vector. This force will be applied to each of the four FabricFace points.
*/
glm::vec3 FabricContainer::ComputeForce(glm::vec3 normal, glm::vec3 direction)
{
	glm::vec3 d = glm::normalize(normal);
	return normal * (glm::dot(d, direction));
}

/*
* Calculates the normal vector of a 3D plane defined by 3 points a, b, and c. 
* Normal is taken by the cross product of two vectors, b - a and c - a. 
* The cross product of two vectors gives a third vector that is orthogonal to both of the original vectors.
*/
glm::vec3 FabricContainer::ComputeNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	return glm::cross(b - a, c - a);
}

// Returns the normalized cross product of two vectors, which is then normalized.
glm::vec3 FabricContainer::ComputeNormalNormalized(ParticleContainer * a, ParticleContainer * b, ParticleContainer * c)
{
	return glm::normalize(glm::cross(b->GetPosition() - a->GetPosition(), c->GetPosition() - a->GetPosition()));
}

int FabricContainer::GetIndexFromGridCoord(int x, int y)
{
	return (x * height + y);
}
