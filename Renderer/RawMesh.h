#pragma once

#include <fstream>
#include <cstdint>
#include <string>

#include "MeshStream.hpp"
#include "RawMeshData.h"

#include <GL\glew.h>
#include <glm\glm.hpp>

class RawMesh
{
public:
	RawMesh() = default;
	~RawMesh() = default;

	RawMesh(RawMesh const&) = delete;
	RawMesh(RawMesh&&) = delete;
	RawMesh&	operator=(RawMesh const&) = delete;
	RawMesh&	operator=(RawMesh&&) = delete;

	RawMeshData&	Data();

	static RawMesh*	Read(MeshStream& stream, std::wstring const& shaderPathLocation, std::wstring const& texturePathLocation);

private:
	static void		StripPath(std::wstring& path);

private:
	RawMeshData	m_data;
};