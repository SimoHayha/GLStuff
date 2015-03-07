#pragma once

#include <future>
#include <string>
#include <map>

#include <GL/glew.h>

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

class Graphics
{
public:
	GLuint	GetMaterialConstants() const;
	GLuint	GetLightConstants() const;
	GLuint	GetObjectConstants() const;
	GLuint	GetMiscConstants() const;

	std::future<GLuint>	GetOrCreateShaderAsync(std::string const& shaderName);

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
};