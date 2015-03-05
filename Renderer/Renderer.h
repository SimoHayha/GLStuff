#pragma once

#include <GL/glew.h>

#include "Graphics.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	bool	Initialize();
	void	Deinitialize();
	void	Render(Graphics& graphics);

private:
	GLuint	m_vertexArrayID;
	GLuint	m_vertexBuffer;
	GLuint	m_shader;
};