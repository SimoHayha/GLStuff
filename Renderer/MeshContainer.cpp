#include <algorithm>

#include "Mesh.h"
#include "MeshContainer.h"

void MeshContainer::Add(Mesh* mesh)
{
	std::unique_lock<std::mutex>	lock(m_lock);
	m_data.push_back(mesh);
}

void MeshContainer::Remove(Mesh* mesh)
{
	std::unique_lock<std::mutex>	lock(m_lock);
	auto	it = std::find(m_data.begin(), m_data.end(), mesh);
	if (it != m_data.end())
		m_data.erase(it);
}

void MeshContainer::Render(Graphics& graphics)
{
	std::unique_lock<std::mutex>	lock(m_lock);
}
