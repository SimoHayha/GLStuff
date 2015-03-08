#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <future>
#include <vector>
#include <cstdint>
#include <fstream>

#include <GL/glew.h>
#include <glm\glm.hpp>

#include "Vertex.h"
#include "Triangle.h"
#include "Graphics.h"
#include "Defines.h"

class MeshStream
{
public:
	MeshStream(std::ifstream& stream);

	uint32_t	ReadUInt32();
	uint16_t	ReadUInt16();
	void		ReadString(std::wstring* output);
	void		ReadString(std::wstring* output, uint32_t count);
	GLbyte		ReadByte();

	template <typename T>
	void		ReadStruct(T* output, size_t size = sizeof(T));

	std::ifstream&	file();

private:
	std::ifstream&	m_stream;
};

template <typename T>
void MeshStream::ReadStruct(T* output, size_t size /*= sizeof(T)*/)
{
	m_stream.read(reinterpret_cast<char*>(output), size);
#ifdef _DEBUG
	if (m_stream.fail())
		std::cerr << "ERROR: ReadStruct failure" << std::endl;
#endif
}

class MeshSlotMap;

class Mesh
{
public:
	static const UINT	MaxTexture = 8;

	struct SubMesh
	{
		UINT	MaterialIndex;
		UINT	IndexBufferIndex;
		UINT	VertexBufferIndex;
		UINT	StartIndex;
		UINT	PrimCount;
	};

	struct Material
	{
		Material() { ZeroMemory(this, sizeof(Material)); }
		~Material() {}

		std::wstring	Name;

		glm::mat4x4		UVTransform;

		float	Ambient[4];
		float	Diffuse[4];
		float	Specular[4];
		float	Emissive[4];
		float	SpecularPower;

		GLuint	Textures[MaxTexture];
		GLuint	VertexShader;
		GLuint	PixelShader;
	};

	Mesh();

	void	Render(Graphics& graphics, glm::mat4x4 const& model);

	static std::future<void>	LoadFromFileAsync(bool& ready, Graphics& graphics, std::wstring const& meshFilename, std::wstring const& shaderPathLocation, std::wstring const& texturePathLocation, MeshSlotMap* slotMap);

	int		GetIndex() const;
	int		GetVersion() const;
	bool	IsReady() const;
	void	SetIndex(int index);
	void	SetVersion(int version);

private:
	static void	Read(Mesh& mesh, Graphics& graphics, MeshStream& stream, std::wstring const& shaderPathLocation, std::wstring const& texturePathLocation);
	static void	StripPath(std::wstring& path);

private:
	std::vector<SubMesh>	m_submeshes;
	std::vector<Material>	m_materials;
	std::vector<GLuint>		m_vertexBuffers;
	std::vector<GLuint>		m_skinningVertexBuffer;
	std::vector<GLuint>		m_indexBuffers;
	std::vector<Triangle>	m_triangles;

	int		m_index;
	int		m_version;
	bool	m_ready;

	std::wstring	m_name;
};
