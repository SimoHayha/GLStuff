#pragma once

#include <vector>

#include <GL/glew.h>

#include "Graphics.h"

class Mesh;

class Renderer
{
public:
	Renderer();
	~Renderer();

	bool	Initialize(Graphics& graphics);
	void	Deinitialize();
	void	Render(Graphics& graphics);

private:
	std::vector<Mesh*>	m_meshes;

	GLuint	m_vertexArrayID;
	GLuint	m_vertexBuffer;
	GLuint	m_shader;
};