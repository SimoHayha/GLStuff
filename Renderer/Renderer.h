#pragma once

#include <vector>

#include "MeshLoader.h"
#include "Graphics.h"
#include "MeshSlotMap.h"

#include <GL/glew.h>

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
	MeshLoader	m_meshLoader;

	bool	m_ready;
};