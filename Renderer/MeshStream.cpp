#include <vector>

#include "MeshStream.hpp"

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

GLbyte MeshStream::ReadByte()
{
	unsigned char	b;
	m_stream.read(reinterpret_cast<char*>(&b), sizeof(unsigned char));
	return b;
}

uint16_t MeshStream::ReadUInt16()
{
	uint16_t	u16;
	m_stream.read(reinterpret_cast<char*>(&u16), sizeof(uint16_t));
	return u16;
}