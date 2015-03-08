#pragma once

#include <GL\glew.h>

class Resources
{
public:
	static GLuint	LoadShader(char const* vertexFilePath, char const* fragmentFilePath);
	 
protected:
	Resources();
};