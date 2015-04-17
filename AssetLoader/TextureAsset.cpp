#include <iostream>
#include <fstream>
#include <sstream>

#include <GL\glew.h>

#include "SOIL.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "TextureAsset.h"

TextureAsset::TextureAsset(std::string const& file, std::string const& path) :
AbstractAsset(file, path),
m_forceChannels(SOIL_LOAD_AUTO),
m_flags(SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT)
{
}

TextureAsset::~TextureAsset()
{
}

bool TextureAsset::OnLoad()
{
	GLuint tex_2d = SOIL_load_OGL_texture(m_name.c_str(), m_forceChannels, SOIL_CREATE_NEW_ID, m_flags);

	if (0 == tex_2d)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
		return false;
	}

	std::cout << m_name << " loaded" << std::endl;
	return true;
}

void TextureAsset::OnUnload()
{
}

bool TextureAsset::OnSerialize()
{
	char	buff[64];

	rapidxml::xml_document<>	doc;
	sprintf(buff, "%d", AssetType::TEXTURE);
	rapidxml::xml_node<>*		assetid = doc.allocate_node(rapidxml::node_element, "AssetID", buff);
	rapidxml::xml_node<>*		guid = doc.allocate_node(rapidxml::node_element, "GUID", m_guid.c_str());
	rapidxml::xml_node<>*		textureImporter = doc.allocate_node(rapidxml::node_element, "TextureImporter");
	sprintf(buff, "%d", m_forceChannels);
	rapidxml::xml_node<>*		forceChannels = doc.allocate_node(rapidxml::node_element, "forcechannels", buff);
	sprintf(buff, "%d", m_flags);
	rapidxml::xml_node<>*		flags = doc.allocate_node(rapidxml::node_element, "flags", buff);

	textureImporter->append_node(forceChannels);
	textureImporter->append_node(flags);
	doc.append_node(assetid);
	doc.append_node(guid);
	doc.append_node(textureImporter);

	std::string	xmlAsString;
	rapidxml::print(std::back_inserter(xmlAsString), doc);

	std::ofstream	file(m_name.append(".asset"));
	file << doc;
	file.close();
	doc.clear();

	return true;
}