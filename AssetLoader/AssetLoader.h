#ifndef ABSTRACTLOADER_H__
#define ABSTRACTLOADER_H__

#include <string>

class AbstractAsset;

class AssetLoader
{
public:
	AssetLoader();
	virtual ~AssetLoader();

	AbstractAsset*	Generate(std::string const& file, std::string const& ext);
	AbstractAsset*	GenerateFromExisting(std::string const& file, std::string const& ext);

};

#endif