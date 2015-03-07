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
