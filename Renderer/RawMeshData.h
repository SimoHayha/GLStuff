#pragma once

#include <vector>
#include <string>

#include "Defines.h"
#include "Vertex.h"
#include "Triangle.h"

#include <GL\glew.h>
#include <glm\glm.hpp>

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
	Material() { memset(this, 0, sizeof(Material)); }
	~Material() = default;

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

struct RawMeshData
{
	std::vector<SubMesh>				SubMeshes;
	std::vector<Material>				Materials;
	std::vector<std::vector<USHORT>>	Indices;
	std::vector<std::vector<Vertex>>	Vertices;
	std::vector<Triangle>				Triangles;

	std::wstring	Name;
};