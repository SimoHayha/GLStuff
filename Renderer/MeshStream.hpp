#pragma once

#ifdef _DEBUG
#include <iostream>
#endif
#include <fstream>
#include <cstdint>

#include <GL\glew.h>
#include <glm\glm.hpp>

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