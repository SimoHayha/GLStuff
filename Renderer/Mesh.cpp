#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdint>

#include <codecvt>
#include <fcntl.h>
#include <io.h>
#include <intrin.h>

#include "Mesh.h"

MeshStream::MeshStream(std::ifstream& stream) :
m_stream(stream)
{
}

uint32_t MeshStream::ReadUInt32()
{
	uint32_t	u32;
	m_stream.read(reinterpret_cast<char*>(&u32), sizeof(uint32_t));
	return u32;
}

void MeshStream::ReadString(std::wstring* output)
{
	uint32_t	count = ReadUInt32();
	ReadString(output, count);
}

void MeshStream::ReadString(std::wstring* output, uint32_t count)
{
	if (count == 0u)
		return;

	std::vector<wchar_t>	characters(count);
	ReadStruct(&characters[0], count * sizeof(wchar_t));
	*output = &characters[0];
}

std::ifstream& MeshStream::file()
{
	return m_stream;
}

std::future<void> Mesh::LoadFromFileAsync(std::string const& meshFilename, std::vector<Mesh*>& loadedMesh, bool clearLoadedMeshesVector /*= true*/)
{
	if (clearLoadedMeshesVector)
		loadedMesh.clear();

	auto	fct = std::async([](std::string const& meshFilename, std::vector<Mesh*>& loadedMesh){

		//std::ifstream	file(meshFilename, std::ios::binary);

		std::ifstream	file;
		file.open(meshFilename, std::ios::binary);
		if (!file.is_open())
			return;
		file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<char, 0x10ffff, std::consume_header>()));

		file.seekg(0, std::ios::end);
		std::streamsize	size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::cout << "Reading " << meshFilename << std::endl;
		MeshStream	stream(file);

		uint32_t	remainingMeshesToRead = stream.ReadUInt32();

		for (uint32_t i = 0u; i < remainingMeshesToRead; ++i)
		{
			Mesh*	mesh = Read(stream, loadedMesh);
			if (mesh)
				loadedMesh.push_back(mesh);
		}

	}, meshFilename, loadedMesh);

	return fct;
}

Mesh* Mesh::Read(MeshStream& stream, std::vector<Mesh*>& loadedMesh)
{
	std::cout << "Submesh" << std::endl;

	Mesh*	mesh = new Mesh();

	stream.ReadString(&mesh->m_name);
	std::wcout << "Name: " << mesh->m_name << std::endl;

	// Read material count
	uint32_t	numMaterials = stream.ReadUInt32();
	mesh->m_materials.resize(numMaterials);
	std::wcout << "Material count: " << numMaterials << std::endl;

	// Load each material
	for (uint32_t i = 0u; i < numMaterials; ++i)
	{
		std::cout << "Material" << std::endl;
		Material&	material = mesh->m_materials[i];

		// Read the material name
		stream.ReadString(&material.Name);
		std::wcout << "\tName:" << material.Name << std::endl;

		// Read the ambient and diffuse properties of material
		std::cout << stream.file().tellg() << std::endl;
		stream.ReadStruct(&material.Ambient, sizeof(material.Ambient));
		std::cout << stream.file().tellg() << std::endl;
		stream.ReadStruct(&material.Diffuse, sizeof(material.Diffuse));
		std::cout << stream.file().tellg() << std::endl;
		stream.ReadStruct(&material.Specular, sizeof(material.Specular));
		std::cout << stream.file().tellg() << std::endl;
		stream.ReadStruct(&material.SpecularPower);
		std::cout << stream.file().tellg() << std::endl;
		stream.ReadStruct(&material.Emissive, sizeof(material.Emissive));
		std::cout << stream.file().tellg() << std::endl;
		stream.ReadStruct(&material.UVTransform);
		std::cout << stream.file().tellg() << std::endl;

		std::cout << "\tAmbient" << material.Ambient[0] << "," << material.Ambient[1] << "," << material.Ambient[2] << "," << material.Ambient[3] << std::endl;
		std::cout << "\tDiffuse" << material.Diffuse[0] << "," << material.Diffuse[1] << "," << material.Diffuse[2] << "," << material.Diffuse[3] << std::endl;
		std::cout << "\tSpecular" << material.Specular[0] << "," << material.Specular[1] << "," << material.Specular[2] << "," << material.Specular[3] << std::endl;
		std::cout << "\tSpecularPower" << material.SpecularPower << std::endl;
		std::cout << "\tEmissive" << material.Emissive[0] << "," << material.Emissive[1] << "," << material.Emissive[2] << "," << material.Emissive[3] << std::endl;
		//std::cout << "\tUVTransform" << material.UVTransform << std::endl;
	}


	return nullptr;
}
