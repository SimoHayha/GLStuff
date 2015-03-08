#pragma once

#include <vector>
#include <mutex>

#include "Graphics.h"

class Mesh;

class MeshContainer
{
public:
	MeshContainer() = default;
	~MeshContainer() = default;

	MeshContainer(MeshContainer const&) = delete;
	MeshContainer(MeshContainer&&) = delete;
	MeshContainer&	operator=(MeshContainer const&) = delete;
	MeshContainer&	operator=(MeshContainer&&) = delete;

	void	Add(Mesh* mesh);
	void	Remove(Mesh* mesh);
	void	Render(Graphics& graphics);

private:
	std::vector<Mesh*>	m_data;
	std::mutex			m_lock;
};