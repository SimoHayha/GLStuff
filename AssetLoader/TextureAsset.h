#ifndef TEXTUREASSET_H__
#define TEXTUREASSET_H__

#include "AbstractAsset.h"

class TextureAsset : public AbstractAsset
{
public:
	TextureAsset(std::string const& file, std::string const& path);
	virtual ~TextureAsset();

	virtual bool OnLoad() override;
	virtual void OnUnload() override;
	virtual bool OnSerialize() override;

protected:
	unsigned int	m_forceChannels;
	unsigned int	m_flags;
};

#endif