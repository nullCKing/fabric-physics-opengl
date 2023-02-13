#ifndef SHADERINIT_H
#define SHADERINIT_H

#include <string>
#include <fstream>
#include <vector>

#include <glew.h>
#include <glfw3.h>

class ShaderInit
{
public:
	ShaderInit();
	~ShaderInit();

	//load the shaders
	GLuint getShaders(const char * _vertexShader, const char * _fragmentShader);

private:

};


#endif