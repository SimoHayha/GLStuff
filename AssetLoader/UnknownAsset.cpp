#include <iostream>
#include <fstream>
#include <sstream>


#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "UnknownAsset.h"

UnknownAsset::UnknownAsset(std::string const& file, std::string const& path) :
AbstractAsset(file, path)
{
}

UnknownAsset::~UnknownAsset()
{
}

bool UnknownAsset::OnLoad()
{
	return true;
}

void UnknownAsset::OnUnload()
{
}

bool UnknownAsset::OnSerialize()
{
	char	buff[64];

	rapidxml::xml_document<>	doc;
	sprintf(buff, "%d", AssetType::UNKNOWN);
	rapidxml::xml_node<>*		assetid = doc.allocate_node(rapidxml::node_element, "AssetID", buff);
	rapidxml::xml_node<>*		guid = doc.allocate_node(rapidxml::node_element, "GUID", m_guid.c_str());

	doc.append_node(assetid);
	doc.append_node(guid);

	std::string	xmlAsString;
	rapidxml::print(std::back_inserter(xmlAsString), doc);

	std::ofstream	file(m_name.append(".asset"));
	file << doc;
	file.close();
	doc.clear();

	return true;
}