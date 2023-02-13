#include "ShaderInit.h"

ShaderInit::ShaderInit()
{
}

ShaderInit::~ShaderInit()
{
}

GLuint ShaderInit::getShaders(const char* vertexpath, const char* fragmentpath) {

    printf("****************** Loading Shaders ******************\nVertex: %s\nFragment: %s\n\n", vertexpath, fragmentpath);
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertexpath, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertexpath);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragmentpath, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    char const* VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(vs, 1, &VertexSourcePointer, NULL);
    glCompileShader(vs);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Check Vertex Shader
    glGetShaderiv(vs, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(vs, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    char const* FragSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(fs, 1, &FragSourcePointer, NULL);
    glCompileShader(fs);

    // Check Fragment Shader
    glGetShaderiv(fs, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(fs, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, fs);
    glAttachShader(program, vs);
    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(program);

        printf("Program Error Report:\n%s\n----------------------------\n", infoLog);

        return 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
