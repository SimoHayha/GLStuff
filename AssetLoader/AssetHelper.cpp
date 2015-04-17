#include "AssetHelper.h"

extern "C"
{
#ifdef WIN32
#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")
#else
#include <uuid/uuid.h>
#endif

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
}

AssetHelper::AssetHelper()
{
}

AssetHelper::~AssetHelper()
{
}

std::string AssetHelper::NewUUID()
{
#ifdef WIN32
	UUID uuid;
	UuidCreate(&uuid);
	//CoCreateGuid(&uuid);

	unsigned char*	str;
	UuidToStringA(&uuid, &str);

	std::string	s((char*)str);

	RpcStringFreeA(&str);
#else
	uuid_t	uuid;
	uuid_generate_random(uuid);
	char	s[37];
	uuid_unparse(uuid, s);
#endif
	return s;
}

std::string AssetHelper::GetProjectDirectory()
{
	char	currentPath[FILENAME_MAX];

	if (!GetCurrentDir(currentPath, sizeof(currentPath)))
		return strerror(errno);

	currentPath[sizeof(currentPath) - 1] = '\0';

	return currentPath;
}