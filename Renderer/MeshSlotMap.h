#pragma once

#include <list>
#include <vector>
#include <mutex>

#include "Mesh.h"
#include "Graphics.h"

struct MeshHandler
{
	int		Index;
	int		Version;
};

class MeshSlotMap
{
public:
	MeshSlotMap();
	~MeshSlotMap();

	MeshSlotMap(MeshSlotMap const&) = delete;
	MeshSlotMap(MeshSlotMap&&) = delete;
	MeshSlotMap&	operator=(MeshSlotMap const&) = delete;
	MeshSlotMap&	operator=(MeshSlotMap&&) = delete;

	bool	Initialize();
	void	Deinitialize();

	Mesh*	Get(MeshHandler id);
	Mesh*	CreateObject();
	void	Remove(MeshHandler id);
	int		Count() const;

	void	Render(Graphics& graphics, GLuint MatrixID, glm::mat4 proj, glm::mat4 view);

public:
	static const int	MaxObjects = 1024;

private:
	std::vector<Mesh>	m_objects;
	std::vector<int>	m_slots;
	std::list<int>		m_freeList;
	std::mutex			m_lock;
	int					m_count;
};