#include <iostream>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "Resources.h"
#include "Graphics.h"

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

std::future<GLuint> Graphics::GetOrCreateShaderAsync(std::string const& shaderName)
{
	std::future<GLuint>	f = std::async(std::launch::async, [](std::string const& shaderName){
		return Resources::LoadShader(shaderName.c_str(), shaderName.c_str());
	}, shaderName);

	m_textureResources[shaderName] = f.get();

	return f;
}

GLuint Graphics::GetMaterialConstants() const
{
	return m_materialConstants;
}

GLuint Graphics::GetLightConstants() const
{
	return m_lightConstants;
}

GLuint Graphics::GetObjectConstants() const
{
	return m_objectConstants;
}

GLuint Graphics::GetMiscConstants() const
{
	return m_miscConstants;
}

void Graphics::UpdateMaterialConstants(MaterialConstants& data) const
{
}

void Graphics::UpdateLightConstants(LightConstants& data) const
{

}

void Graphics::UpdateObjectConstants(ObjectConstants& data) const
{

}

void Graphics::UpdateMiscConstants(MiscConstants& data) const
{

}

void Graphics::SetWindow(GLFWwindow* window)
{
	m_window = window;
}

GLFWwindow* Graphics::GetWindow() const
{
	return m_window;
}

void Graphics::UpdateGenBuffers()
{
	if (m_genBuffersLock.try_lock())
	{
		for (auto& task : m_genBuffers)
		{
			//std::lock_guard<std::mutex>	lk(*task->Mutex);
			std::cout << "Generate " << task->Count << " buffer(s) for thread " << task->ThreadId << std::endl;
			task->BuffersIds.resize(task->Count);
			glGenBuffers(task->Count, &task->BuffersIds[0]);
			std::cout << "Buffer " << task->BuffersIds[0] << " created" << std::endl;
			task->Condition.notify_one();
		}
		m_genBuffers.clear();
		m_genBuffersLock.unlock();
	}
}

void Graphics::AddGenBuffers(GenBuffers* task)
{
	m_genBuffersLock.lock();
	m_genBuffers.push_back(task);
	m_genBuffersLock.unlock();
}

void Graphics::Update()
{

}

void Graphics::AddMeshToCollect(Mesh* mesh)
{

}
