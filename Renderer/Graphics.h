#pragma once

#include <future>
#include <string>
#include <map>

#include <GL/glew.h>

class Graphics
{
public:
	GLuint	GetMaterialConstants() const;
	GLuint	GetLightConstants() const;
	GLuint	GetObjectConstants() const;
	GLuint	GetMiscConstants() const;

	std::future<GLuint>	GetOrCreateShaderAsync(std::string const& shaderName);

private:
	std::map<std::string, GLuint>	m_shaderResources;
	std::map<std::string, GLuint>	m_textureResources;
};