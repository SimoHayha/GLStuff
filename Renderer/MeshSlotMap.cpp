#include <iostream>

#include <glm\gtc\matrix_transform.hpp>

#include "MeshSlotMap.h"

Mesh* MeshSlotMap::Get(MeshHandler id)
{
	std::unique_lock<std::mutex>	lock(m_lock);

	int	index = m_slots[id.Index];
	if (m_objects[index].GetVersion() == id.Version)
		return &m_objects[index];
	else
		return nullptr;
}

Mesh* MeshSlotMap::CreateObject()
{
	std::unique_lock<std::mutex>	lock(m_lock);

	int	index = m_freeList.front();
	m_freeList.pop_front();

	m_objects[m_count].SetIndex(index);
	m_objects[m_count].SetVersion(m_objects[m_count].GetVersion() + 1);

	m_slots[index] = m_count;

	Mesh*	object = &m_objects[m_count];
	// TODO: private init

	m_count += 1;

	return object;
}

void MeshSlotMap::Remove(MeshHandler id)
{
	std::unique_lock<std::mutex>	lock(m_lock);

	int	index = m_slots[id.Index];
	if (m_objects[index].GetVersion() == id.Version)
	{
		m_objects[index].SetVersion(m_objects[m_count].GetVersion() + 1);
		m_objects[m_count - 1].SetVersion(m_objects[m_count - 1].GetVersion() - 1);

		std::swap(m_objects[index], m_objects[m_count - 1]);
	}
}

bool MeshSlotMap::Initialize()
{
	m_count = 0;
	m_objects.resize(MaxObjects);
	m_freeList.resize(MaxObjects);
	int i = 0;
	for (auto& it : m_freeList)
		it = i++;
	m_slots.resize(MaxObjects);

	return true;
}

void MeshSlotMap::Deinitialize()
{
	m_objects.clear();
	m_freeList.clear();
	m_slots.clear();
}

void MeshSlotMap::Render(Graphics& graphics, GLuint MatrixID, glm::mat4 proj, glm::mat4 view)
{
	int	mul = m_count / 2;
	if (m_count == 1)
		mul = 1;

	for (int i = 0; i < m_count; ++i)
	{
		if (!m_objects[i].IsReady())
			continue;

		glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !

		Model = glm::translate(Model, glm::vec3(25.0f * (i - mul), 0.0f, 0.0f));

		glm::mat4 MVP = proj * view * Model; // Remember, matrix multiplication is the other way around

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		m_objects[i].Render(graphics, Model);
	}
}

MeshSlotMap::MeshSlotMap()
{
}

MeshSlotMap::~MeshSlotMap()
{
}

int MeshSlotMap::Count() const
{
	return m_count;
}
