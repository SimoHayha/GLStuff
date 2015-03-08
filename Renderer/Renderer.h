#pragma once

#include <vector>

#include <GL/glew.h>

#include "Graphics.h"
#include "MeshSlotMap.h"

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
	//std::vector<Mesh*>	m_meshes;
	//std::vector<std::vector<Mesh*>>	m_samples;

	GLuint	m_vertexArrayID;
	GLuint	m_vertexBuffer;
	GLuint	m_shader;

	MeshSlotMap	m_meshes;;

	bool	m_ready;
};