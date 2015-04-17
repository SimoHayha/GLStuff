#ifndef ASSETMANAGER_H__
#define ASSETMANAGER_H__

#include <string>
#include <map>
#include <list>
#include <functional>

#include "AssetLoader.h"

class AbstractAsset;

class AssetManager
{
public:
	AssetManager(std::string assetsPath);
	virtual ~AssetManager();

	void ScanProject();

private:
	std::string								m_assetsPath;
	AssetLoader								m_loader;

	std::map<std::string, AbstractAsset*>	m_assets;

private:
	void GenerateAssets(std::string dirname);
};

#endif