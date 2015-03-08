#pragma once

#include <future>
#include <string>
#include <map>
#include <list>
#include <condition_variable>
#include <mutex>

#include <glm\glm.hpp>

#include <GL/glew.h>

// Mesh are loaded in a parallel thread, but we can only generate buffers in the main thread.
// The loaded thread will wait for the Condition variable to be released. The main thread will release it.
// The GenBuffers struct is a link between both threads.
// The BufferIds vector will contains all the ids.
struct GenBuffers
{
	GenBuffers()
	{
		Mutex = new std::mutex;
	}

	~GenBuffers()
	{
		delete Mutex;
	}

	size_t					ThreadId;
	std::mutex*				Mutex;
	std::condition_variable	Condition;
	GLuint					Count;
	std::vector<GLuint>		BuffersIds;
};

struct MaterialConstants
{
	MaterialConstants();

	glm::vec4	Ambient;
	glm::vec4	Diffuse;
	glm::vec4	Specular;
	glm::vec4	Emissive;
	float		SpecularPower;
	float		Padding0;
	float		Padding1;
	float		Padding2;
};

struct LightConstants
{
	LightConstants();

	glm::vec4	Ambient;
	glm::vec4	LightColor[4];
	glm::vec4	LightAttenuation[4];
	glm::vec4	LightDirection[4];
	glm::vec4	LightSpecularIntensity[4];
	GLuint		IsPointLight[4];
	GLuint		ActiveLight;
	float		Padding0;
	float		Padding1;
	float		Padding2;
};

struct ObjectConstants
{
	ObjectConstants();

	glm::mat4x4	LocalToWorld4x4;
	glm::mat4x4	LocalToProjected4x4;
	glm::mat4x4	WorldToLocal4x4;
	glm::mat4x4	WorldToView4x4;
	glm::mat4x4	UvTransform4x4;
	glm::vec3	EyePosition;
	float		Padding0;
};

struct MiscConstants
{
	MiscConstants();

	float	ViewportWidth;
	float	ViewportHeight;
	float	Time;
	float	Padding1;
};

struct GLFWwindow;

class Graphics
{
public:
	Graphics();
	~Graphics();

	void		SetWindow(GLFWwindow* window);
	GLFWwindow*	GetWindow() const;

	GLuint	GetMaterialConstants() const;
	GLuint	GetLightConstants() const;
	GLuint	GetObjectConstants() const;
	GLuint	GetMiscConstants() const;

	std::future<GLuint>	GetOrCreateShaderAsync(std::string const& shaderName);

	void	UpdateGenBuffers();
	void	AddGenBuffers(GenBuffers* task);

	void	UpdateMaterialConstants(MaterialConstants& data) const;
	void	UpdateLightConstants(LightConstants& data) const;
	void	UpdateObjectConstants(ObjectConstants& data) const;
	void	UpdateMiscConstants(MiscConstants& data) const;

private:
	std::map<std::string, GLuint>	m_shaderResources;
	std::map<std::string, GLuint>	m_textureResources;

	GLuint	m_materialConstants;
	GLuint	m_lightConstants;
	GLuint	m_objectConstants;
	GLuint	m_miscConstants;

	std::list<GenBuffers*>	m_genBuffers;
	std::mutex*				m_genBuffersLock;

	GLFWwindow*	m_window;
};