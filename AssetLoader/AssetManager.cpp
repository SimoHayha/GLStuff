#include <string>
#include <iostream>

#include "dirent.h"
#include "AssetHelper.h"
#include "AbstractAsset.h"
#include "AssetManager.h"

AssetManager::AssetManager(std::string assetsPath) :
m_assetsPath(assetsPath)
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::ScanProject()
{
	GenerateAssets(m_assetsPath);
}

void AssetManager::GenerateAssets(std::string dirname)
{
	DIR*		dir;
	char		buffer[PATH_MAX + 2];
	char*		p = buffer;
	char const*	src;
	char*		end = &buffer[PATH_MAX];
	int			ok;

	src = dirname.c_str();
	while (p < end && *src != '\0')
		*p++ = *src++;
	*p = '\0';

	dir = opendir(dirname.c_str());
	if (dir)
	{
		struct dirent*	ent;

		while ((ent = readdir(dir)) != nullptr)
		{
			char*	q = p;
			char	c;

			if (buffer < q)
				c = q[-1];
			else
				c = ':';

			if (c != ':' && c != '/' && c != '\\')
				*q++ = '/';

			src = ent->d_name;
			while (q < end && *src != '\0')
				*q++ = *src++;
			*q = '\0';

			switch (ent->d_type)
			{
			case DT_LNK:
			case DT_REG:
			{
				std::string file(buffer);
				std::string	ext = file.substr(file.find_last_of(".") + 1);

				AbstractAsset* asset = m_loader.Generate(file, ext);
				if (asset)
				{
					if (asset->Load())
					{
						asset->Serialize();
					}
				}

				break;
			}

			case DT_DIR:
				if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
					GenerateAssets(buffer);
				break;

			default:
				/*NOP*/;
			}
		}

		closedir(dir);
		ok = 1;
	}
	else
	{
		std::cerr << "Cannot open directory " << dirname << std::endl;
		ok = 0;
	}
}
