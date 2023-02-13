#include <string>
#include <fstream>
#include <vector>

#include <glew.h>
#include <glfw3.h>

class ShaderContainer
{
public:
	ShaderContainer();
	~ShaderContainer();
	
	GLuint litShader;
	GLuint flatShader;
	GLuint backgroundShader;

private:

};

