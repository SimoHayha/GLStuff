#ifndef ASSETHELPER_H__
#define ASSETHELPER_H__

#include <string>

class AssetHelper
{
public:
	virtual ~AssetHelper();

	static std::string	NewUUID();
	static std::string	GetProjectDirectory();

protected:
	AssetHelper();
};

#endif