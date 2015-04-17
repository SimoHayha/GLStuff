#include "AbstractAsset.h"
#include "TextureAsset.h"
#include "UnknownAsset.h"
#include "AssetLoader.h"

AssetLoader::AssetLoader()
{
}

AssetLoader::~AssetLoader()
{
}

AbstractAsset* AssetLoader::Generate(std::string const& file, std::string const& ext)
{
	if (ext == "dds")
		return new TextureAsset(file, std::string());
	else
		return new UnknownAsset(file, std::string());

	return nullptr;
}

AbstractAsset* AssetLoader::GenerateFromExisting(std::string const& file, std::string const& ext)
{
	int lastindex = file.find_last_of(".");
	std::string name = file.substr(0, lastindex);



	return nullptr;
}
