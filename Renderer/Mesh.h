#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <future>
#include <vector>
#include <cstdint>
#include <fstream>

#include <GL/glew.h>
#include <glm\glm.hpp>

#include "Defines.h"

class MeshStream
{
public:
	MeshStream(std::ifstream& stream);

	uint32_t	ReadUInt32();
	void		ReadString(std::wstring* output);
	void		ReadString(std::wstring* output, uint32_t count);

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
	if (m_stream.failbit == std::ios_base::eofbit)
		std::cerr << "ERROR: ReadStruct failure (eofbit)" << std::endl;
	else if (m_stream.failbit == std::ios_base::failbit)
		std::cerr << "ERROR: ReadStruct failure (failbit)" << std::endl;
	else if (m_stream.failbit == std::ios_base::badbit)
		std::cerr << "ERROR: ReadStruct failure (badbit)" << std::endl;
#endif
}

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

	static std::future<void>	LoadFromFileAsync(std::string const& meshFilename, std::vector<Mesh*>& loadedMesh, bool clearLoadedMeshesVector = true);

private:
	static Mesh*	Read(MeshStream& stream, std::vector<Mesh*>& loadedMesh);

private:
	std::vector<SubMesh>	m_submeshes;
	std::vector<Material>	m_materials;
	std::vector<GLuint>		m_vertexBuffers;
	std::vector<GLuint>		m_skinningVertexBuffer;
	std::vector<GLuint>		m_indexBuffers;

	std::wstring	m_name;
};
