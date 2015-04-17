#ifndef UNKNOWNASSET_H__
#define UNKNOWNASSET_H__

#include "AbstractAsset.h"

class UnknownAsset : public AbstractAsset
{
public:
	UnknownAsset(std::string const& file, std::string const& path);
	virtual ~UnknownAsset();

	virtual bool OnLoad() override;
	virtual void OnUnload() override;
	virtual bool OnSerialize() override;
};

#endif