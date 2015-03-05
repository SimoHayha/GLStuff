#include "Resources.h"
#include "Graphics.h"

std::future<GLuint> Graphics::GetOrCreateShaderAsync(std::string const& shaderName)
{
	std::future<GLuint>	f = std::async(std::launch::async, [](std::string const& shaderName){
		return Resources::LoadShader(shaderName.c_str(), shaderName.c_str());
	}, shaderName);

	m_textureResources[shaderName] = f.get();

	return f;
}
