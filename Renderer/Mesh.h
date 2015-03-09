#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <future>
#include <vector>
#include <cstdint>
#include <fstream>

#include "RawMeshData.h"
#include "Graphics.h"
#include "Defines.h"

#include <GL/glew.h>
#include <glm\glm.hpp>

class Mesh
{
public:
	Mesh();

	bool	Initialize(class RawMesh* rawMesh);
	void	Render(Graphics& graphics, glm::mat4x4 const& model);

	int		GetIndex() const;
	int		GetVersion() const;
	bool	IsReady() const;
	void	SetIndex(int index);
	void	SetVersion(int version);

private:
	RawMesh*			m_rawMesh;
	std::vector<GLuint>	m_vertexBuffers;
	std::vector<GLuint>	m_skinningVertexBuffer;
	std::vector<GLuint>	m_indexBuffers;

	int		m_index;
	int		m_version;
	bool	m_ready;
};
